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

#ifndef __devinit
#define __devinit
#define __devexit
#endif

#ifndef R8180H
#define R8180H

#define RTL8187_MODULE_NAME "rtl8187L"
#define DMESG(x,a...) printk(KERN_INFO RTL8187_MODULE_NAME ": " x "\n", ## a)
#define DMESGW(x,a...) printk(KERN_WARNING RTL8187_MODULE_NAME ": WW:" x "\n", ## a)
#define DMESGE(x,a...) printk(KERN_WARNING RTL8187_MODULE_NAME ": EE:" x "\n", ## a)

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#include <linux/usb.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>
#include <linux/rtnetlink.h>
#include <linux/wireless.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <linux/if_arp.h>
#include <linux/random.h>
#include <linux/version.h>
#include <asm/io.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
#include <asm/semaphore.h>
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18))
#include <linux/config.h>
#endif
#include "../ieee80211/ieee80211.h"
#ifdef LED
#include "r8187_led.h"
#endif
#include "r8180_hw.h"

#define FALSE 0
#define TRUE 1
#define MAX_KEY_LEN     61
#define KEY_BUF_SIZE    5

#define BIT0    0x00000001
#define BIT1    0x00000002
#define BIT2    0x00000004
#define BIT3    0x00000008
#define BIT4    0x00000010
#define BIT5    0x00000020
#define BIT6    0x00000040
#define BIT7    0x00000080
#define BIT8    0x00000100
#define BIT9    0x00000200
#define BIT10   0x00000400
#define BIT11   0x00000800
#define BIT12   0x00001000
#define BIT13   0x00002000
#define BIT14   0x00004000
#define BIT15   0x00008000
#define BIT16   0x00010000
#define BIT17   0x00020000
#define BIT18   0x00040000
#define BIT19   0x00080000
#define BIT20   0x00100000
#define BIT21   0x00200000
#define BIT22   0x00400000
#define BIT23   0x00800000
#define BIT24   0x01000000
#define BIT25   0x02000000
#define BIT26   0x04000000
#define BIT27   0x08000000
#define BIT28   0x10000000
#define BIT29   0x20000000
#define BIT30   0x40000000
#define BIT31   0x80000000

#define EPROM_93c46 0
#define EPROM_93c56 1

#define DEFAULT_FRAG_THRESHOLD 2342U
#define MIN_FRAG_THRESHOLD     256U
#define DEFAULT_RTS_THRESHOLD 2342U
#define MIN_RTS_THRESHOLD 0U
#define MAX_RTS_THRESHOLD 2342U

#define DEFAULT_BEACONINTERVAL 0x64U
#define DEFAULT_BEACON_ESSID "Rtl8187"

#define DEFAULT_SSID ""
#define DEFAULT_RETRY_RTS 7
#define DEFAULT_RETRY_DATA 7
#define PRISM_HDR_SIZE 64

typedef enum _WIRELESS_MODE {
    WIRELESS_MODE_UNKNOWN = 0x00,
    WIRELESS_MODE_A = 0x01,
    WIRELESS_MODE_B = 0x02,
    WIRELESS_MODE_G = 0x04,
    WIRELESS_MODE_AUTO = 0x08,
} WIRELESS_MODE;

typedef enum _TR_SWITCH_STATE{
    TR_HW_CONTROLLED = 0,
    TR_SW_TX = 1,
}TR_SWITCH_STATE, *PTR_SWITCH_STATE;


#define RTL_IOCTL_WPA_SUPPLICANT        SIOCIWFIRSTPRIV+30

typedef struct buffer
{
    struct buffer *next;
    u32 *buf;

} buffer;

typedef struct rtl_reg_debug{
        unsigned int  cmd;
        struct {
                unsigned char type;
                unsigned char addr;
                unsigned char page;
                unsigned char length;
        } head;
        unsigned char buf[0xff];
}rtl_reg_debug;
#if 0
typedef struct _CHANNEL_LIST{
    u8  Channel[MAX_CHANNEL_NUMBER + 1];
    u8  Len;
}CHANNEL_LIST, *PCHANNEL_LIST;
#endif
#if 0

typedef struct tx_pendingbuf
{
    struct ieee80211_txb *txb;
    short ispending;
    short descfrag;
} tx_pendigbuf;

#endif


typedef enum{
    LOW_PRIORITY ,
    NORM_PRIORITY
    } priority_t;

typedef enum{
    NIC_8187 = 1,
    NIC_8187B
    } nic_t;

typedef struct Stats
{
    unsigned long txrdu;
    unsigned long rxok;
    unsigned long rxurberr;
    unsigned long rxstaterr;
    unsigned long txnperr;
    unsigned long txnpdrop;
    unsigned long txresumed;
    unsigned long rxerr;
    unsigned long rxint;
    unsigned long txnpokint;
    unsigned long txhpokint;
    unsigned long txhperr;
    unsigned long txoverflow;
    unsigned long txok;
    unsigned long txerr;
    unsigned long txretry;
    unsigned long txbeaconerr;
    unsigned long txbeaconok;
    unsigned long txlpokint;
    unsigned long txlpdrop;
    unsigned long txlperr;
    unsigned long txdatapkt;
    unsigned long rxretry;
    unsigned long rxcrcerrmin;
    unsigned long rxcrcerrmid;
    unsigned long rxcrcerrmax;
    unsigned long rxicverr;

    unsigned long txbeaconokint;
    unsigned long txbeacondrop;
} Stats;

typedef struct  ChnlAccessSetting {
    u16 SIFS_Timer;
    u16 DIFS_Timer;
    u16 SlotTimeTimer;
    u16 EIFS_Timer;
    u16 CWminIndex;
    u16 CWmaxIndex;
}*PCHANNEL_ACCESS_SETTING,CHANNEL_ACCESS_SETTING;

typedef struct tx_urb_context {
    struct urb *tx_urb;
    u8  *transfer_buffer;
    u8  *ptalign_buf;
}tx_urb_context;

typedef enum _RT_RF_POWER_STATE
{
    eRfOn,
    eRfSleep,
    eRfOff
}RT_RF_POWER_STATE;
typedef enum _RT_PS_MODE
{
    eActive,
    eMaxPs,
    eFastPs
}RT_PS_MODE;
#define IC_DEFAULT_THREE_WIRE    0
#define SW_THREE_WIRE        1
#define SW_THREE_WIRE_BY_8051    2
#define HW_THREE_WIRE        3
#define HW_THREE_WIRE_BY_8051    4

typedef struct r8180_priv
{
    struct usb_device *udev;
    short epromtype;
    int irq;
    struct ieee80211_device *ieee80211;
    struct net_device *dev;

    short card_8187;
    short card_8187_Bversion;
    short card_8185;
    short card_8185_Bversion;
    short phy_ver;
    short enable_gpio0;
    enum card_type {PCI,MINIPCI,CARDBUS,USB}card_type;
    short hw_plcp_len;
    short plcp_preamble_mode;


    spinlock_t tx_lock;
    spinlock_t ps_lock;
    spinlock_t rf_ps_lock;

    u16 irq_mask;
    short chan;
    short sens;
    short max_sens;
    u8 chtxpwr[15];
    u8 chtxpwr_ofdm[15];
    u8 cck_txpwr_base;
    u8 ofdm_txpwr_base;
    u8 challow[15];
    u8 channel_plan;

    short up;
    short crcmon;
    struct timer_list ps_timer;
    short PS_timeout;

    struct semaphore wx_sem;
    struct semaphore power_sem;

    short rf_chip;
    short (*rf_set_sens)(struct net_device *dev,short sens);
    void (*rf_set_chan)(struct net_device *dev,short ch);
    void (*rf_close)(struct net_device *dev);
    void (*rf_init)(struct net_device *dev);
    void (*rf_wakeup)(struct net_device *dev);
    void (*rf_sleep)(struct net_device *dev);
    short promisc;
    struct Stats stats;
    struct iw_statistics wstats;
    struct proc_dir_entry *dir_dev;

    struct urb **rx_urb;
    struct tx_urb_context tx_context[MAX_TX_URB];
#ifdef THOMAS_BEACON
    u32 *oldaddr;
#endif
#ifdef THOMAS_TASKLET
    atomic_t irt_counter;
#endif
#ifdef JACKSON_NEW_RX
        struct sk_buff **pp_rxskb;
        int     rx_inx;
#endif
    short  tx_urb_index;




    atomic_t tx_lp_pending;
    atomic_t tx_np_pending;
#if 0
    u32 *txlpring;
    u32 *txhpring;
    u32 *txnpring;
    dma_addr_t txlpringdma;
    dma_addr_t txhpringdma;
    dma_addr_t txnpringdma;
    u32 *txlpringtail;
    u32 *txhpringtail;
    u32 *txnpringtail;
    u32 *txlpringhead;
    u32 *txhpringhead;
    u32 *txnpringhead;
    struct buffer *txlpbufs;
    struct buffer *txhpbufs;
    struct buffer *txnpbufs;
    struct buffer *txlpbufstail;
    struct buffer *txhpbufstail;
    struct buffer *txnpbufstail;
    int txringcount;
    int txbuffsize;

    struct tasklet_struct irq_tx_tasklet;
#endif
    struct tasklet_struct irq_rx_tasklet;
    struct urb *rxurb_task;

#if 0
    u32 *txbeacontail;
    dma_addr_t txbeaconringdma;
    u32 *txbeaconring;
    int txbeaconcount;
#endif

    u16 ShortRetryLimit;
    u16 LongRetryLimit;
    u32 TransmitConfig;
    u8  RegCWinMin;

    u16 EarlyRxThreshold;
    u32 ReceiveConfig;
    u8  AcmControl;

    u8  RFProgType;

    u8 retry_data;
    u8 retry_rts;
    u16 rts;
    u8 ps_sleep_finish;

        u8                      antenna_flag;
        bool                    flag_beacon;
    u8      ScanAllowedNow;
    struct timer_list rateadapter_timer;
    u16                                 LastRetryCnt;
        u16                                     LastRetryRate;
        unsigned long           LastTxokCnt;
        unsigned long           LastRxokCnt;
        u16                                     CurrRetryCnt;
        unsigned long           LastTxOKBytes;
        u8                                      LastFailTxRate;
        long                            LastFailTxRateSS;
        u8                                      FailTxRateCount;
        u32                             LastTxThroughput;
        unsigned long txokbytestotal;
        unsigned short          bTryuping;
        u8                                      CurrTxRate;
        u16                                     CurrRetryRate;
        u16                                     TryupingCount;
        u8                                      TryDownCountLowData;
        u8                                      TryupingCountNoData;

        u8                  CurrentOperaRate;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    struct work_struct rtl8180_hw_wakeup_wq;
#else
    struct tq_struct rtl8180_hw_wakeup_wq;
#endif
    short ack_tx_to_ieee;

#ifdef POLLING_METHOD_FOR_RADIO
    u8 iwpriv_radio_on;
    u8 wlan_first_up_flag1;
    struct timer_list gpio_polling_timer;
#endif
    struct timer_list watch_dog_timer;
    bool bInactivePs;
    bool bSwRfProcessing;
    RT_RF_POWER_STATE eInactivePowerState;
    RT_RF_POWER_STATE eRFPowerState;
    u32 RfOffReason;
    bool RFChangeInProgress;
    bool bInHctTest;
    bool SetRFPowerStateInProgress;
    bool bLeisurePs;
    RT_PS_MODE dot11PowerSaveMode;
    u32 NumRxOkInPeriod;
    u32 NumTxOkInPeriod;
    u8 RegThreeWireMode;
    bool ps_mode;
    bool bDigMechanism;
    bool bCCKThMechanism;
    u8   InitialGain;
    u8   StageCCKTh;
    u8   RegBModeGainStage;
    u8   RegDigOfdmFaUpTh;
    u8   DIG_NumberFallbackVote;
    u8   DIG_NumberUpgradeVote;
    u16  CCKUpperTh;
    u16  CCKLowerTh;
    u32  FalseAlarmRegValue;
    int  UndecoratedSmoothedSS;
    bool bRegHighPowerMechanism;
    bool bToUpdateTxPwr;
    u8   Z2HiPwrUpperTh;
    u8   Z2HiPwrLowerTh;
    u8   Z2RSSIHiPwrUpperTh;
    u8   Z2RSSIHiPwrLowerTh;
    u8   CurCCKRSSI;
    bool bCurCCKPkt;
    u32  wMacRegRfPinsOutput;
    u32  wMacRegRfPinsSelect;
    TR_SWITCH_STATE TrSwitchState;
    u8   radion;
    struct  ChnlAccessSetting  ChannelAccessSetting;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    struct work_struct reset_wq;
#else
    struct tq_struct reset_wq;
#endif


#ifdef LED
    u8      EEPROMCustomerID;
    RT_CID_TYPE CustomerID;
    LED_8187    Gpio0Led;
    LED_8187    SwLed0;
    LED_8187    SwLed1;
    u8          bEnableLedCtrl;
    LED_STRATEGY_8187   LedStrategy;
    u8          PsrValue;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    struct work_struct      Gpio0LedWorkItem;
    struct work_struct      SwLed0WorkItem;
    struct work_struct      SwLed1WorkItem;
#else
    struct tq_struct        Gpio0LedWorkItem;
    struct tq_struct        SwLed0WorkItem;
    struct tq_struct        SwLed1WorkItem;
#endif
#endif

    bool            bAntennaDiversityTimerIssued;
    short antb;
    short diversity;
    bool    AutoloadFailFlag;
    u16 EEPROMVersion;
    u8                  EEPROMAntennaDiversity;
    u16                 EEPROMCSThreshold;
    u8                  EEPROMDefaultAntennaB;
    u8                  EEPROMDigitalPhy;
    u32                 EEPROMCSMethod;
    u8                  EEPROMGEPRFOffState;
    u32         AdMainAntennaRxOkCnt;
    u32         AdAuxAntennaRxOkCnt;
    bool            bHWAdSwitched;
    u8 EEPROMSwAntennaDiversity;
    bool EEPROMDefaultAntenna1;
    u8 RegSwAntennaDiversityMechanism;
    bool bSwAntennaDiverity;
    u8 RegDefaultAntenna;
    bool bDefaultAntenna1;
    u8 SignalStrength;
    long Stats_SignalStrength;
    long LastSignalStrengthInPercent;
    u8   SignalQuality;
    long Stats_SignalQuality;
    long RecvSignalPower;
    long Stats_RecvSignalPower;
    u8   LastRxPktAntenna;
    u32 AdRxOkCnt;
    long AdRxSignalStrength;
    u8 CurrAntennaIndex;
    u8 AdTickCount;
    u8 AdCheckPeriod;
    u8 AdMinCheckPeriod;
    u8 AdMaxCheckPeriod;
    long AdRxSsThreshold;
    long AdMaxRxSsThreshold;
    bool bAdSwitchedChecking;
    long AdRxSsBeforeSwitched;
    struct timer_list SwAntennaDiversityTimer;

}r8180_priv;

#ifdef JOHN_HWSEC
struct ssid_thread {
    struct net_device *dev;
        u8 name[IW_ESSID_MAX_SIZE + 1];
};
#endif

short rtl8180_tx(struct net_device *dev,u8* skbuf, int len,priority_t priority,short morefrag,short rate);

#ifdef JOHN_TKIP
u32 read_cam(struct net_device *dev, u8 addr);
void write_cam(struct net_device *dev, u8 addr, u32 data);
#endif
u8 read_nic_byte(struct net_device *dev, int x);
u8 read_nic_byte_E(struct net_device *dev, int x);
u32 read_nic_dword(struct net_device *dev, int x);
u16 read_nic_word(struct net_device *dev, int x) ;
void write_nic_byte(struct net_device *dev, int x,u8 y);
void write_nic_byte_E(struct net_device *dev, int x,u8 y);
void write_nic_word(struct net_device *dev, int x,u16 y);
void write_nic_dword(struct net_device *dev, int x,u32 y);
void force_pci_posting(struct net_device *dev);

void rtl8180_rtx_disable(struct net_device *);
void rtl8180_rx_enable(struct net_device *);
void rtl8180_tx_enable(struct net_device *);

void rtl8180_disassociate(struct net_device *dev);
void rtl8185_set_rf_pins_enable(struct net_device *dev,u32 a);

void rtl8180_set_anaparam(struct net_device *dev,u32 a);
void rtl8185_set_anaparam2(struct net_device *dev,u32 a);
void rtl8180_update_msr(struct net_device *dev);
int rtl8180_down(struct net_device *dev);
int rtl8180_up(struct net_device *dev);
void rtl8180_commit(struct net_device *dev);
void rtl8180_set_chan(struct net_device *dev,short ch);
void write_phy(struct net_device *dev, u8 adr, u8 data);
void write_phy_cck(struct net_device *dev, u8 adr, u32 data);
void write_phy_ofdm(struct net_device *dev, u8 adr, u32 data);
void rtl8185_tx_antenna(struct net_device *dev, u8 ant);
void rtl8187_set_rxconf(struct net_device *dev);
bool MgntActSet_RF_State(struct net_device *dev,RT_RF_POWER_STATE StateToSet,u32 ChangeSource);
void IPSEnter(struct net_device *dev);
void IPSLeave(struct net_device *dev);
bool SetRFPowerState(struct net_device *dev,RT_RF_POWER_STATE eRFPowerState);
void rtl8187_usb_deleteendpoints(struct net_device *dev);

#ifdef JOHN_TKIP
void EnableHWSecurityConfig8187(struct net_device *dev);
void setKey(struct net_device *dev, u8 EntryNo, u8 KeyIndex, u16 KeyType, u8 *MacAddr, u8 DefaultKey, u32 *KeyContent );

#endif

#endif
