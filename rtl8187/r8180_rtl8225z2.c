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

u8 rtl8225z2_threshold[]={
        0x8d, 0x8d, 0x8d, 0x8d, 0x9d, 0xad, 0xbd,
};

u8 rtl8225z2_gain_bg[]={
	0x23, 0x15, 0xa5, 
        0x23, 0x15, 0xb5, 
        0x23, 0x15, 0xc5, 
        0x33, 0x15, 0xc5, 
        0x43, 0x15, 0xc5, 
        0x53, 0x15, 0xc5, 
        0x63, 0x15, 0xc5, 
};

u8 rtl8225z2_gain_a[]={
	0x13,0x27,0x5a,
	0x23,0x23,0x58,
	0x33,0x1f,0x56,
	0x43,0x1b,0x54,
	0x53,0x17,0x51,
	0x63,0x24,0x4f,
	0x73,0x0f,0x4c,
};
#if 0
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
#endif

#if 0
u16 rtl8225z2_rxgain[]={	
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
	0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03b0, 0x03b1, 0x03b2, 0x03b3,  
	0x03b4, 0x03b5, 0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bb

};
#else
u16 rtl8225z2_rxgain[]={
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0008, 0x0009,
	0x000a,	0x000b, 0x0102, 0x0103, 0x0104, 0x0105, 0x0140, 0x0141,	  
        0x0142,	0x0143, 0x0144, 0x0145, 0x0180, 0x0181, 0x0182, 0x0183,
	0x0184,	0x0185, 0x0188, 0x0189, 0x018a, 0x018b, 0x0243, 0x0244,	 
	0x0245,	0x0280, 0x0281, 0x0282, 0x0283, 0x0284, 0x0285, 0x0288,
        0x0289, 0x028a, 0x028b, 0x028c, 0x0342, 0x0343, 0x0344, 0x0345,
	0x0380, 0x0381, 0x0382, 0x0383, 0x0384, 0x0385, 0x0388, 0x0389,
	0x038a, 0x038b, 0x038c, 0x038d, 0x0390, 0x0391, 0x0392, 0x0393,
	0x0394, 0x0395, 0x0398, 0x0399, 0x039a, 0x039b, 0x039c, 0x039d,	  
	0x03a0, 0x03a1, 0x03a2, 0x03a3, 0x03a4, 0x03a5, 0x03a8, 0x03a9,	  
	0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03b0, 0x03b1, 0x03b2, 0x03b3,	
	0x03b4, 0x03b5, 0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bb
};
#endif



u8 ZEBRA2_CCK_OFDM_GAIN_SETTING[]={
        0x00,0x01,0x02,0x03,0x04,0x05,
        0x06,0x07,0x08,0x09,0x0a,0x0b,
        0x0c,0x0d,0x0e,0x0f,0x10,0x11,
        0x12,0x13,0x14,0x15,0x16,0x17,
        0x18,0x19,0x1a,0x1b,0x1c,0x1d,
        0x1e,0x1f,0x20,0x21,0x22,0x23,
};

#if 0
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
#endif

u8 rtl8225z2_tx_power_ofdm[]={
	0x42,0x00,0x40,0x00,0x40
};


u8 rtl8225z2_tx_power_cck_ch14[]={
	0x36,0x35,0x2e,0x1b,0x00,0x00,0x00,0x00
};


u8 rtl8225z2_tx_power_cck[]={
	0x36,0x35,0x2e,0x25,0x1c,0x12,0x09,0x04
};


void rtl8225z2_set_gain(struct net_device *dev, short gain)
{
	u8* rtl8225_gain;
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	u8 mode = priv->ieee80211->mode;
	if((mode&IEEE_B) == IEEE_B || (mode&IEEE_G) == IEEE_G) {
		rtl8225_gain = rtl8225z2_gain_bg;
	} else {
		rtl8225_gain = rtl8225z2_gain_a;
	}
		
        write_phy_ofdm(dev, 0x0d, rtl8225_gain[gain * 3]);
	mdelay(1);
        write_phy_ofdm(dev, 0x1b, rtl8225_gain[gain * 3 + 1]);
	mdelay(1);
        write_phy_ofdm(dev, 0x1d, rtl8225_gain[gain * 3 + 2]);
	mdelay(1);
	write_phy_ofdm(dev, 0x21, 0x17);
	mdelay(1);

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

u32 read_rtl8225(struct net_device *dev, u8 adr)
{
	u32 data2Write = ((u32)(adr & 0x1f)) << 27;
	u32 dataRead;
	u32 mask;
	u16 oval,oval2,oval3,tmp;
	int i;
	short bit, rw;
	
	u8 wLength = 6;
	u8 rLength = 12;
	u8 low2high = 0;

	oval = read_nic_word(dev, RFPinsOutput);
	oval2 = read_nic_word(dev, RFPinsEnable);
	oval3 = read_nic_word(dev, RFPinsSelect);

	write_nic_word(dev, RFPinsEnable, (oval2|0xf));
	write_nic_word(dev, RFPinsSelect, (oval3|0xf));

	dataRead = 0;

	oval &= ~0xf; 

	write_nic_word(dev, RFPinsOutput, oval | BB_HOST_BANG_EN ); udelay(4);

	write_nic_word(dev, RFPinsOutput, oval ); udelay(5);
	
	rw = 0;
	
	mask = (low2high) ? 0x01 : (((u32)0x01)<<(32-1));
	for(i = 0; i < wLength/2; i++)
	{
		bit = ((data2Write&mask) != 0) ? 1 : 0;
		write_nic_word(dev, RFPinsOutput, bit|oval | rw); udelay(1);
		
		write_nic_word(dev, RFPinsOutput, bit|oval | BB_HOST_BANG_CLK | rw); udelay(2);
		write_nic_word(dev, RFPinsOutput, bit|oval | BB_HOST_BANG_CLK | rw); udelay(2);

		mask = (low2high) ? (mask<<1): (mask>>1);

		if(i == 2)
		{
			rw = BB_HOST_BANG_RW;
			write_nic_word(dev, RFPinsOutput, bit|oval | BB_HOST_BANG_CLK | rw); udelay(2);
			write_nic_word(dev, RFPinsOutput, bit|oval | rw); udelay(2);
			break;
		}
		
		bit = ((data2Write&mask) != 0) ? 1: 0;
		
		write_nic_word(dev, RFPinsOutput, oval|bit|rw| BB_HOST_BANG_CLK); udelay(2);
		write_nic_word(dev, RFPinsOutput, oval|bit|rw| BB_HOST_BANG_CLK); udelay(2);

		write_nic_word(dev, RFPinsOutput, oval| bit |rw); udelay(1);

		mask = (low2high) ? (mask<<1) : (mask>>1);
	}

	write_nic_word(dev, RFPinsOutput, rw|oval); udelay(2);
	mask = (low2high) ? 0x01 : (((u32)0x01) << (12-1));

	for(i = 0; i < rLength; i++)
	{
		write_nic_word(dev, RFPinsOutput, rw|oval); udelay(1);
		
		write_nic_word(dev, RFPinsOutput, rw|oval|BB_HOST_BANG_CLK); udelay(2);
		write_nic_word(dev, RFPinsOutput, rw|oval|BB_HOST_BANG_CLK); udelay(2);
		write_nic_word(dev, RFPinsOutput, rw|oval|BB_HOST_BANG_CLK); udelay(2);
		tmp = read_nic_word(dev, RFPinsInput);
		
		dataRead |= (tmp & BB_HOST_BANG_CLK ? mask : 0);

		write_nic_word(dev, RFPinsOutput, (rw|oval)); udelay(2);

		mask = (low2high) ? (mask<<1) : (mask>>1);
	}
	
	write_nic_word(dev, RFPinsOutput, BB_HOST_BANG_EN|BB_HOST_BANG_RW|oval); udelay(2);

	write_nic_word(dev, RFPinsEnable, oval2);   
	write_nic_word(dev, RFPinsSelect, oval3);   
	write_nic_word(dev, RFPinsOutput, 0x3a0);

	return dataRead;
}
#if 0
void write_rtl8225(struct net_device *dev, u8 adr, u16 data)
{
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
}

#endif
short rtl8225_is_V_z2(struct net_device *dev)
{
	short vz2 = 1;

	write_nic_word(dev, RFPinsOutput, 0x0080);
	write_nic_word(dev, RFPinsSelect, 0x0080);
	write_nic_word(dev, RFPinsEnable, 0x0080);
	mdelay(100);

	mdelay(1000);
	
	write_rtl8225(dev, 0, 0x1b7);
	
	if( read_rtl8225(dev, 8) != 0x588)
		vz2 = 0;
	
	else	
		if( read_rtl8225(dev, 9) != 0x700)
			vz2 = 0;
	
	write_rtl8225(dev, 0, 0xb7);

	return vz2;
	
}

#if 0
void rtl8225_rf_close(struct net_device *dev)
{
	write_rtl8225(dev, 0x4, 0x1f);   
	
	force_pci_posting(dev);
	mdelay(1);
	
	rtl8180_set_anaparam(dev, RTL8225_ANAPARAM_OFF);
	rtl8185_set_anaparam2(dev, RTL8225_ANAPARAM2_OFF);
}
#endif
#if 0
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
#endif
void rtl8225z2_SetTXPowerLevel(struct net_device *dev, short ch)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	int i;
	u8 power;
	u8 *cck_power_table;
	u8 max_cck_power_level;
	u8 max_ofdm_power_level;
	u8 min_ofdm_power_level;	
	u8 cck_power_level = 0xff & priv->chtxpwr[ch];
	u8 ofdm_power_level = 0xff & priv->chtxpwr_ofdm[ch];
	
		
	max_cck_power_level = 15;
	max_ofdm_power_level = 25; 
	min_ofdm_power_level = 10;
	
	if(cck_power_level > max_cck_power_level)
		cck_power_level = max_cck_power_level;
	
	cck_power_level += priv->cck_txpwr_base;
	
	if(cck_power_level > 35)
		cck_power_level = 35;
		
	if(ch == 14) 
		cck_power_table = rtl8225z2_tx_power_cck_ch14;
	else 
		cck_power_table = rtl8225z2_tx_power_cck;
	
	
	for(i=0;i<8;i++){
	
		power = cck_power_table[i];
		write_phy_cck(dev, 0x44 + i, power);
	}
	
	write_nic_byte(dev, TX_GAIN_CCK, ZEBRA2_CCK_OFDM_GAIN_SETTING[cck_power_level]);
	
	force_pci_posting(dev);
	mdelay(1);
	
	if(ofdm_power_level > (max_ofdm_power_level - min_ofdm_power_level))
		ofdm_power_level = max_ofdm_power_level;
	else
		ofdm_power_level += min_ofdm_power_level;
	
	ofdm_power_level += priv->ofdm_txpwr_base;
		
	if(ofdm_power_level > 35)
		ofdm_power_level = 35;
	
	rtl8185_set_anaparam2(dev,RTL8225_ANAPARAM2_ON);
		
	write_phy_ofdm(dev,2,0x42);
	write_phy_ofdm(dev,5,0);
	write_phy_ofdm(dev,6,0x40);
	write_phy_ofdm(dev,7,0);
	write_phy_ofdm(dev,8,0x40);	
	

        write_nic_byte(dev, TX_GAIN_OFDM, ZEBRA2_CCK_OFDM_GAIN_SETTING[ofdm_power_level]);

	
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

void rtl8225z2_rf_set_chan(struct net_device *dev, short ch)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	short gset = (priv->ieee80211->state == IEEE80211_LINKED &&
		ieee80211_is_54g(priv->ieee80211->current_network)) ||
		priv->ieee80211->iw_mode == IW_MODE_MONITOR;
	
	rtl8225z2_SetTXPowerLevel(dev, ch);
	
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
#if 0
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
#endif
void
UpdateCCKThreshold(
	struct net_device *dev
	)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	switch(priv->StageCCKTh)
	{
	case 0:
		write_phy_cck(dev, 0xc1, 0x88);mdelay(1);
		break;
		
	case 1:
		write_phy_cck(dev, 0xc1, 0x98);mdelay(1);
		break;

	case 2:
		write_phy_cck(dev, 0xc1, 0xC8);mdelay(1);
		break;

	case 3:
		write_phy_cck(dev, 0xc1, 0xD8);mdelay(1);
		break;

	default:
		break;
	}
}
void
UpdateInitialGain(
	struct net_device *dev
	)
{
	struct r8180_priv *priv = ieee80211_priv(dev);


	switch(priv->rf_chip)
	{
#if 0
		case  RF_ZEBRA:

			switch(pHalData->InitialGain)
			{
				case 1: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 1: -82 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x238d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x7c9b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xa59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x88a3);	delay_ms(1);
					break;
					
				case 2: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 2: -82 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x238d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x7c9b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xb59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x88a3);	delay_ms(1);
					break;

				case 3: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 3: -82 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x238d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x7c9b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x88a3);	delay_ms(1);
					break;

				case 4: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 4: -78 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x338d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x799b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x80a3);	delay_ms(1);
					break;

				case 5: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 5: -74 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x438d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x769b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x78a3);	delay_ms(1);
					break;

				case 6: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 6: -70 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x538d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x739b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x60a3);	delay_ms(1);
					break;

				case 7: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 7: -66 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x638d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x709b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x58a3);	delay_ms(1);
					break;

				default:	
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 5: -74 dBm (default)\n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x438d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x769b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x78a3);	delay_ms(1);
					break;
				
			}
			

			
			break;


		case  RF_ZEBRA2:  
			switch(pHalData->InitialGain)
			{
				case 1: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 1: -82 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x238d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x159b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xa59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x17a1);	delay_ms(1);
					break;

				case 2: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 2: -82 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x238d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x159b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xb59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x17a1);	delay_ms(1);
					break;

				case 3: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 3: -82 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x238d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x159b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x17a1);	delay_ms(1);
					break;

				case 4: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 4: -78 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x338d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x159b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x17a1);	delay_ms(1);
					break;

				case 5: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 5: -74 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x438d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x159b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x17a1);	delay_ms(1);
					break;

				case 6: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 6: -70 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x538d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x159b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x17a1);	delay_ms(1);
					break;

				case 7: 
					RT_TRACE(COMP_DIG, DBG_LOUD, ("RTL8187 + 8225 Initial Gain State 7: -66 dBm \n"));
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x638d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x159b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, BBAddr, 0x17a1);	delay_ms(1);
					break;

				default:	
					RT_TRACE(COMP_DBG, DBG_LOUD, ("RTL8187 + Zebra2 Initial Gain State 4: -78 dBm (default)\n"));
					PlatformEFIOWrite4Byte(Adapter, 0xFF7C, 0x338d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, 0xFF7C, 0x159b);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, 0xFF7C, 0xc59d);	delay_ms(1);
					PlatformEFIOWrite4Byte(Adapter, 0xFF7C, 0x17a1);	delay_ms(1);
					break;	
			}
			break;
#endif
		case RF_ZEBRA:
		case RF_ZEBRA2:


			switch(priv->InitialGain)
			{
				case 1: 
					write_phy_ofdm(dev, 0x97, 0x26);	mdelay(1);
					write_phy_ofdm(dev, 0xa4, 0x86);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfa);	mdelay(1);
					break;
					
				case 2: 
					write_phy_ofdm(dev, 0x97, 0x36);	mdelay(1);
					write_phy_ofdm(dev, 0xa4, 0x86);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfa);	mdelay(1);
					break;

				case 3: 
					write_phy_ofdm(dev, 0x97, 0x36);	mdelay(1);
					write_phy_ofdm(dev, 0xa4, 0x86);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfb);	mdelay(1);
					break;

				case 4: 
					write_phy_ofdm(dev, 0x97, 0x46);	mdelay(1);
					write_phy_ofdm(dev, 0xa4, 0x86);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfb);	mdelay(1);
					break;

				case 5: 
					write_phy_ofdm(dev, 0x97, 0x46);	mdelay(1);
					write_phy_ofdm(dev, 0xa4, 0x96);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfb);	mdelay(1);
					break;

				case 6: 
					write_phy_ofdm(dev, 0x97, 0x56);	mdelay(1);					
					write_phy_ofdm(dev, 0xa4, 0x96);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfc);	mdelay(1);
					break;

				case 7: 
					write_phy_ofdm(dev, 0x97, 0x56);	mdelay(1);
					write_phy_ofdm(dev, 0xa4, 0xa6);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfc);	mdelay(1);
					break;

				case 8:
					write_phy_ofdm(dev, 0x97, 0x66);	mdelay(1);
					write_phy_ofdm(dev, 0xa4, 0xb6);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfc);	mdelay(1);
					break;

				default:	
					write_phy_ofdm(dev, 0x97, 0x26);	mdelay(1);
					write_phy_ofdm(dev, 0xa4, 0x86);	mdelay(1);
					write_phy_ofdm(dev, 0x85, 0xfa);	mdelay(1);
					break;	
			}			
			break;

		default:
			break;
	}
}
#if 0
void PhyConfig8187(struct net_device *dev)
{
        struct r8180_priv *priv = ieee80211_priv(dev);
	u8			btConfig4;

	btConfig4 = read_nic_byte(dev, CONFIG4);
	priv->RFProgType = (btConfig4 & 0x03);



	switch(priv->rf_chip)
	{
	case  RF_ZEBRA2:
		ZEBRA_Config_87BASIC(dev);
		break;
	}
	if(priv->bDigMechanism)
	{
		if(priv->InitialGain == 0)
			priv->InitialGain = 4;
	}

	UpdateCCKThreshold(dev);
	UpdateInitialGain(dev);
	return ;
}
#endif
void rtl8225z2_rf_init(struct net_device *dev) 
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int i;
	short channel = 1;
	u16	brsr;
	u32	data,addr;
	
	priv->chan = channel;

	rtl8180_set_anaparam(dev, RTL8225_ANAPARAM_ON);

	if(priv->card_type == USB)
		rtl8225_host_usb_init(dev);
	else
		rtl8225_host_pci_init(dev);

	write_nic_dword(dev, RF_TIMING, 0x000a8008);
	
	brsr = read_nic_word(dev, BRSR);
	
	write_nic_word(dev, BRSR, 0xffff); 


	write_nic_dword(dev, RF_PARA, 0x100044);
	
	#if 1  
	rtl8180_set_mode(dev, EPROM_CMD_CONFIG);
	write_nic_byte(dev, CONFIG3, 0x44);
	rtl8180_set_mode(dev, EPROM_CMD_NORMAL);
	#endif
	
	
	rtl8185_rf_pins_enable(dev);


	write_rtl8225(dev, 0x0, 0x2bf); mdelay(1);
	
	
	write_rtl8225(dev, 0x1, 0xee0); mdelay(1);

	write_rtl8225(dev, 0x2, 0x44d); mdelay(1);

	write_rtl8225(dev, 0x3, 0x441); mdelay(1);

	
	write_rtl8225(dev, 0x4, 0x8c3);mdelay(1);
	
	
	
	write_rtl8225(dev, 0x5, 0xc72);mdelay(1);
	
	write_rtl8225(dev, 0x6, 0xe6);  mdelay(1);

	write_rtl8225(dev, 0x7, ((priv->card_type == USB)? 0x82a : rtl8225_chan[channel]));  mdelay(1);

	write_rtl8225(dev, 0x8, 0x3f);  mdelay(1);

	write_rtl8225(dev, 0x9, 0x335);  mdelay(1);

	write_rtl8225(dev, 0xa, 0x9d4);  mdelay(1);

	write_rtl8225(dev, 0xb, 0x7bb);  mdelay(1);

	write_rtl8225(dev, 0xc, 0x850);  mdelay(1);


	write_rtl8225(dev, 0xd, 0xcdf);   mdelay(1);

	write_rtl8225(dev, 0xe, 0x2b);  mdelay(1);

	write_rtl8225(dev, 0xf, 0x114); 
	
	
	mdelay(100);
	
	
	
	write_rtl8225(dev, 0x0, 0x1b7);
	
	for(i=0;i<95;i++){
		write_rtl8225(dev, 0x1, (u8)(i+1));
		
		#if 0
		if(priv->phy_ver == 1) 
			write_rtl8225(dev, 0x2, rtl8225a_rxgain[i]);
		else
		#endif
		
		write_rtl8225(dev, 0x2, rtl8225z2_rxgain[i]);
	}
	write_rtl8225(dev, 0x3, 0x2);
	write_rtl8225(dev, 0x5, 0x4);

	write_rtl8225(dev, 0x0, 0xb7);

	write_rtl8225(dev, 0x2, 0xc4d);
	
	if(priv->card_type == USB){
		mdelay(200);
		
		write_rtl8225(dev, 0x2, 0x44d);
		
		mdelay(200);
		
	}
        data = read_rtl8225(dev, 6);
        if (!(data&0x00000080))
        {
                write_rtl8225(dev, 0x02, 0x0c4d);
                force_pci_posting(dev); mdelay(200);
                write_rtl8225(dev, 0x02, 0x044d);
                force_pci_posting(dev); mdelay(100);
                data = read_rtl8225(dev, 6);
                if (!(data&0x00000080))
                        {
                                DMESGW("RF Calibration Failed!!!!\n");
                        }
        }
	
	mdelay(200); 
	
	
	
	write_rtl8225(dev, 0x0, 0x2bf);   
	
	if(priv->card_type != USB)
		rtl8185_rf_pins_enable(dev);
        for(i=0;i<128;i++){
                data = rtl8225_agc[i];

                addr = i + 0x80; 
                write_phy_ofdm(dev, 0xb, data);

                mdelay(1);
                write_phy_ofdm(dev, 0xa, addr);

                mdelay(1);
        }
#if 0	
	for(i=0;i<128;i++){
		write_phy_ofdm(dev, 0xb, rtl8225_agc[i]);
		
		mdelay(1); 
		write_phy_ofdm(dev, 0xa, (u8)i+ 0x80);
	
		mdelay(1); 
	}
#endif
		
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

	write_phy_ofdm(dev, 0xa, 0x8); mdelay(1);

	write_phy_ofdm(dev, 0xb, 0x80); mdelay(1);

	write_phy_ofdm(dev, 0xc, 0x1);mdelay(1);

	
	write_phy_ofdm(dev, 0xd, 0x43); 
		
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
	
	write_phy_ofdm(dev, 0x11, 0x07);mdelay(1);

	
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
	write_phy_ofdm(dev, 0x1b, 0x15);mdelay(1);
	
	write_phy_ofdm(dev, 0x1c, 0x4);mdelay(1);

	write_phy_ofdm(dev, 0x1d, 0xc5);mdelay(1); 
	
	write_phy_ofdm(dev, 0x1e, 0x95);mdelay(1);

	write_phy_ofdm(dev, 0x1f, 0x75);	mdelay(1);

	
	write_phy_ofdm(dev, 0x20, 0x1f);mdelay(1);

	write_phy_ofdm(dev, 0x21, 0x17);mdelay(1);
	
	write_phy_ofdm(dev, 0x22, 0x16);mdelay(1);

	write_phy_ofdm(dev, 0x23, 0x80);mdelay(1); 
	
	write_phy_ofdm(dev, 0x24, 0x46); mdelay(1);
	write_phy_ofdm(dev, 0x25, 0x00); mdelay(1);
	write_phy_ofdm(dev, 0x26, 0x90); mdelay(1);

	write_phy_ofdm(dev, 0x27, 0x88); mdelay(1);

	
	
	rtl8225z2_set_gain(dev,4);
	
	write_phy_cck(dev, 0x0, 0x98); mdelay(1);
	write_phy_cck(dev, 0x3, 0x20); mdelay(1);
	write_phy_cck(dev, 0x4, 0x7e); mdelay(1);
	write_phy_cck(dev, 0x5, 0x12); mdelay(1);
	write_phy_cck(dev, 0x6, 0xfc); mdelay(1);
	write_phy_cck(dev, 0x7, 0x78);mdelay(1);
	write_phy_cck(dev, 0x8, 0x2e);mdelay(1);

	write_phy_cck(dev, 0x9, 0x11);mdelay(1);
	write_phy_cck(dev, 0xa, 0x17);mdelay(1);
	write_phy_cck(dev, 0xb, 0x11);mdelay(1);
	
	write_phy_cck(dev, 0x10, ((priv->card_type == USB) ? 0x9b: 0x93)); mdelay(1);
	write_phy_cck(dev, 0x11, 0x88); mdelay(1);
	write_phy_cck(dev, 0x12, 0x47); mdelay(1);
#if 0
	if(priv->card_8185 == 1 && priv->card_8185_Bversion)
		write_phy_cck(dev, 0x13, 0x98); 
	else
#endif
	write_phy_cck(dev, 0x13, 0xd0); 
		
	write_phy_cck(dev, 0x19, 0x0); mdelay(1);
	write_phy_cck(dev, 0x1a, 0xa0); mdelay(1);
	write_phy_cck(dev, 0x1b, 0x8); mdelay(1);
	write_phy_cck(dev, 0x1d, 0x0); mdelay(1);
	
	write_phy_cck(dev, 0x40, 0x86); 
	
	write_phy_cck(dev, 0x41, 0x9d);mdelay(1);

	
	write_phy_cck(dev, 0x42, 0x15); mdelay(1);
	write_phy_cck(dev, 0x43, 0x18); mdelay(1);
	
	
	write_phy_cck(dev, 0x44, 0x36); mdelay(1);
	write_phy_cck(dev, 0x45, 0x35); mdelay(1);
	write_phy_cck(dev, 0x46, 0x2e); mdelay(1);
	write_phy_cck(dev, 0x47, 0x25); mdelay(1);
	write_phy_cck(dev, 0x48, 0x1c); mdelay(1);
	write_phy_cck(dev, 0x49, 0x12); mdelay(1);
	write_phy_cck(dev, 0x4a, 0x09); mdelay(1);
	write_phy_cck(dev, 0x4b, 0x04); mdelay(1);
	write_phy_cck(dev, 0x4c, 0x5);mdelay(1);


	write_nic_byte(dev, 0x5b, 0x0d); mdelay(1);

	

	
	rtl8225z2_SetTXPowerLevel(dev, channel);
	
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

void rtl8225z2_rf_set_mode(struct net_device *dev) 
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->mode == IEEE_A)
	{
		write_rtl8225(dev, 0x5, 0x1865);
		write_nic_dword(dev, RF_PARA, 0x10084);
		write_nic_dword(dev, RF_TIMING, 0xa8008);
		write_phy_ofdm(dev, 0x0, 0x0);
		write_phy_ofdm(dev, 0xa, 0x6);
		write_phy_ofdm(dev, 0xb, 0x99);
		write_phy_ofdm(dev, 0xf, 0x20);
		write_phy_ofdm(dev, 0x11, 0x7);
		
		rtl8225z2_set_gain(dev,4);
		
		write_phy_ofdm(dev,0x15, 0x40);
		write_phy_ofdm(dev,0x17, 0x40);
	
		write_nic_dword(dev, 0x94,0x10000000);
	}else{
	
		write_rtl8225(dev, 0x5, 0x1864);
		write_nic_dword(dev, RF_PARA, 0x10044);
		write_nic_dword(dev, RF_TIMING, 0xa8008);
		write_phy_ofdm(dev, 0x0, 0x1);
		write_phy_ofdm(dev, 0xa, 0x6);
		write_phy_ofdm(dev, 0xb, 0x99);
		write_phy_ofdm(dev, 0xf, 0x20);
		write_phy_ofdm(dev, 0x11, 0x7);
		
		rtl8225z2_set_gain(dev,4);
		
		write_phy_ofdm(dev,0x15, 0x40);
		write_phy_ofdm(dev,0x17, 0x40);
	
		write_nic_dword(dev, 0x94,0x04000002);
	}
}
