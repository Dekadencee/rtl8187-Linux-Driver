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

#ifndef RTL8225H
#define RTL8225H

#include "r8187.h"

#define RTL8225_ANAPARAM_ON  0xa0000a59

#define RTL8225_ANAPARAM_OFF 0xa00beb59
#define RTL8225_ANAPARAM2_OFF 0x840dec11

#define RTL8225_ANAPARAM2_ON  0x860c7312
void rtl8225_rf_init(struct net_device *dev);
void rtl8225z2_rf_init(struct net_device *dev);
void rtl8225z2_rf_set_chan(struct net_device *dev, short ch);
short rtl8225_is_V_z2(struct net_device *dev);
void rtl8225_rf_set_chan(struct net_device *dev,short ch);
void rtl8225_rf_close(struct net_device *dev);
short rtl8225_rf_set_sens(struct net_device *dev, short sens);
void rtl8225_rf_wakeup(struct net_device *dev);
void rtl8225_rf_sleep(struct net_device *dev);
void rtl8225_host_pci_init(struct net_device *dev);
void rtl8225_host_usb_init(struct net_device *dev);
void write_rtl8225(struct net_device *dev, u8 adr, u16 data);
void rtl8225z2_rf_set_mode(struct net_device *dev) ;
void rtl8185_rf_pins_enable(struct net_device *dev);
void rtl8180_set_mode(struct net_device *dev,int mode);
void UpdateInitialGain(struct net_device *dev);
void UpdateCCKThreshold(struct net_device *dev);
void rtl8225_SetTXPowerLevel(struct net_device *dev, short ch);
void rtl8225z2_SetTXPowerLevel(struct net_device *dev, short ch);

#define RTL8225_RF_MAX_SENS 6
#define RTL8225_RF_DEF_SENS 4

extern u8 rtl8225_agc[];

extern u32 rtl8225_chan[];

#endif
