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

#include "../ieee80211/ieee80211.h"
#include "r8180_hw.h"
#include "r8187.h"
#include "r8180_93cx6.h" 
#include "r8187_led.h"



void
InitSwLeds(
	struct net_device *dev
	)
{

	struct r8180_priv *priv = ieee80211_priv(dev);
	u16	usValue;

	priv->CustomerID = RT_CID_DEFAULT; 
	priv->bEnableLedCtrl = 1;
	priv->PsrValue = read_nic_byte(dev, PSR);
	usValue = eprom_read(dev, EEPROM_SW_REVD_OFFSET >> 1);
	priv->EEPROMCustomerID = (u8)( usValue & EEPROM_CID_MASK );
	DMESG("EEPROM Customer ID: %02X\n", priv->EEPROMCustomerID);

	if(priv->CustomerID == RT_CID_DEFAULT)
	{ 
		switch(priv->EEPROMCustomerID)
		{
		case EEPROM_CID_RSVD0:
		case EEPROM_CID_RSVD1:
			priv->CustomerID = RT_CID_DEFAULT;
			break;
	
		case EEPROM_CID_ALPHA0:
			priv->CustomerID = RT_CID_8187_ALPHA0;
			break;
	
		case EEPROM_CID_SERCOMM_PS:
			priv->CustomerID = RT_CID_8187_SERCOMM_PS;
			break;
	
		case EEPROM_CID_HW_LED:
			priv->CustomerID = RT_CID_8187_HW_LED;
			break;
	
		default:
			priv->CustomerID = RT_CID_DEFAULT;
			break;
		}
	}
	switch(priv->CustomerID)
	{
	case RT_CID_DEFAULT: 
		priv->LedStrategy = SW_LED_MODE0;
		break;
	
	case RT_CID_8187_ALPHA0:
		priv->LedStrategy = SW_LED_MODE1;
		break;	

	case RT_CID_8187_SERCOMM_PS:
		priv->LedStrategy = SW_LED_MODE3;
		break;

	case RT_CID_8187_HW_LED:
		priv->LedStrategy = HW_LED;
		break;

	default:
		priv->LedStrategy = SW_LED_MODE0;
		break;
	}
	
	InitLed8187(dev, 
				&(priv->Gpio0Led), 
				LED_PIN_GPIO0, 
				Gpio0LedBlinkTimerCallback);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	INIT_WORK(&priv->Gpio0LedWorkItem, (void*)Gpio0LedWorkItemCallback);

	InitLed8187(dev,	&(priv->SwLed0), 	LED_PIN_LED0, SwLed0BlinkTimerCallback);
	INIT_WORK(&priv->SwLed0WorkItem, 	(void*)SwLed0WorkItemCallback);

	InitLed8187(dev,	&(priv->SwLed1), 	LED_PIN_LED1, SwLed1BlinkTimerCallback);
	INIT_WORK(&priv->SwLed1WorkItem, 	(void*)SwLed1WorkItemCallback);

#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	INIT_WORK(&priv->Gpio0LedWorkItem,(void*)Gpio0LedWorkItemCallback, dev);

	InitLed8187(dev,	&(priv->SwLed0), 	LED_PIN_LED0, SwLed0BlinkTimerCallback);
	INIT_WORK(&priv->SwLed0WorkItem, 	(void*)SwLed0WorkItemCallback, dev);

	InitLed8187(dev,	&(priv->SwLed1), 	LED_PIN_LED1, SwLed1BlinkTimerCallback);
	INIT_WORK(&priv->SwLed1WorkItem, 	(void*)SwLed1WorkItemCallback, dev);
#else
	tq_init(&priv->Gpio0LedWorkItem,(void*)Gpio0LedWorkItemCallback, dev);

	InitLed8187(dev,	&(priv->SwLed0), 	LED_PIN_LED0, SwLed0BlinkTimerCallback);
	tq_init(&priv->SwLed0WorkItem, 	(void*)SwLed0WorkItemCallback, dev);

	InitLed8187(dev,	&(priv->SwLed1), 	LED_PIN_LED1, SwLed1BlinkTimerCallback);
	tq_init(&priv->SwLed1WorkItem, 	(void*)SwLed1WorkItemCallback, dev);
#endif
}

void
DeInitSwLeds(
	struct net_device *dev
	)
{
	struct r8180_priv *priv = ieee80211_priv(dev);

	DeInitLed8187(dev, &(priv->Gpio0Led));
	DeInitLed8187(dev, &(priv->SwLed0));
	DeInitLed8187(dev, &(priv->SwLed1));
}

void
InitLed8187(
	struct net_device *dev, 
	PLED_8187		pLed,
	LED_PIN_8187		LedPin,
	void	* 		BlinkCallBackFunc)
{
	pLed->LedPin = LedPin;

	pLed->bLedOn = 0;
	pLed->CurrLedState = LED_OFF;

	pLed->bLedBlinkInProgress = 0;
	pLed->BlinkTimes = 0;
	pLed->BlinkingLedState = LED_OFF;

	init_timer(&(pLed->BlinkTimer));
	pLed->BlinkTimer.data = (unsigned long)dev;
	pLed->BlinkTimer.function = BlinkCallBackFunc;
}

void
DeInitLed8187(
	struct net_device *dev, 
	PLED_8187			pLed)
{
	del_timer_sync(&(pLed->BlinkTimer));
	pLed->bLedBlinkInProgress = 0;
}

void
LedControl8187(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	if( priv->bEnableLedCtrl == 0)
		return;


	switch(priv->LedStrategy)
	{
	case SW_LED_MODE0:
		SwLedControlMode0(dev, LedAction);
		break;

	case SW_LED_MODE1:
		SwLedControlMode1(dev, LedAction);
		break;

	case SW_LED_MODE2:
		SwLedControlMode2(dev, LedAction);
		break;

	case SW_LED_MODE3:
		SwLedControlMode3(dev, LedAction);
		break;

	default:
		break;
	}
}


void
SwLedControlMode0(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	PLED_8187 pLed = &(priv->Gpio0Led);

	switch(LedAction)
	{
	case LED_CTL_TX:
	case LED_CTL_RX:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->CurrLedState = LED_BLINK_NORMAL;
			pLed->BlinkTimes = 2;
		}
		else
		{
			return;
		}
		break;

	case LED_CTL_SITE_SURVEY:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->CurrLedState = LED_BLINK_SLOWLY;
		}
		else
		{
			return;
		}
		break;

	case LED_CTL_LINK:
		pLed->CurrLedState = LED_ON;
		break;

	case LED_CTL_NO_LINK:
		pLed->CurrLedState = LED_OFF;
		break;
	
	case LED_CTL_POWER_ON:
		pLed->CurrLedState = LED_POWER_ON_BLINK;
		break;

	case LED_CTL_POWER_OFF:
		pLed->CurrLedState = LED_OFF;
		break;

	default:
		return;
		break;
	}

	switch(pLed->CurrLedState)
	{
	case LED_ON:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			SwLedOn(dev, pLed);
		}
		break;

	case LED_OFF:
	
	if(pLed->bLedBlinkInProgress )
		{
			del_timer_sync(&(pLed->BlinkTimer));
			pLed->bLedBlinkInProgress = false;
		}
		SwLedOff(dev, pLed);
		break;

	case LED_BLINK_NORMAL:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->bLedBlinkInProgress = 1;
			if( pLed->bLedOn )
				pLed->BlinkingLedState = LED_OFF; 
			else
				pLed->BlinkingLedState = LED_ON; 
			
			mod_timer(&(pLed->BlinkTimer), jiffies + LED_BLINK_NORMAL_INTERVAL);
		}
		break;

	case LED_BLINK_SLOWLY:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->bLedBlinkInProgress = 1;
				pLed->BlinkingLedState = LED_OFF; 
			
			mod_timer(&(pLed->BlinkTimer), jiffies + LED_BLINK_SLOWLY_INTERVAL);
		}
		break;

	case LED_POWER_ON_BLINK:
		SwLedOn(dev, pLed);
#ifdef LED_SHIN
		mdelay(50);
		SwLedOff(dev, pLed);
#endif
		break;

	default:
		break;
	}
}

void
SwLedControlMode1(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	PLED_8187 pLed0 = &(priv->SwLed0);
	PLED_8187 pLed1 = &(priv->SwLed1);

	switch(LedAction)
	{
	case LED_CTL_TX:
		if( pLed0->bLedBlinkInProgress == 0 )
		{
			pLed0->CurrLedState = LED_BLINK_NORMAL;
			pLed0->BlinkTimes = 2;
			pLed0->bLedBlinkInProgress = 1;
			if( pLed0->bLedOn )
				pLed0->BlinkingLedState = LED_OFF; 
			else
				pLed0->BlinkingLedState = LED_ON; 

			mod_timer(&(pLed0->BlinkTimer),jiffies + LED_BLINK_NORMAL_INTERVAL);
		}
		break;

	case LED_CTL_LINK:
		pLed0->CurrLedState = LED_ON;
		if( pLed0->bLedBlinkInProgress == 0 )
		{
			SwLedOn(dev, pLed0);
		}
		break;

	case LED_CTL_NO_LINK:
		pLed0->CurrLedState = LED_OFF;
		if( pLed0->bLedBlinkInProgress == 0 )
		{
			SwLedOff(dev, pLed0);
		}
		break;
	
	case LED_CTL_POWER_ON:
		pLed0->CurrLedState = LED_OFF;
		SwLedOff(dev, pLed0);

		pLed1->CurrLedState = LED_ON;
		SwLedOn(dev, pLed1);

		break;

	case LED_CTL_POWER_OFF:
		pLed0->CurrLedState = LED_OFF;
		SwLedOff(dev, pLed0);

		pLed1->CurrLedState = LED_OFF;
		SwLedOff(dev, pLed1);
		break;

	case LED_CTL_SITE_SURVEY:
		if( pLed0->bLedBlinkInProgress == 0 )
		{
			pLed0->CurrLedState = LED_BLINK_SLOWLY;;
			pLed0->BlinkTimes = 10;
			pLed0->bLedBlinkInProgress = 1;
			if( pLed0->bLedOn )
				pLed0->BlinkingLedState = LED_OFF; 
			else
				pLed0->BlinkingLedState = LED_ON;

			mod_timer(&(pLed0->BlinkTimer),jiffies + LED_BLINK_SLOWLY_INTERVAL);
		}
		break;

	default:
		break;
	}
}

void
SwLedControlMode2(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	PLED_8187 pLed = &(priv->Gpio0Led);

	switch(LedAction)
	{
	case LED_CTL_TX:
	case LED_CTL_RX:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->bLedBlinkInProgress = 1;

			pLed->CurrLedState = LED_BLINK_NORMAL;
			pLed->BlinkTimes = 2;

			if( pLed->bLedOn )
				pLed->BlinkingLedState = LED_OFF; 
			else
				pLed->BlinkingLedState = LED_ON; 

			mod_timer(&(pLed->BlinkTimer),jiffies + LED_BLINK_NORMAL_INTERVAL);
		}
		break;

	case LED_CTL_SITE_SURVEY:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->bLedBlinkInProgress = 1;

				pLed->CurrLedState = LED_SCAN_BLINK;
				pLed->BlinkTimes = 4;

			if( pLed->bLedOn )
			{
				pLed->BlinkingLedState = LED_OFF;
				mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM2_BLINK_ON_INTERVAL);
			}
			else
			{
				pLed->BlinkingLedState = LED_ON; 
				mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM2_BLINK_OFF_INTERVAL);
			}
		}
		else
		{
			if(pLed->CurrLedState != LED_NO_LINK_BLINK)
			{
				pLed->CurrLedState = LED_SCAN_BLINK;
			}
		}
		break;

	case LED_CTL_NO_LINK:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->bLedBlinkInProgress = 1;

			pLed->CurrLedState = LED_NO_LINK_BLINK;
			pLed->BlinkTimes = 24;

			if( pLed->bLedOn )
			{
				pLed->BlinkingLedState = LED_OFF; 
				mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM2_BLINK_ON_INTERVAL);
			}
			else
			{
				pLed->BlinkingLedState = LED_ON; 
				mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM2_BLINK_OFF_INTERVAL);
			}
		}
		else
		{
			pLed->CurrLedState = LED_NO_LINK_BLINK;
		}
		break;

	case LED_CTL_LINK:
		pLed->CurrLedState = LED_ON;
		if( pLed->bLedBlinkInProgress == 0 )
		{
			SwLedOn(dev, pLed);
		}
		break;

	case LED_CTL_POWER_OFF:
		pLed->CurrLedState = LED_OFF;
		if( pLed->bLedBlinkInProgress == 0 )
		{
			SwLedOff(dev, pLed);
		}
		break;

	default:
		break;
	}
}


void
SwLedControlMode3(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	PLED_8187 pLed = &(priv->Gpio0Led);

	switch(LedAction)
	{
	case LED_CTL_TX:
	case LED_CTL_RX:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->bLedBlinkInProgress = 1;

			pLed->CurrLedState = LED_BLINK_CM3;
			pLed->BlinkTimes = 2;

			if( pLed->bLedOn )
				pLed->BlinkingLedState = LED_OFF; 
			else
				pLed->BlinkingLedState = LED_ON; 

			mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM3_BLINK_INTERVAL);
		}
		break;

	case LED_CTL_SITE_SURVEY:
		if( pLed->bLedBlinkInProgress == 0 )
		{
			pLed->bLedBlinkInProgress = 1;

			pLed->CurrLedState = LED_BLINK_CM3;
			pLed->BlinkTimes = 10;

			if( pLed->bLedOn )
				pLed->BlinkingLedState = LED_OFF; 
			else
				pLed->BlinkingLedState = LED_ON; 

			mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM3_BLINK_INTERVAL);
		}
		break;

	case LED_CTL_LINK:
		pLed->CurrLedState = LED_ON;
		if( pLed->bLedBlinkInProgress == 0 )
		{
			SwLedOn(dev, pLed);
		}
		break;

	case LED_CTL_NO_LINK:
		pLed->CurrLedState = LED_OFF;
		if( pLed->bLedBlinkInProgress == 0 )
		{
			SwLedOff(dev, pLed);
		}
		break;

	case LED_CTL_POWER_ON:
		pLed->CurrLedState = LED_POWER_ON_BLINK;
		SwLedOn(dev, pLed);
		mdelay(100);
		SwLedOff(dev, pLed);
		break;

	case LED_CTL_POWER_OFF:
		pLed->CurrLedState = LED_OFF;
		if( pLed->bLedBlinkInProgress == 0 )
		{
			SwLedOff(dev, pLed);
		}
		break;

	default:
		break;
	}
}

void
Gpio0LedBlinkTimerCallback(
	unsigned long		data
	)
{
	struct net_device *dev = (struct net_device *)data;
	struct r8180_priv *priv = ieee80211_priv(dev);

	PlatformSwLedBlink(dev, &(priv->Gpio0Led));
}



void
SwLed0BlinkTimerCallback(
	unsigned long		data
	)
{
	struct net_device *dev = (struct net_device *)data;
	struct r8180_priv *priv = ieee80211_priv(dev);

	PlatformSwLedBlink(dev, &(priv->SwLed0));
}



void
SwLed1BlinkTimerCallback(
	unsigned long		data
	)
{
	struct net_device *dev = (struct net_device *)data;
	struct r8180_priv *priv = ieee80211_priv(dev);

	PlatformSwLedBlink(dev, &(priv->SwLed1));
}

void
PlatformSwLedBlink(
	struct net_device *dev,
	PLED_8187		pLed
	)
{
	struct r8180_priv *priv = ieee80211_priv(dev);

	switch(pLed->LedPin)
	{
	case LED_PIN_GPIO0:
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
		schedule_work(&(priv->Gpio0LedWorkItem));
#else
		schedule_task(&(priv->Gpio0LedWorkItem));
#endif		
		break;

	case LED_PIN_LED0:
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
		schedule_work(&(priv->SwLed0WorkItem));
#else
		schedule_task(&(priv->SwLed0WorkItem));
#endif	
		break;

	case LED_PIN_LED1:
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
		schedule_work(&(priv->SwLed1WorkItem));
#else
		schedule_task(&(priv->SwLed1WorkItem));
#endif	
		break;

	default:
		break;
	}
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
void Gpio0LedWorkItemCallback(struct work_struct *work)
{
	struct r8180_priv *priv = container_of(work, struct r8180_priv,Gpio0LedWorkItem);
	struct net_device *dev = priv->ieee80211->dev;
#else
void
Gpio0LedWorkItemCallback(struct net_device *dev)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
#endif
	PLED_8187	pLed = &(priv->Gpio0Led); 
	if (priv == NULL || dev == NULL){
	}
	
#if 0 
	if(priv->LedStrategy == SW_LED_MODE2)
		SwLedCm2Blink(dev, pLed);
	else
		SwLedBlink(dev, pLed);
#endif

#if 1 
	switch(priv->LedStrategy)
	{
	case SW_LED_MODE2:
		SwLedCm2Blink(dev, pLed);
		break;
	default:
		SwLedBlink(dev, pLed);
		break;
	}
#endif

}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
void SwLed0WorkItemCallback(struct work_struct *work)
{
#else
void  SwLed0WorkItemCallback(struct net_device *dev)
{
#endif

}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
void SwLed1WorkItemCallback(struct work_struct *work)
{
#else
void
SwLed1WorkItemCallback(struct net_device *dev)
{
#endif

}

void
SwLedBlink(
	struct net_device *dev, 
	PLED_8187			pLed
	)
{
	u8 bStopBlinking = 0;

	if( pLed->BlinkingLedState == LED_ON ) 
	{
		SwLedOn(dev, pLed);
	}	
	else 
	{
		SwLedOff(dev, pLed);
	}

	if(pLed->CurrLedState == LED_BLINK_SLOWLY)
	{
		bStopBlinking = 0;
	} else {
	pLed->BlinkTimes--;
	if(	pLed->BlinkTimes == 0 )
	{
		bStopBlinking = 1;
	}
	else
	{
		if(	pLed->CurrLedState != LED_BLINK_NORMAL &&
			pLed->CurrLedState != LED_BLINK_SLOWLY &&
			pLed->CurrLedState != LED_BLINK_CM3	)
		{
			bStopBlinking = 1;
		}
	}
	}

	if(bStopBlinking)
	{
		if(	pLed->CurrLedState == LED_ON && pLed->bLedOn == 0)
		{
			SwLedOn(dev, pLed);
		}
		else if(pLed->CurrLedState == LED_OFF && pLed->bLedOn == 1)
		{
			SwLedOff(dev, pLed);
		}

		pLed->BlinkTimes = 0;
		pLed->bLedBlinkInProgress = 0;	
	}
	else
	{
		if( pLed->BlinkingLedState == LED_ON ) 
			pLed->BlinkingLedState = LED_OFF;
		else 
			pLed->BlinkingLedState = LED_ON;

		switch( pLed->CurrLedState )
		{
		case LED_BLINK_NORMAL:
			mod_timer(&(pLed->BlinkTimer),jiffies + LED_BLINK_NORMAL_INTERVAL);
			break;

		case LED_BLINK_SLOWLY:
			if( pLed->bLedOn == 1 ) 
			{
			mod_timer(&(pLed->BlinkTimer),jiffies + LED_BLINK_SLOWLY_INTERVAL+50);
			pLed->BlinkingLedState = LED_OFF;			
			} else {
			mod_timer(&(pLed->BlinkTimer),jiffies + 5000);
			pLed->BlinkingLedState = LED_ON;			
			}	
			break;

		case LED_BLINK_CM3:
			mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM3_BLINK_INTERVAL);
			break;

		default:
			mod_timer(&(pLed->BlinkTimer),jiffies + LED_BLINK_SLOWLY_INTERVAL);
			break;
		}
	}
}



void
SwLedCm2Blink(
	struct net_device *dev, 
	PLED_8187			pLed
	)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	u8 bStopBlinking = 0;

	if((priv->eRFPowerState == eRfOff) && (priv->RfOffReason>RF_CHANGE_BY_IPS))
	{
		SwLedOff(dev, pLed);

		mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM2_BLINK_ON_INTERVAL);
		return;
	}
	
	if( pLed->BlinkingLedState == LED_ON ) 
	{
		SwLedOn(dev, pLed);
	}	
	else 
	{
		SwLedOff(dev, pLed);
	}

	if(pLed->BlinkTimes > 0)
	{
	pLed->BlinkTimes--;
	}
		
	switch(pLed->CurrLedState)
	{
	case LED_BLINK_NORMAL: 
		if(pLed->BlinkTimes == 0)
		{
			bStopBlinking = 1;
		}
		break;
	default:
		bStopBlinking = 1;
		break;
	}

	if(bStopBlinking)
	{
		pLed->BlinkTimes = 0;
		pLed->bLedBlinkInProgress = 0;	
	}
	else
	{
		if( pLed->BlinkingLedState == LED_ON ) 
			pLed->BlinkingLedState = LED_OFF;
		else 
			pLed->BlinkingLedState = LED_ON;

		switch( pLed->CurrLedState )
		{
		case LED_BLINK_NORMAL:
			mod_timer(&(pLed->BlinkTimer),jiffies + LED_BLINK_NORMAL_INTERVAL);
			break;

		case LED_BLINK_SLOWLY:
			mod_timer(&(pLed->BlinkTimer),jiffies + LED_BLINK_SLOWLY_INTERVAL);
			break;

		case LED_SCAN_BLINK:
		case LED_NO_LINK_BLINK:
			if( pLed->bLedOn ) {
				mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM2_BLINK_ON_INTERVAL);
			} else {
				mod_timer(&(pLed->BlinkTimer),jiffies + LED_CM2_BLINK_OFF_INTERVAL);
			}
			break;

		default:
			mod_timer(&(pLed->BlinkTimer),jiffies + LED_BLINK_SLOWLY_INTERVAL);
			break;
		}
	}
}

void
SwLedOn(
	struct net_device *dev, 
	PLED_8187			pLed
)
{
	struct r8180_priv *priv = ieee80211_priv(dev);

	switch(pLed->LedPin)
	{
	case LED_PIN_GPIO0:
		write_nic_byte(dev,0xFF91,0x01);
		write_nic_byte(dev,0xFF90,0x00);	
		break;

	case LED_PIN_LED0:
		priv->PsrValue &= ~(0x01 << 4);
		write_nic_byte(dev, PSR, priv->PsrValue);
		break;

	case LED_PIN_LED1:
		priv->PsrValue &= ~(0x01 << 5);
		write_nic_byte(dev, PSR, priv->PsrValue);
		break;

	default:
		break;
	}

	pLed->bLedOn = 1;
}

void
SwLedOff(
	struct net_device *dev, 
	PLED_8187			pLed
)
{
	struct r8180_priv *priv = ieee80211_priv(dev);

	switch(pLed->LedPin)
	{
	case LED_PIN_GPIO0:
		write_nic_byte(dev,0xFF91,0x01);
		write_nic_byte(dev,0xFF90,0x01);	
		break;

	case LED_PIN_LED0:
		priv->PsrValue |= (0x01 << 4);
		write_nic_byte(dev, PSR, priv->PsrValue);
		break;

	case LED_PIN_LED1:
		priv->PsrValue |= (0x01 << 5);
		write_nic_byte(dev, PSR, priv->PsrValue);
		break;

	default:
		break;
	}

	pLed->bLedOn = 0;
}	

