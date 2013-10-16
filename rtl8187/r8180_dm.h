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

#ifndef R8180_DM_H
#define R8180_DM_H

#include "r8187.h"

bool CheckDig(struct net_device *dev);
bool CheckHighPower(struct net_device *dev);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_hw_dig_wq (struct work_struct *work);
#else
void rtl8180_hw_dig_wq(struct net_device *dev);
#endif

bool CheckHighPower(struct net_device *dev);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_tx_pw_wq (struct work_struct *work);
#else
void rtl8180_tx_pw_wq(struct net_device *dev);
#endif



#ifdef SW_ANTE_DIVERSITY
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void SwAntennaWorkItemCallback(struct work_struct *work);
#else   
void SwAntennaWorkItemCallback(struct net_device *dev);
#endif
void SwAntennaDiversityRxOk8185(struct net_device *dev, u8 SignalStrength);
void SwAntennaDiversityTimerCallback(struct net_device *dev);
#endif 
#endif

#endif
