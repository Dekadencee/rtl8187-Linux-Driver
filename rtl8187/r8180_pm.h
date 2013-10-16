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

#ifdef CONFIG_PM

#ifndef R8180_PM_H
#define R8180_PM_H

#include <linux/types.h>
#include <linux/usb.h>

int rtl8180_save_state (struct pci_dev *dev, u32 state);
int rtl8187_suspend  (struct usb_interface *intf,pm_message_t state);
int rtl8187_resume (struct usb_interface *intf);
int rtl8180_enable_wake (struct pci_dev *dev, u32 state, int enable);

#endif 

#endif 
