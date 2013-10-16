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

#ifndef R8187_LED_H
#define R8187_LED_H

#include <linux/types.h>
#include <linux/timer.h>


#define EEPROM_SW_REVD_OFFSET		0x7E

#define EEPROM_CID_MASK			0x00FF
#define EEPROM_CID_RSVD0			0x00
#define EEPROM_CID_RSVD1			0xFF
#define EEPROM_CID_ALPHA0			0x01
#define EEPROM_CID_SERCOMM_PS	0x02
#define EEPROM_CID_HW_LED			0x03

#define LED_BLINK_NORMAL_INTERVAL	100 
#define LED_BLINK_SLOWLY_INTERVAL	200

#define LED_CM2_BLINK_ON_INTERVAL		250
#define LED_CM2_BLINK_OFF_INTERVAL	4750

#define LED_CM3_BLINK_INTERVAL			1500




typedef	enum _LED_STATE_8187{
	LED_UNKNOWN = 0,
	LED_ON = 1,
	LED_OFF = 2,
	LED_BLINK_NORMAL = 3,
	LED_BLINK_SLOWLY = 4,
	LED_POWER_ON_BLINK = 5,
	LED_SCAN_BLINK = 6, 	
	LED_NO_LINK_BLINK = 7,	
	LED_BLINK_CM3 = 8, 		
}LED_STATE_8187;

typedef enum _RT_CID_TYPE {
	RT_CID_DEFAULT,
	RT_CID_8187_ALPHA0,
	RT_CID_8187_SERCOMM_PS,
	RT_CID_8187_HW_LED,
} RT_CID_TYPE;

typedef	enum _LED_STRATEGY_8187{
	SW_LED_MODE0, 
	SW_LED_MODE1, 
	SW_LED_MODE2, 
	SW_LED_MODE3, 
	HW_LED, 		
}LED_STRATEGY_8187, *PLED_STRATEGY_8187;

typedef enum _LED_PIN_8187{
	LED_PIN_GPIO0,
	LED_PIN_LED0,
	LED_PIN_LED1
}LED_PIN_8187;


typedef struct _LED_8187{
	LED_PIN_8187		LedPin;	

	LED_STATE_8187		CurrLedState; 
	u8				bLedOn; 

	u8				bLedBlinkInProgress; 
	u32				BlinkTimes; 
	LED_STATE_8187		BlinkingLedState; 
	struct timer_list		BlinkTimer;  
} LED_8187, *PLED_8187;





void
InitSwLeds(
	struct net_device *dev
	);

void
DeInitSwLeds(
	struct net_device *dev
	);

void
InitLed8187(
	struct net_device *dev, 
	PLED_8187		pLed,
	LED_PIN_8187		LedPin,
	void	* 		BlinkCallBackFunc);

void
DeInitLed8187(
	struct net_device *dev, 
	PLED_8187			pLed);

void
LedControl8187(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
);

void
SwLedControlMode0(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
);

void
SwLedControlMode1(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
);

void
SwLedControlMode2(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
);

void
SwLedControlMode3(
	struct net_device *dev,
	LED_CTL_MODE		LedAction
);

void
Gpio0LedBlinkTimerCallback(
	unsigned long		data
	);

void
SwLed0BlinkTimerCallback(
	unsigned long		data
	);

void
SwLed1BlinkTimerCallback(
	unsigned long		data
	);

void
PlatformSwLedBlink(
	struct net_device *dev,
	PLED_8187		pLed
	);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
void
Gpio0LedWorkItemCallback(struct net_device *dev);

void
SwLed0WorkItemCallback(struct net_device *dev);

void
SwLed1WorkItemCallback(struct net_device *dev);
#else
void
Gpio0LedWorkItemCallback(struct work_struct *work);

void
SwLed0WorkItemCallback(struct work_struct *work);

void
SwLed1WorkItemCallback(struct work_struct *work);
#endif
void
SwLedBlink(
	struct net_device *dev, 
	PLED_8187			pLed
	);

void
SwLedCm2Blink(
	struct net_device *dev, 
	PLED_8187			pLed
	);

void
SwLedOn(
	struct net_device *dev, 
	PLED_8187			pLed
);

void
SwLedOff(
	struct net_device *dev, 
	PLED_8187			pLed
);


#endif
