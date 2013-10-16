/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * Jerry chuang <wlanfae@realtek.com>
******************************************************************************/
#ifdef DIG

#include "r8180_dm.h"
#include "r8180_hw.h"
#include "r8180_rtl8225.h"

#define Z1_HIPWR_UPPER_TH			99
#define Z1_HIPWR_LOWER_TH			70	
#define Z2_HIPWR_UPPER_TH			99
#define Z2_HIPWR_LOWER_TH			90

bool CheckDig(struct net_device *dev)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
	struct ieee80211_device *ieee = priv->ieee80211;

	if(ieee->state != IEEE80211_LINKED)
		return false;

	if(priv->card_8187 == NIC_8187B) {
		if(!priv->bDigMechanism && !priv->bCCKThMechanism)
			return false;

		if(priv->CurrentOperaRate < 36) 
			return false;
	} else { 
		if(!priv->bDigMechanism)
			return false;

		if(priv->CurrentOperaRate < 48)
			return false;
	}
	return true;
}


void DIG_Zebra(struct net_device *dev)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
	u16			CCKFalseAlarm, OFDMFalseAlarm;
	u16			OfdmFA1, OfdmFA2;
	int			InitialGainStep = 7; 
	int			LowestGainStage = 4; 


	if(priv->card_8187_Bversion == VERSION_8187B_B)
	{
		CCKFalseAlarm = 0;
		OFDMFalseAlarm = (u16)(priv->FalseAlarmRegValue);
		OfdmFA1 =  0x01;
		OfdmFA2 = priv->RegDigOfdmFaUpTh; 
	}
	else
	{
		CCKFalseAlarm = (u16)(priv->FalseAlarmRegValue & 0x0000ffff);
		OFDMFalseAlarm = (u16)((priv->FalseAlarmRegValue >> 16) & 0x0000ffff);
		OfdmFA1 =  0x15;
		OfdmFA2 = ((u16)(priv->RegDigOfdmFaUpTh)) << 8;
	}	




	if(priv->card_8187 == NIC_8187) {
		if (priv->InitialGain == 0 ) 
		{
			switch( priv->rf_chip)
			{
				case RF_ZEBRA:
					priv->InitialGain = 5; 
					break;
				case RF_ZEBRA2:
					priv->InitialGain = 4; 
					break;
				default:
					priv->InitialGain = 5; 
					break;
			}
		}
		InitialGainStep = 7;
		if(priv->InitialGain > 7)
			priv->InitialGain = 5;
		LowestGainStage = 4;
	} 
	else 
	{
		if (priv->InitialGain == 0 ) 
		{ 
			priv->InitialGain = 4; 
		}
		if(priv->card_8187_Bversion != VERSION_8187B_B)
		{ 
			OfdmFA1 =  0x20;
		}
		InitialGainStep = 8;
		LowestGainStage = priv->RegBModeGainStage; 
	}

	if (OFDMFalseAlarm > OfdmFA1)
	{
		if (OFDMFalseAlarm > OfdmFA2)
		{
			priv->DIG_NumberFallbackVote++;
			if (priv->DIG_NumberFallbackVote >1)
			{
				if (priv->InitialGain < InitialGainStep)
				{
					priv->InitialGain = (priv->InitialGain + 1);
					UpdateInitialGain(dev); 
				}
				priv->DIG_NumberFallbackVote = 0;
				priv->DIG_NumberUpgradeVote=0;
			}
		}
		else
		{
			if (priv->DIG_NumberFallbackVote)
				priv->DIG_NumberFallbackVote--;
		}
		priv->DIG_NumberUpgradeVote=0;		
	}
	else	
	{
		if (priv->DIG_NumberFallbackVote)
			priv->DIG_NumberFallbackVote--;
		priv->DIG_NumberUpgradeVote++;

		if (priv->DIG_NumberUpgradeVote>9)
		{
			if (priv->InitialGain > LowestGainStage) 
			{
				priv->InitialGain = (priv->InitialGain - 1);
				UpdateInitialGain(dev); 
			}
			priv->DIG_NumberFallbackVote = 0;
			priv->DIG_NumberUpgradeVote=0;
		}
	}

}

void DynamicInitGain(struct net_device *dev)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

	switch(priv->rf_chip)
	{
		case RF_ZEBRA:
		case RF_ZEBRA2:  
			DIG_Zebra(dev);
			break;
		
		default:
			printk("DynamicInitGain(): unknown RFChipID(%d) !!!\n", priv->rf_chip);
			break;
	}
}

void DynamicCCKThreshold(struct net_device *dev)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
	u16			CCK_Up_Th;
	u16			CCK_Lw_Th;
	u16			CCKFalseAlarm;


	CCK_Up_Th = priv->CCKUpperTh;
	CCK_Lw_Th = priv->CCKLowerTh;	
	CCKFalseAlarm = (u16)((priv->FalseAlarmRegValue & 0x0000ffff) >> 8); 

	if(priv->StageCCKTh < 3 && CCKFalseAlarm >= CCK_Up_Th)
	{
		priv->StageCCKTh ++;
		UpdateCCKThreshold(dev);
	}
	else if(priv->StageCCKTh > 0 && CCKFalseAlarm <= CCK_Lw_Th)
	{
		priv->StageCCKTh --;
		UpdateCCKThreshold(dev);
	}
	
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_hw_dig_wq (struct work_struct *work)
{
        struct delayed_work *dwork = container_of(work,struct delayed_work,work);
        struct ieee80211_device *ieee = container_of(dwork,struct ieee80211_device,hw_dig_wq);
        struct net_device *dev = ieee->dev;
#else
void rtl8180_hw_dig_wq(struct net_device *dev)
{
#endif
	struct r8180_priv *priv = ieee80211_priv(dev);

	if(priv->card_8187_Bversion == VERSION_8187B_B) {
		priv->FalseAlarmRegValue = (u32)read_nic_byte(dev, (OFDM_FALSE_ALARM+1));
	} else {
		priv->FalseAlarmRegValue = read_nic_dword(dev, CCK_FALSE_ALARM);
	}

	if(priv->bDigMechanism) {
		DynamicInitGain(dev);
	}

	if(priv->card_8187 == NIC_8187B) {
		if(priv->bCCKThMechanism)
		{
			DynamicCCKThreshold(dev);
		}
	}
}

void SetTxPowerLevel8187(struct net_device *dev, short chan)
{
	struct r8180_priv *priv = ieee80211_priv(dev);

	switch(priv->rf_chip)
	{
	case  RF_ZEBRA:
		rtl8225_SetTXPowerLevel(dev,chan);
		break;

	case RF_ZEBRA2:
		rtl8225z2_SetTXPowerLevel(dev,chan);
		break;
	}
}

void DoRxHighPower(struct net_device *dev)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	TR_SWITCH_STATE TrSwState;
	u16		HiPwrUpperTh = 0;
	u16		HiPwrLowerTh = 0;
	u16		RSSIHiPwrUpperTh = 0;
	u16		RSSIHiPwrLowerTh = 0;	

	if((priv->card_8187 == NIC_8187B)||(priv->card_8187 == NIC_8187)) {


		TrSwState = priv->TrSwitchState;

		switch(priv->rf_chip)
		{
			case RF_ZEBRA:
				HiPwrUpperTh = Z1_HIPWR_UPPER_TH;
				HiPwrLowerTh = Z1_HIPWR_LOWER_TH;
				break;	

			case RF_ZEBRA2:
				if((priv->card_8187 == NIC_8187)) {
					HiPwrUpperTh = Z2_HIPWR_UPPER_TH;
					HiPwrLowerTh = Z2_HIPWR_LOWER_TH;
				} else {

					HiPwrUpperTh = priv->Z2HiPwrUpperTh;
					HiPwrLowerTh = priv->Z2HiPwrLowerTh;
					HiPwrUpperTh = HiPwrUpperTh * 10;
					HiPwrLowerTh = HiPwrLowerTh * 10;

					RSSIHiPwrUpperTh = priv->Z2RSSIHiPwrUpperTh;
					RSSIHiPwrLowerTh = priv->Z2RSSIHiPwrLowerTh;
				}
				break;	

			default:
				return;
				break;	
		}

		if((priv->card_8187 == NIC_8187)) {
			if (priv->UndecoratedSmoothedSS > HiPwrUpperTh)
			{ 
				if( priv->TrSwitchState == TR_HW_CONTROLLED )	
				{
					write_nic_byte(dev, RFPinsSelect, 
							(u8)(priv->wMacRegRfPinsSelect | TR_SW_MASK_8187 ));
					write_nic_byte(dev, RFPinsOutput, 
						(u8)((priv->wMacRegRfPinsOutput&(~TR_SW_MASK_8187))|TR_SW_MASK_TX_8187));
					priv->TrSwitchState = TR_SW_TX;
					priv->bToUpdateTxPwr = true;
				}
			}
			else if (priv->UndecoratedSmoothedSS < HiPwrLowerTh)
			{ 
				if( priv->TrSwitchState == TR_SW_TX)	
				{
					write_nic_byte(dev, RFPinsOutput, (u8)(priv->wMacRegRfPinsOutput));
					write_nic_byte(dev, RFPinsSelect, (u8)(priv->wMacRegRfPinsSelect));
					priv->TrSwitchState = TR_HW_CONTROLLED;
					priv->bToUpdateTxPwr = true;
				}
			}
		}else {
			if(TrSwState == TR_HW_CONTROLLED)
			{
				if((priv->UndecoratedSmoothedSS > HiPwrUpperTh) ||
						(priv->bCurCCKPkt && (priv->CurCCKRSSI > RSSIHiPwrUpperTh)))
				{
					write_nic_byte(dev, RFPinsSelect, (u8)(priv->wMacRegRfPinsSelect|TR_SW_MASK_8187 ));
					write_nic_byte(dev, RFPinsOutput, 
						(u8)((priv->wMacRegRfPinsOutput&(~TR_SW_MASK_8187))|TR_SW_MASK_TX_8187));
					priv->TrSwitchState = TR_SW_TX;
					priv->bToUpdateTxPwr = true;
				}
			}
			else
			{
				if((priv->UndecoratedSmoothedSS < HiPwrLowerTh) &&
						(!priv->bCurCCKPkt || priv->CurCCKRSSI < RSSIHiPwrLowerTh))
				{

					write_nic_byte(dev, RFPinsOutput, (u8)(priv->wMacRegRfPinsOutput));
					write_nic_byte(dev, RFPinsSelect, (u8)(priv->wMacRegRfPinsSelect));
					priv->TrSwitchState = TR_HW_CONTROLLED;
					priv->bToUpdateTxPwr = true;
				}
			}
		}
	}
}


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_tx_pw_wq (struct work_struct *work)
{
        struct delayed_work *dwork = container_of(work,struct delayed_work,work);
        struct ieee80211_device *ieee = container_of(dwork,struct ieee80211_device,tx_pw_wq);
        struct net_device *dev = ieee->dev;
#else
void rtl8180_tx_pw_wq(struct net_device *dev)
{
#endif

	struct r8180_priv *priv = ieee80211_priv(dev);

	
	if(priv->bToUpdateTxPwr)
	{
		priv->bToUpdateTxPwr = false;
		SetTxPowerLevel8187(dev, priv->chan);
	}
	
	DoRxHighPower(dev);	
}

bool CheckHighPower(struct net_device *dev)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	struct ieee80211_device *ieee = priv->ieee80211;
	if(!priv->bRegHighPowerMechanism)
	{
		return false;
	}
		
	if((ieee->state == IEEE80211_LINKED_SCANNING))
	{
		return false;
	}

	return true;
}

#ifdef SW_ANTE_DIVERSITY

#define ANTENNA_DIVERSITY_TIMER_PERIOD          1000 

void
SwAntennaDiversityRxOk8185(
	struct net_device *dev, 
	u8 SignalStrength
	)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);


	priv->AdRxOkCnt++;

	if( priv->AdRxSignalStrength != -1)
	{
		priv->AdRxSignalStrength = ((priv->AdRxSignalStrength*7) + (SignalStrength*3)) / 10;
	}
	else
	{ 
		priv->AdRxSignalStrength = SignalStrength;
	}
        
	if( priv->LastRxPktAntenna ) 
		priv->AdMainAntennaRxOkCnt++;	
	else	 
		priv->AdAuxAntennaRxOkCnt++;
}

bool
SetAntenna8185(
	struct net_device *dev,
	u8		u1bAntennaIndex
	)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
	bool bAntennaSwitched = false;


	switch(u1bAntennaIndex)
	{
	case 0:
		switch(priv->rf_chip)
		{
		case RF_ZEBRA:
		case RF_ZEBRA2:
			write_nic_byte(dev, 0x7f, ((0x01009b90 & 0xff000000) >> 24)); 
			write_nic_byte(dev, 0x7e, ((0x01009b90 & 0x00ff0000) >> 16)); 
			write_nic_byte(dev, 0x7d, ((0x01009b90 & 0x0000ff00) >> 8)); 
			write_nic_byte(dev, 0x7c, ((0x01009b90 & 0x000000ff) >> 0)); 

			write_nic_byte(dev, 0x7f, ((0x000090a6 & 0xff000000) >> 24)); 
			write_nic_byte(dev, 0x7e, ((0x000090a6 & 0x00ff0000) >> 16)); 
			write_nic_byte(dev, 0x7d, ((0x000090a6 & 0x0000ff00) >> 8)); 
			write_nic_byte(dev, 0x7c, ((0x000090a6 & 0x000000ff) >> 0)); 
			write_nic_byte(dev, ANTSEL, 0x03); 
                        bAntennaSwitched = true;
			break;

		default:
			printk("SetAntenna8185: unkown RFChipID(%d)\n", priv->rf_chip);
			break;
		}
		break;

	case 1:
		switch(priv->rf_chip)
		{
		case RF_ZEBRA:
		case RF_ZEBRA2:
			write_nic_byte(dev, 0x7f, ((0x0100db90 & 0xff000000) >> 24)); 
			write_nic_byte(dev, 0x7e, ((0x0100db90 & 0x00ff0000) >> 16)); 
			write_nic_byte(dev, 0x7d, ((0x0100db90 & 0x0000ff00) >> 8)); 
			write_nic_byte(dev, 0x7c, ((0x0100db90 & 0x000000ff) >> 0)); 

			write_nic_byte(dev, 0x7f, ((0x000010a6 & 0xff000000) >> 24)); 
			write_nic_byte(dev, 0x7e, ((0x000010a6 & 0x00ff0000) >> 16)); 
			write_nic_byte(dev, 0x7d, ((0x000010a6 & 0x0000ff00) >> 8)); 
			write_nic_byte(dev, 0x7c, ((0x000010a6 & 0x000000ff) >> 0)); 
			write_nic_byte(dev, ANTSEL, 0x00); 
			bAntennaSwitched = true;
			break;

		default:
			printk("SetAntenna8185: unkown RFChipID(%d)\n", priv->rf_chip);
			break;
		}
		break;

	default:
		printk("SetAntenna8185: unkown u1bAntennaIndex(%d)\n", u1bAntennaIndex);
		break;
	}

	if(bAntennaSwitched)
	{
		priv->CurrAntennaIndex = u1bAntennaIndex;
	}


	return bAntennaSwitched;
}

bool
SwitchAntenna(
	struct net_device *dev
	)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

	bool		bResult = false;

	if(priv->CurrAntennaIndex == 0)
	{
            bResult = SetAntenna8185(dev, 1);
            if(priv->ieee80211->state == IEEE80211_LINKED)
                printk("To aux antenna 1......\n");
	}
	else
	{
            bResult = SetAntenna8185(dev, 0);
            if(priv->ieee80211->state == IEEE80211_LINKED)
                printk("To main antenna 0......\n");
	}

	return bResult;
}

void
SwAntennaDiversity(
	struct net_device *dev
	)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
	bool   bSwCheckSS=true;

	if(bSwCheckSS)
	{
		priv->AdTickCount++;
	
	}
	
	if(priv->ieee80211->state != IEEE80211_LINKED)
	{

		priv->bAdSwitchedChecking = false;
		SwitchAntenna(dev);
	}
	else if(priv->AdRxOkCnt == 0)
	{

		priv->bAdSwitchedChecking = false;
		SwitchAntenna(dev);
	}
	else if(priv->bAdSwitchedChecking == true)
	{

		priv->bAdSwitchedChecking = false;

		priv->AdRxSsThreshold = (priv->AdRxSignalStrength + priv->AdRxSsBeforeSwitched) / 2;

		priv->AdRxSsThreshold = (priv->AdRxSsThreshold > priv->AdMaxRxSsThreshold) ?	
					priv->AdMaxRxSsThreshold: priv->AdRxSsThreshold;
		if(priv->AdRxSignalStrength < priv->AdRxSsBeforeSwitched)
		{ 
                        
			priv->AdCheckPeriod *= 2;
			if(priv->AdCheckPeriod > priv->AdMaxCheckPeriod)
				priv->AdCheckPeriod = priv->AdMaxCheckPeriod;
	
			SwitchAntenna(dev);
		}
		else
		{ 

			priv->AdCheckPeriod = priv->AdMinCheckPeriod;
		}

	}
	else
	{

		priv->AdTickCount = 0;

		
                
		if((priv->AdMainAntennaRxOkCnt < priv->AdAuxAntennaRxOkCnt) && (priv->CurrAntennaIndex == 0))
		{ 

			
			SwitchAntenna(dev);	
			priv->bHWAdSwitched = true;
		}
		else if((priv->AdAuxAntennaRxOkCnt < priv->AdMainAntennaRxOkCnt) && (priv->CurrAntennaIndex == 1))
		{ 

			
			SwitchAntenna(dev);
			priv->bHWAdSwitched = true;
		}
		else
		{


			priv->bHWAdSwitched = false;	
		}
		if( (!priv->bHWAdSwitched) && (bSwCheckSS))
		{

                if(priv->AdRxSignalStrength < priv->AdRxSsThreshold)
                { 

                    priv->AdRxSsBeforeSwitched = priv->AdRxSignalStrength; 
                    priv->bAdSwitchedChecking = true;

                    SwitchAntenna(dev);
                }
                else
                { 

			priv->bAdSwitchedChecking = false;
			if(	(priv->AdRxSignalStrength > (priv->AdRxSsThreshold + 10)) && 
				priv->AdRxSsThreshold <= priv->AdMaxRxSsThreshold) 
			{
				priv->AdRxSsThreshold = (priv->AdRxSsThreshold + priv->AdRxSignalStrength) / 2;
				priv->AdRxSsThreshold = (priv->AdRxSsThreshold > priv->AdMaxRxSsThreshold) ?
							priv->AdMaxRxSsThreshold: priv->AdRxSsThreshold;
			}

			if( priv->AdCheckPeriod > priv->AdMinCheckPeriod )
			{
				priv->AdCheckPeriod /= 2; 
			}
		}
		}
	}
	priv->AdRxOkCnt = 0;
	priv->AdMainAntennaRxOkCnt = 0;
	priv->AdAuxAntennaRxOkCnt = 0;


}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void SwAntennaWorkItemCallback(struct work_struct *work)
{
	struct delayed_work *dwork = container_of(work,struct delayed_work,work);
        struct ieee80211_device *ieee = container_of(dwork,struct ieee80211_device,SwAntennaWorkItem);
	struct net_device *dev = ieee->dev;
#else
void SwAntennaWorkItemCallback(struct net_device *dev)
{
#endif
	SwAntennaDiversity(dev);
}

void
SwAntennaDiversityTimerCallback(
	struct net_device *dev
	)
{
	struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
	RT_RF_POWER_STATE rtState;
	

	rtState = priv->eRFPowerState;
	do{
		if (rtState == eRfOff)
		{	
			break;
		}  	
		else if (rtState == eRfSleep)
		{	
			break;
		}  
		
#ifdef SW_ANTE_DIVERSITY
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20) 
		queue_delayed_work(priv->ieee80211->wq,&priv->ieee80211->SwAntennaWorkItem,0);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
		queue_work(priv->ieee80211->wq,(void *)&priv->ieee80211->SwAntennaWorkItem);
#else
		schedule_task(&priv->ieee80211->SwAntennaWorkItem);
#endif	
#endif

	}while(false);

	if(priv->up)
	{
		mod_timer(&priv->SwAntennaDiversityTimer, jiffies + MSECS(ANTENNA_DIVERSITY_TIMER_PERIOD));
	}

}
#endif

#endif
