/******************************************************************************
 * Copyright(c) 2008 - 2010 Realtek Corporation. All rights reserved.
 *
 * Based on the r8180 driver, which is:
 * Copyright 2004-2005 Andrea Merello <andreamrl@tiscali.it>, et al.
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
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



#include "r8180_hw.h"
#include "r8180_rtl8225.h"

#define USE_8051_3WIRE 1

u8 rtl8225_threshold[]={
	0x8d, 0x8d, 0x8d, 0x8d, 0x9d, 0xad, 0xbd,
};

u8 rtl8225_gain[]={
	0x23,0x88,0x7c,0xa5,
	0x23,0x88,0x7c,0xb5,
	0x23,0x88,0x7c,0xc5,
	0x33,0x80,0x79,0xc5,
	0x43,0x78,0x76,0xc5,
	0x53,0x60,0x73,0xc5,
	0x63,0x58,0x70,0xc5,
};

#if 0
u8 rtl8225_init_gain[]={
	0x33,0x80,0x6c,0xc5,
	0x43,0x78,0x69,0xc5,
	0x53,0x60,0x66,0xc5,
	0x63,0x58,0x63,0xc5,
	0x73,0x50,0x62,0xc5,
	0x83,0x43,0x61,0xc5,
	0x93,0x38,0x5a,0xc5,
};
#endif





u16 rtl8225bcd_rxgain[]={	
	0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0408, 0x0409,
	0x040a, 0x040b, 0x0502, 0x0503, 0x0504, 0x0505, 0x0540, 0x0541,  
	0x0542, 0x0543, 0x0544, 0x0545, 0x0580, 0x0581, 0x0582, 0x0583,
	0x0584, 0x0585, 0x0588, 0x0589, 0x058a, 0x058b, 0x0643, 0x0644, 
	0x0645, 0x0680, 0x0681, 0x0682, 0x0683, 0x0684, 0x0685, 0x0688,
	0x0689, 0x068a, 0x068b, 0x068c, 0x0742, 0x0743, 0x0744, 0x0745,
	0x0780, 0x0781, 0x0782, 0x0783, 0x0784, 0x0785, 0x0788, 0x0789,
	0x078a, 0x078b, 0x078c, 0x078d, 0x0790, 0x0791, 0x0792, 0x0793,
	0x0794, 0x0795, 0x0798, 0x0799, 0x079a, 0x079b, 0x079c, 0x079d,  
	0x07a0, 0x07a1, 0x07a2, 0x07a3, 0x07a4, 0x07a5, 0x07a8, 0x07a9,  
	0x07aa, 0x07ab, 0x07ac, 0x07ad, 0x07b0, 0x07b1, 0x07b2, 0x07b3,  
	0x07b4, 0x07b5, 0x07b8, 0x07b9, 0x07ba, 0x07bb, 0x07bb

};


#if 0
u16 rtl8225bc_rxgain[]={	
	0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0408, 0x0409,
	0x040a, 0x040b, 0x0502, 0x0503, 0x0504, 0x0505, 0x0540, 0x0541,  
	0x0542, 0x0543, 0x0544, 0x0545, 0x0580, 0x0581, 0x0582, 0x0583,
	0x0584, 0x0585, 0x0588, 0x0589, 0x058a, 0x058b, 0x0643, 0x0644, 
	0x0645, 0x0680, 0x0681, 0x0682, 0x0683, 0x0684, 0x0685, 0x0688,
	0x0689, 0x068a, 0x068b, 0x068c, 0x0742, 0x0743, 0x0744, 0x0745,
	0x0780, 0x0781, 0x0782, 0x0783, 0x0784, 0x0785, 0x0788, 0x0789,
	0x078a, 0x078b, 0x078c, 0x078d, 0x0790, 0x0791, 0x0792, 0x0793,
	0x0794, 0x0795, 0x0798, 0x0799, 0x039a, 0x039b, 0x039c, 0x039d,  
	0x03a0, 0x03a1, 0x03a2, 0x03a3, 0x03a4, 0x03a5, 0x03a8, 0x03a9,  
	0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03b0, 0x03b1, 0x03b2, 0x03b3,  
	0x03b4, 0x03b5, 0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bb

};


u16 rtl8225a_rxgain[]={	
	0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0408, 0x0409,
	0x040a, 0x040b, 0x0502, 0x0503, 0x0504, 0x0505, 0x0540, 0x0541,  
	0x0542, 0x0543, 0x0544, 0x0545, 0x0580, 0x0581, 0x0582, 0x0583,
	0x0584, 0x0585, 0x0588, 0x0589, 0x058a, 0x058b, 0x0643, 0x0644, 
	0x0645, 0x0680, 0x0681, 0x0682, 0x0683, 0x0684, 0x0685, 0x0688,
	0x0689, 0x068a, 0x068b, 0x068c, 0x0742, 0x0743, 0x0744, 0x0745,
	0x0780, 0x0781, 0x0782, 0x0783, 0x0784, 0x0785, 0x0788, 0x0789,
	0x078a, 0x078b, 0x078c, 0x078d, 0x0790, 0x0791, 0x0792, 0x0793,
	0x0794, 0x0795, 0x0798, 0x0799, 0x079a, 0x079b, 0x079c, 0x079d,  
	0x07a0, 0x07a1, 0x07a2, 0x07a3, 0x07a4, 0x07a5, 0x07a8, 0x07a9,  
	0x07aa, 0x07ab, 0x07ac, 0x07ad, 0x07ad, 0x07ad, 0x07ad, 0x07ad,  
	0x07ad, 0x07ad, 0x07ad, 0x07ad, 0x07ad, 0x07ad, 0x07ad
};
#endif



u8 rtl8225_tx_gain_cck_ofdm[]={
	0x02,0x06,0x0e,0x1e,0x3e,0x7e
};


u8 rtl8225_tx_power_ofdm[]={
	0x80,0x90,0xa2,0xb5,0xcb,0xe4
};


u8 rtl8225_tx_power_cck_ch14[]={
	0x18,0x17,0x15,0x0c,0x00,0x00,0x00,0x00,
	0x1b,0x1a,0x17,0x0e,0x00,0x00,0x00,0x00,
	0x1f,0x1e,0x1a,0x0f,0x00,0x00,0x00,0x00,
	0x22,0x21,0x1d,0x11,0x00,0x00,0x00,0x00,
	0x26,0x25,0x21,0x13,0x00,0x00,0x00,0x00,
	0x2b,0x2a,0x25,0x15,0x00,0x00,0x00,0x00
};


u8 rtl8225_tx_power_cck[]={
	0x18,0x17,0x15,0x11,0x0c,0x08,0x04,0x02,
	0x1b,0x1a,0x17,0x13,0x0e,0x09,0x04,0x02,
	0x1f,0x1e,0x1a,0x15,0x10,0x0a,0x05,0x02,
	0x22,0x21,0x1d,0x18,0x11,0x0b,0x06,0x02,
	0x26,0x25,0x21,0x1b,0x14,0x0d,0x06,0x03,
	0x2b,0x2a,0x25,0x1e,0x16,0x0e,0x07,0x03
};

u8 rtl8225_agc[]={
	0x9e,0x9e,0x9e,0x9e,0x9e,0x9e,0x9e,0x9e,0x9d,0x9c,0x9b,0x9a,0x99,0x98,0x97,0x96,
	0x95,0x94,0x93,0x92,0x91,0x90,0x8f,0x8e,0x8d,0x8c,0x8b,0x8a,0x89,0x88,0x87,0x86,
	0x85,0x84,0x83,0x82,0x81,0x80,0x3f,0x3e,0x3d,0x3c,0x3b,0x3a,0x39,0x38,0x37,0x36,
	0x35,0x34,0x33,0x32,0x31,0x30,0x2f,0x2e,0x2d,0x2c,0x2b,0x2a,0x29,0x28,0x27,0x26,
	0x25,0x24,0x23,0x22,0x21,0x20,0x1f,0x1e,0x1d,0x1c,0x1b,0x1a,0x19,0x18,0x17,0x16,
	0x15,0x14,0x13,0x12,0x11,0x10,0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,
	0x05,0x04,0x03,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
};

u32 rtl8225_chan[] = {
	0,	
	0x085c, 
	0x08dc, 
	0x095c, 
	0x09dc, 
	0x0a5c, 
	0x0adc, 
	0x0b5c, 
	0x0bdc, 
	0x0c5c, 
	0x0cdc, 
	0x0d5c, 
	0x0ddc, 
	0x0e5c, 
	0x0f72, 
};

void rtl8225_set_gain(struct net_device *dev, short gain)
{
	write_phy_ofdm(dev, 0x0d, rtl8225_gain[gain * 4]);
	write_phy_ofdm(dev, 0x1b, rtl8225_gain[gain * 4 + 2]);
	write_phy_ofdm(dev, 0x1d, rtl8225_gain[gain * 4 + 3]);
	write_phy_ofdm(dev, 0x23, rtl8225_gain[gain * 4 + 1]);

}

#if 0

void rtl8225_set_gain(struct net_device *dev, short gain)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	rtl8180_set_anaparam(dev, RTL8225_ANAPARAM_ON);
	
	if(priv->card_8185 == 2)
		write_phy_ofdm(dev, 0x21, 0x27);
	else
		write_phy_ofdm(dev, 0x21, 0x37);
		
	write_phy_ofdm(dev, 0x25, 0x20);
	write_phy_ofdm(dev, 0x11, 0x6);
	
	if(priv->card_8185 == 1 && priv->card_8185_Bversion)
		write_phy_ofdm(dev, 0x27, 0x8);
	else
		write_phy_ofdm(dev, 0x27, 0x88);
		
	write_phy_ofdm(dev, 0x14, 0);
	write_phy_ofdm(dev, 0x16, 0);
	write_phy_ofdm(dev, 0x15, 0x40);
	write_phy_ofdm(dev, 0x17, 0x40);
	
	write_phy_ofdm(dev, 0x0d, rtl8225_gain[gain * 4]);
	write_phy_ofdm(dev, 0x23, rtl8225_gain[gain * 4 + 1]);
	write_phy_ofdm(dev, 0x1b, rtl8225_gain[gain * 4 + 2]);
	write_phy_ofdm(dev, 0x1d, rtl8225_gain[gain * 4 + 3]);
}
#endif


void write_rtl8225(struct net_device *dev, u8 adr, u16 data)
{
#ifdef USE_8051_3WIRE

        struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
        struct usb_device *udev = priv->udev;
        u16                             wReg80, wReg82, wReg84;

        wReg80 = read_nic_word(dev, 0x80);
        wReg80 &= 0xfff3;
        wReg82 = read_nic_word(dev, 0x82);
        wReg84 = read_nic_word(dev, 0x84);
        wReg84 &= 0xfff0;

        write_nic_word(dev, 0x82, (wReg82|0x0007));     
        write_nic_word(dev, 0x84, (wReg84|0x0007));     
        force_pci_posting(dev);
        udelay(10); 

        write_nic_word(dev, 0x80, (BB_HOST_BANG_EN|wReg80)); 
        force_pci_posting(dev);
        udelay(2);
        write_nic_word(dev, 0x80, (wReg80)); 
        force_pci_posting(dev);
        udelay(10);

        usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
                               RTL8187_REQ_SET_REGS, RTL8187_REQT_WRITE,
                               adr, 0x8225, &data, 2, HZ / 2);

        write_nic_word(dev, 0x80, (BB_HOST_BANG_EN|wReg80));
        force_pci_posting(dev);
        udelay(10);

        write_nic_word(dev, 0x80, (wReg80|0x0004));
        write_nic_word(dev, 0x84, (wReg84|0x0000));

        if(priv->card_type == USB)
                mdelay(2);
        else
                rtl8185_rf_pins_enable(dev);

#else
	int i;
	u16 out,select;
	u8 bit;
	u32 bangdata = (data << 4) | (adr & 0xf);
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	out = read_nic_word(dev, RFPinsOutput) & 0xfff3;
		
	write_nic_word(dev,RFPinsEnable,
		(read_nic_word(dev,RFPinsEnable) | 0x7));
	
	select = read_nic_word(dev, RFPinsSelect);
	
	write_nic_word(dev, RFPinsSelect, select | 0x7 | 
		((priv->card_type == USB) ? 0 : SW_CONTROL_GPIO));
	
	force_pci_posting(dev);
	udelay(10);
	
	write_nic_word(dev, RFPinsOutput, out | BB_HOST_BANG_EN );
	
	force_pci_posting(dev);
	udelay(2);
	
	write_nic_word(dev, RFPinsOutput, out);
	
	force_pci_posting(dev);
	udelay(10);
	
	
	for(i=15; i>=0;i--){
	
		bit = (bangdata & (1<<i)) >> i;
		
		write_nic_word(dev, RFPinsOutput, bit | out);
		
		write_nic_word(dev, RFPinsOutput, bit | out | BB_HOST_BANG_CLK);
		write_nic_word(dev, RFPinsOutput, bit | out | BB_HOST_BANG_CLK);

		i--;
		bit = (bangdata & (1<<i)) >> i;
		
		write_nic_word(dev, RFPinsOutput, bit | out | BB_HOST_BANG_CLK);
		write_nic_word(dev, RFPinsOutput, bit | out | BB_HOST_BANG_CLK);

		write_nic_word(dev, RFPinsOutput, bit | out);

	}
	
	write_nic_word(dev, RFPinsOutput, out | BB_HOST_BANG_EN);
	
	force_pci_posting(dev);
	udelay(10);

	write_nic_word(dev, RFPinsOutput, out | 
		((priv->card_type == USB) ? 4 : BB_HOST_BANG_EN));

	write_nic_word(dev, RFPinsSelect, select | 
		((priv->card_type == USB) ? 0 : SW_CONTROL_GPIO));	

	if(priv->card_type == USB)
		mdelay(2);
	else
		rtl8185_rf_pins_enable(dev);
#endif
}

void rtl8225_rf_close(struct net_device *dev)
{
	write_rtl8225(dev, 0x4, 0x1f);   
	
	force_pci_posting(dev);
	mdelay(1);
	
	rtl8180_set_anaparam(dev, RTL8225_ANAPARAM_OFF);
	rtl8185_set_anaparam2(dev, RTL8225_ANAPARAM2_OFF);
}


short rtl8225_rf_set_sens(struct net_device *dev, short sens)
{
	if (sens <0 || sens > 6) return -1;
	
	if(sens > 4)
		write_rtl8225(dev, 0x0c, 0x850);
	else	
		write_rtl8225(dev, 0x0c, 0x50);

	sens= 6-sens;
	rtl8225_set_gain(dev, sens);
	
	write_phy_cck(dev, 0x41, rtl8225_threshold[sens]);
	return 0;
	
}

void rtl8225_SetTXPowerLevel(struct net_device *dev, short ch)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	int GainIdx;
	int GainSetting;
	int i;
	u8 power;
	u8 *cck_power_table;
	u8 max_cck_power_level;
	u8 max_ofdm_power_level;
	u8 min_ofdm_power_level;	
	u8 cck_power_level = 0xff & priv->chtxpwr[ch];
	u8 ofdm_power_level = 0xff & priv->chtxpwr_ofdm[ch];
	
	if(priv->card_type == USB){
		max_cck_power_level = 11;
		max_ofdm_power_level = 25; 
		min_ofdm_power_level = 10;
	}else{
		max_cck_power_level = 35;
		max_ofdm_power_level = 35;
		min_ofdm_power_level = 0;
	}
	if(cck_power_level > max_cck_power_level)
		cck_power_level = max_cck_power_level;
	GainIdx=cck_power_level % 6;
	GainSetting=cck_power_level / 6;
	
	if(ch == 14) 
		cck_power_table = rtl8225_tx_power_cck_ch14;
	else 
		cck_power_table = rtl8225_tx_power_cck;
	
	write_nic_byte(dev, TX_GAIN_CCK, rtl8225_tx_gain_cck_ofdm[GainSetting]>>1);
	
	for(i=0;i<8;i++){
	
		power = cck_power_table[GainIdx * 8 + i];
		write_phy_cck(dev, 0x44 + i, power);
	}
	
	force_pci_posting(dev);
	mdelay(1);
	
	if(ofdm_power_level > (max_ofdm_power_level - min_ofdm_power_level))
		ofdm_power_level = max_ofdm_power_level;
	else
		ofdm_power_level += min_ofdm_power_level;
	if(ofdm_power_level > 35)
		ofdm_power_level = 35;
	
	GainIdx=ofdm_power_level % 6;
	GainSetting=ofdm_power_level / 6;
#if 1 	
		rtl8185_set_anaparam2(dev,RTL8225_ANAPARAM2_ON);
	
		write_phy_ofdm(dev,2,0x42);
		write_phy_ofdm(dev,6,0);
		write_phy_ofdm(dev,8,0);	
#endif
	write_nic_byte(dev, TX_GAIN_OFDM, rtl8225_tx_gain_cck_ofdm[GainSetting]>>1);
				
			
	power = rtl8225_tx_power_ofdm[GainIdx];
	
	write_phy_ofdm(dev, 0x5, power);
	write_phy_ofdm(dev, 0x7, power);
	
	force_pci_posting(dev);
	mdelay(1);
}
#if 0
void rtl8225_set_mode(struct net_device *dev, short modeb)
{
	write_phy_ofdm(dev, 0x15, (modeb ? 0x0 : 0x40));
	write_phy_ofdm(dev, 0x17, (modeb ? 0x0 : 0x40));	
}
#endif

void rtl8225_rf_set_chan(struct net_device *dev, short ch)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	short gset = (priv->ieee80211->state == IEEE80211_LINKED &&
		ieee80211_is_54g(priv->ieee80211->current_network)) ||
		priv->ieee80211->iw_mode == IW_MODE_MONITOR;
	
	rtl8225_SetTXPowerLevel(dev, ch);
	
	write_rtl8225(dev, 0x7, rtl8225_chan[ch]);
	
	force_pci_posting(dev);
	mdelay(10);
	
	write_nic_byte(dev,SIFS,0x22);
	
	if(gset)
		write_nic_byte(dev,DIFS,20); 
	else
		write_nic_byte(dev,DIFS,0x24); 
	
	if(priv->ieee80211->state == IEEE80211_LINKED &&
		ieee80211_is_shortslot(priv->ieee80211->current_network))
		write_nic_byte(dev,SLOT,0x9); 
		
	else
		write_nic_byte(dev,SLOT,0x14); 
		
	
	if(gset){
		write_nic_byte(dev,EIFS,91 - 20); 
		write_nic_byte(dev,CW_VAL,0x73); 
	}else{
		write_nic_byte(dev,EIFS,91 - 0x24); 
		write_nic_byte(dev,CW_VAL,0xa5); 
	}


}

void rtl8225_rf_sleep(struct net_device *dev)
{

} 

void rtl8225_rf_wakeup(struct net_device *dev) 
{


	
}

void rtl8225_host_pci_init(struct net_device *dev) 
{
	write_nic_word(dev, RFPinsOutput, 0x480);
	
	rtl8185_rf_pins_enable(dev);
	
	write_nic_word(dev, RFPinsSelect, 0x88 | SW_CONTROL_GPIO); 
	
	write_nic_byte(dev, GP_ENABLE, 0);
	
	force_pci_posting(dev);
	mdelay(200);
	
	write_nic_word(dev, GP_ENABLE, 0xff & (~(1<<6))); 

	
}

void rtl8225_host_usb_init(struct net_device *dev) 
{
	write_nic_byte(dev,RFPinsSelect+1,0);

	write_nic_byte(dev,GPIO,0);
	
	write_nic_byte_E(dev,0x53,read_nic_byte_E(dev,0x53) | (1<<7));
		
	write_nic_byte(dev,RFPinsSelect+1,4);

	write_nic_byte(dev,GPIO,0x20);

	write_nic_byte(dev,GP_ENABLE,0);

			
	write_nic_word(dev, RFPinsOutput, 0x80);

	write_nic_word(dev, RFPinsSelect, 0x80);

	write_nic_word(dev, RFPinsEnable, 0x80);

	
	mdelay(100);

	mdelay(1000); 

}

void rtl8225_rf_init(struct net_device *dev) 
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int i;
	short channel = 1;
	u16 brsr;
	
	priv->chan = channel;

	rtl8180_set_anaparam(dev, RTL8225_ANAPARAM_ON);
	

	if(priv->card_type == USB)
		rtl8225_host_usb_init(dev);
	else
		rtl8225_host_pci_init(dev);

	write_nic_dword(dev, RF_TIMING, 0x000a8008);
	
	brsr = read_nic_word(dev, BRSR);
	
	write_nic_word(dev, BRSR, 0xffff); 

	#if 0
	if(priv->card_8185 == 1){
		if(priv->card_8185_Bversion)  
			write_nic_dword(dev, RF_PARA, 0x44);
		else    
			write_nic_dword(dev, RF_PARA, 0x100044);
	}else{ 
		if(priv->enable_gpio0)	
			write_nic_dword(dev, RF_PARA, 0x20100044);  
		else 
			write_nic_dword(dev, RF_PARA, 0x100044);
	}
	#endif
	
	write_nic_dword(dev, RF_PARA, 0x100044);
	
	#if 1  
	rtl8180_set_mode(dev, EPROM_CMD_CONFIG);
	write_nic_byte(dev, CONFIG3, 0x44);
	rtl8180_set_mode(dev, EPROM_CMD_NORMAL);
	#endif
	
	if(priv->card_type == USB){
		rtl8185_rf_pins_enable(dev);

		mdelay(1000);
	}

	write_rtl8225(dev, 0x0, 0x67); mdelay(1);
	
	
	write_rtl8225(dev, 0x1, 0xfe0); mdelay(1);

	write_rtl8225(dev, 0x2, 0x44d); mdelay(1);

	write_rtl8225(dev, 0x3, 0x441); mdelay(1);

	if(priv->card_type == USB)
		write_rtl8225(dev, 0x4, 0x486);
	else
		write_rtl8225(dev, 0x4, 0x8be);

	mdelay(1);
	
	
	#if 0
	}else if(priv->phy_ver == 1){
		write_rtl8225(dev, 0x5, 0xbc0 + 2);
	}else{
	#endif
	
	if(priv->card_type == USB)
		write_rtl8225(dev, 0x5, 0xbc0);
	else if(priv->card_type == MINIPCI)
		write_rtl8225(dev, 0x5, 0xbc0 + 3 +(6<<3));
	else
		write_rtl8225(dev, 0x5, 0xbc0 + (6<<3));

	 mdelay(1);
	
	write_rtl8225(dev, 0x6, 0xae6);  mdelay(1);

	write_rtl8225(dev, 0x7, ((priv->card_type == USB)? 0x82a : rtl8225_chan[channel]));  mdelay(1);

	write_rtl8225(dev, 0x8, 0x1f);  mdelay(1);

	write_rtl8225(dev, 0x9, 0x334);  mdelay(1);

	write_rtl8225(dev, 0xa, 0xfd4);  mdelay(1);

	write_rtl8225(dev, 0xb, 0x391);  mdelay(1);

	write_rtl8225(dev, 0xc, 0x50);  mdelay(1);


	write_rtl8225(dev, 0xd, 0x6db);   mdelay(1);

	write_rtl8225(dev, 0xe, 0x29);  mdelay(1);

	write_rtl8225(dev, 0xf, 0x914); 
	
	if(priv->card_type == USB){
		mdelay(100);
	}
	
	write_rtl8225(dev, 0x2, 0xc4d);
	
	if(priv->card_type == USB){
		mdelay(200);
		
		write_rtl8225(dev, 0x2, 0x44d);
		
		mdelay(100);
		
	}
	force_pci_posting(dev);
	
	mdelay(100); 
	
	
	write_rtl8225(dev, 0x0, 0x127);
	
	for(i=0;i<95;i++){
		write_rtl8225(dev, 0x1, (u8)(i+1));
		
		#if 0
		if(priv->phy_ver == 1) 
			write_rtl8225(dev, 0x2, rtl8225a_rxgain[i]);
		else
		#endif
		
		write_rtl8225(dev, 0x2, rtl8225bcd_rxgain[i]);
	}
	
	write_rtl8225(dev, 0x0, 0x27);


	
	write_rtl8225(dev, 0x0, 0x22f);   
	
	if(priv->card_type != USB)
		rtl8185_rf_pins_enable(dev);
	
	for(i=0;i<128;i++){
		write_phy_ofdm(dev, 0xb, rtl8225_agc[i]);
		
		mdelay(1); 
		write_phy_ofdm(dev, 0xa, (u8)i+ 0x80);
	
		mdelay(1); 
	}
	
	force_pci_posting(dev);
	mdelay(1);
	
	write_phy_ofdm(dev, 0x0, 0x1); mdelay(1);
	write_phy_ofdm(dev, 0x1, 0x2); mdelay(1);
	write_phy_ofdm(dev, 0x2, ((priv->card_type == USB)? 0x42 : 0x62)); mdelay(1);
	write_phy_ofdm(dev, 0x3, 0x0); mdelay(1);
	write_phy_ofdm(dev, 0x4, 0x0); mdelay(1);
	write_phy_ofdm(dev, 0x5, 0x0); mdelay(1);
	write_phy_ofdm(dev, 0x6, 0x40); mdelay(1);
	write_phy_ofdm(dev, 0x7, 0x0); mdelay(1);
	write_phy_ofdm(dev, 0x8, 0x40); mdelay(1);
	write_phy_ofdm(dev, 0x9, 0xfe); mdelay(1);
	
	#if 0
	if(priv->card_type == USB){
		write_phy_ofdm(dev, 0xa, 0x9);
	}else{
		if(priv->card_8185 == 1 && priv->card_8185_Bversion){ 
			* maybe later version can accept this also? 
			*/
			write_phy_ofdm(dev, 0xa, 0x6);
			write_phy_ofdm(dev, 0x18, 0x6f);
		}else{
	#endif
	write_phy_ofdm(dev, 0xa, 0x9); mdelay(1);

	write_phy_ofdm(dev, 0xb, 0x80); mdelay(1);

	write_phy_ofdm(dev, 0xc, 0x1);mdelay(1);

	
		
	write_phy_ofdm(dev, 0xe, 0xd3);mdelay(1);

	
	#if 0
	if(priv->card_8185 == 1){
		if(priv->card_8185_Bversion)
			write_phy_ofdm(dev, 0xf, 0x20);
		else
			write_phy_ofdm(dev, 0xf, 0x28);
	}else{
	#endif
	write_phy_ofdm(dev, 0xf, 0x38);mdelay(1);
	
	write_phy_ofdm(dev, 0x10, 0x84);mdelay(1);
	
	write_phy_ofdm(dev, 0x11, 0x06);mdelay(1);

	
	write_phy_ofdm(dev, 0x12, 0x20);mdelay(1);

	write_phy_ofdm(dev, 0x13, 0x20);mdelay(1);

#if 0
	}else{
		write_phy_ofdm(dev, 0x12, 0x0);
		write_phy_ofdm(dev, 0x13, 0x0);
	}
#endif
	write_phy_ofdm(dev, 0x14, 0x0); mdelay(1);
	write_phy_ofdm(dev, 0x15, 0x40); mdelay(1);
	write_phy_ofdm(dev, 0x16, 0x0); mdelay(1);
	write_phy_ofdm(dev, 0x17, 0x40); mdelay(1);
	
	
	write_phy_ofdm(dev, 0x18, 0xef);mdelay(1);
 

	write_phy_ofdm(dev, 0x19, 0x19); mdelay(1);
	write_phy_ofdm(dev, 0x1a, 0x20); mdelay(1);
	
	write_phy_ofdm(dev, 0x1b, 0x76);mdelay(1);
	
	write_phy_ofdm(dev, 0x1c, 0x4);mdelay(1);

#if 0	
	if(priv->card_8185 == 1){
		if(priv->card_8185_Bversion){
			write_phy_ofdm(dev, 0x1e, 0x95);
			write_phy_ofdm(dev, 0x1f, 0x55);
		}else{
			write_phy_ofdm(dev, 0x1e, 0x90);
			write_phy_ofdm(dev, 0x1f, 0x34);
			
		}
	}else{
#endif
	write_phy_ofdm(dev, 0x1e, 0x95);mdelay(1);

	write_phy_ofdm(dev, 0x1f, 0x75);	mdelay(1);

	
	write_phy_ofdm(dev, 0x20, 0x1f);mdelay(1);

	write_phy_ofdm(dev, 0x21, 0x27);mdelay(1);
	
	write_phy_ofdm(dev, 0x22, 0x16);mdelay(1);

	
	write_phy_ofdm(dev, 0x24, 0x46); mdelay(1);
	write_phy_ofdm(dev, 0x25, 0x20); mdelay(1);
	write_phy_ofdm(dev, 0x26, 0x90); mdelay(1);
#if 0
	if(priv->card_8185 == 1 && priv->card_8185_Bversion)
		write_phy_ofdm(dev, 0x27, 0x08); 
	else
#endif
	write_phy_ofdm(dev, 0x27, 0x88); mdelay(1);
	
	
	rtl8225_set_gain(dev,4);
		
	write_phy_cck(dev, 0x0, 0x98); mdelay(1);
	write_phy_cck(dev, 0x3, 0x20); mdelay(1);
	write_phy_cck(dev, 0x4, 0x7e); mdelay(1);
	write_phy_cck(dev, 0x5, 0x12); mdelay(1);
	write_phy_cck(dev, 0x6, 0xfc); mdelay(1);
#if 0	
	if(priv->card_8185 == 1 && priv->card_8185_Bversion)
		write_phy_cck(dev, 0x7, 0xd8); 
	else
#endif
	write_phy_cck(dev, 0x7, 0x78);mdelay(1);

	write_phy_cck(dev, 0x8, 0x2e);mdelay(1);

	write_phy_cck(dev, 0x10, ((priv->card_type == USB) ? 0x9b: 0x93)); mdelay(1);
	write_phy_cck(dev, 0x11, 0x88); mdelay(1);
	write_phy_cck(dev, 0x12, 0x47); mdelay(1);
#if 0
	if(priv->card_8185 == 1 && priv->card_8185_Bversion)
		write_phy_cck(dev, 0x13, 0x98); 
	else
#endif
	write_phy_cck(dev, 0x13, 0xd0); 
		
	write_phy_cck(dev, 0x19, 0x0);
	write_phy_cck(dev, 0x1a, 0xa0);
	write_phy_cck(dev, 0x1b, 0x8);
	write_phy_cck(dev, 0x40, 0x86); 
	
	write_phy_cck(dev, 0x41, 0x8d);mdelay(1);

	
	write_phy_cck(dev, 0x42, 0x15); mdelay(1);
	write_phy_cck(dev, 0x43, 0x18); mdelay(1);
	write_phy_cck(dev, 0x44, 0x1f); mdelay(1);
	write_phy_cck(dev, 0x45, 0x1e); mdelay(1);
	write_phy_cck(dev, 0x46, 0x1a); mdelay(1);
	write_phy_cck(dev, 0x47, 0x15); mdelay(1);
	write_phy_cck(dev, 0x48, 0x10); mdelay(1);
	write_phy_cck(dev, 0x49, 0xa); mdelay(1);
	write_phy_cck(dev, 0x4a, 0x5); mdelay(1);
	write_phy_cck(dev, 0x4b, 0x2); mdelay(1);
	write_phy_cck(dev, 0x4c, 0x5);mdelay(1);


	write_nic_byte(dev, 0x5b, 0x0d); mdelay(1);

	

	
	rtl8225_SetTXPowerLevel(dev, channel);
	
	write_phy_cck(dev, 0x10, 0x9b); mdelay(1); 
	write_phy_ofdm(dev, 0x26, 0x90); mdelay(1); 
	
	rtl8185_tx_antenna(dev, 0x3); 
	
	if(priv->card_type == USB)
		write_nic_dword(dev, 0x94, 0x3dc00002);
	else{
		write_nic_dword(dev, 0x94, 0x15c00002);
		rtl8185_rf_pins_enable(dev);
	}

	
	rtl8225_rf_set_chan(dev, priv->chan);


}
