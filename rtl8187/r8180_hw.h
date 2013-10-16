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


#ifndef R8180_HW
#define R8180_HW

#define RF_CHANGE_BY_SW BIT31
#define RF_CHANGE_BY_HW BIT30
#define RF_CHANGE_BY_PS BIT29
#define RF_CHANGE_BY_IPS BIT28
#define ANAPARM_ASIC_ON    0x45090658
#define ANAPARM2_ASIC_ON  0x727f3f52

#define ANAPARM_ON ANAPARM_ASIC_ON
#define ANAPARM2_ON ANAPARM2_ASIC_ON
#define TFPC			0x5C			
#define Config4_PowerOff		BIT6			
#define ANAPARM_OFF 0x51480658
#define ANAPARM2_OFF 0x72003f70

typedef	enum _RF_TYPE_8187{
	RF_TYPE_MIN,
	RF_ZEBRA = 5,
	RF_ZEBRA2,		
	RF_TYPE_MAX,
}RF_TYPE_8187,*PRF_TYPE_8187;


typedef enum _VERSION_8187{
	VERSION_8187_B, 
	VERSION_8187_D, 
	VERSION_8187B_B, 
	VERSION_8187B_D,  
	VERSION_8187B_E,  
}VERSION_8187,*PVERSION_8187;

#define GPO				0xFF90
#define GPE				0xFF91
#define GPI				0xFF92

#define MAX_SLEEP_TIME (10000000) 
#define MIN_SLEEP_TIME (50000)

#define	RTL8187_RF_INDEX	0x8225
#define	RTL8187_REQT_READ	0xc0
#define	RTL8187_REQT_WRITE	0x40
#define	RTL8187_REQ_GET_REGS	0x05
#define	RTL8187_REQ_SET_REGS	0x05



#define MAX_TX_URB 16
#define MAX_RX_URB 1
#define RX_URB_SIZE 0x9c4
#define TX_URB_SIZE 0x9c4





#define BB_ANTATTEN_CHAN14	0x0c
#define BB_ANTENNA_B 0x40

#define BB_HOST_BANG (1<<30)
#define BB_HOST_BANG_EN (1<<2)
#define BB_HOST_BANG_CLK (1<<1)
#define BB_HOST_BANG_RW (1<<3)
#define BB_HOST_BANG_DATA	 1

#define ANAPARAM_TXDACOFF_SHIFT 27
#define ANAPARAM_PWR0_MASK ((1<<30)|(1<<29)|(1<<28))
#define ANAPARAM_PWR0_SHIFT 28
#define ANAPARAM_PWR1_MASK ((1<<26)|(1<<25)|(1<<24)|(1<<23)|(1<<22)|(1<<21)|(1<<20))
#define ANAPARAM_PWR1_SHIFT 20

#define MAC0 0
#define MAC1 1
#define MAC2 2
#define MAC3 3
#define MAC4 4
#define MAC5 5

#define RXFIFOCOUNT 0x10
#define TXFIFOCOUNT 0x12
#define BcnIntTime 0x74
#define TALLY_SEL 0xfc
#define BQREQ 0x13

#define CMD 0x37
#define CMD_RST_SHIFT 4
#define CMD_RESERVED_MASK ((1<<1) | (1<<5) | (1<<6) | (1<<7))
#define CMD_RX_ENABLE_SHIFT 3
#define CMD_TX_ENABLE_SHIFT 2

#define EPROM_CMD 0x50
#define EPROM_CMD_RESERVED_MASK ((1<<5)|(1<<4))
#define EPROM_CMD_OPERATING_MODE_SHIFT 6
#define EPROM_CMD_OPERATING_MODE_MASK ((1<<7)|(1<<6))
#define EPROM_CMD_CONFIG 0x3
#define EPROM_CMD_NORMAL 0 
#define EPROM_CMD_LOAD 1
#define EPROM_CMD_PROGRAM 2
#define EPROM_CS_SHIFT 3
#define EPROM_CK_SHIFT 2
#define EPROM_W_SHIFT 1
#define EPROM_R_SHIFT 0
#define CONFIG2_DMA_POLLING_MODE_SHIFT 3
#define INTA 0x3e
#define INTA_TXOVERFLOW (1<<15)
#define INTA_TIMEOUT (1<<14)
#define INTA_BEACONTIMEOUT (1<<13)
#define INTA_ATIM (1<<12)
#define INTA_BEACONDESCERR (1<<11)
#define INTA_BEACONDESCOK (1<<10)
#define INTA_HIPRIORITYDESCERR (1<<9)
#define INTA_HIPRIORITYDESCOK (1<<8)
#define INTA_NORMPRIORITYDESCERR (1<<7)
#define INTA_NORMPRIORITYDESCOK (1<<6)
#define INTA_RXOVERFLOW (1<<5)
#define INTA_RXDESCERR (1<<4)
#define INTA_LOWPRIORITYDESCERR (1<<3)
#define INTA_LOWPRIORITYDESCOK (1<<2)
#define INTA_RXCRCERR (1<<1)
#define INTA_RXOK (1)
#define INTA_MASK 0x3c
#define RXRING_ADDR 0xe4 
#define PGSELECT 0x5e
#define PGSELECT_PG_SHIFT 0
#define RX_CONF 0x44
#define MAC_FILTER_MASK ((1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<5) | \
(1<<12) | (1<<18) | (1<<19) | (1<<20) | (1<<21) | (1<<22) | (1<<23))
#define RX_CHECK_BSSID_SHIFT 23
#define ACCEPT_PWR_FRAME_SHIFT 22
#define ACCEPT_MNG_FRAME_SHIFT 20
#define ACCEPT_CTL_FRAME_SHIFT 19
#define ACCEPT_DATA_FRAME_SHIFT 18
#define ACCEPT_ICVERR_FRAME_SHIFT 12
#define ACCEPT_CRCERR_FRAME_SHIFT 5
#define ACCEPT_BCAST_FRAME_SHIFT 3
#define ACCEPT_MCAST_FRAME_SHIFT 2
#define ACCEPT_ALLMAC_FRAME_SHIFT 0
#define ACCEPT_NICMAC_FRAME_SHIFT 1
#define RX_FIFO_THRESHOLD_MASK ((1<<13) | (1<<14) | (1<<15))
#define RX_FIFO_THRESHOLD_SHIFT 13
#define RX_FIFO_THRESHOLD_128 3
#define RX_FIFO_THRESHOLD_256 4
#define RX_FIFO_THRESHOLD_512 5
#define RX_FIFO_THRESHOLD_1024 6
#define RX_FIFO_THRESHOLD_NONE 7
#define RX_AUTORESETPHY_SHIFT 28
#define EPROM_TYPE_SHIFT 6
#define TX_CONF 0x40
#define TX_CONF_HEADER_AUTOICREMENT_SHIFT 30
#define TX_LOOPBACK_SHIFT 17
#define TX_LOOPBACK_MAC 1
#define TX_LOOPBACK_BASEBAND 2
#define TX_LOOPBACK_NONE 0
#define TX_LOOPBACK_CONTINUE 3
#define TX_LOOPBACK_MASK ((1<<17)|(1<<18))
#define TX_DPRETRY_SHIFT 0
#define R8180_MAX_RETRY 255
#define TX_RTSRETRY_SHIFT 8
#define TX_NOICV_SHIFT 19
#define TX_NOCRC_SHIFT 16
#define TX_DMA_POLLING 0xd9
#define TX_DMA_POLLING_BEACON_SHIFT 7
#define TX_DMA_POLLING_HIPRIORITY_SHIFT 6
#define TX_DMA_POLLING_NORMPRIORITY_SHIFT 5
#define TX_DMA_POLLING_LOWPRIORITY_SHIFT 4
#define TX_DMA_STOP_BEACON_SHIFT 3
#define TX_DMA_STOP_HIPRIORITY_SHIFT 2
#define TX_DMA_STOP_NORMPRIORITY_SHIFT 1
#define TX_DMA_STOP_LOWPRIORITY_SHIFT 0
#define TX_NORMPRIORITY_RING_ADDR 0x24
#define TX_HIGHPRIORITY_RING_ADDR 0x28
#define TX_LOWPRIORITY_RING_ADDR 0x20
#define MAX_RX_DMA_MASK ((1<<8) | (1<<9) | (1<<10))
#define MAX_RX_DMA_2048 7
#define MAX_RX_DMA_1024	6
#define MAX_RX_DMA_SHIFT 10
#define INT_TIMEOUT 0x48
#define CONFIG3_CLKRUN_SHIFT 2
#define CONFIG3_ANAPARAM_W_SHIFT 6
#define ANAPARAM 0x54
#define BEACON_INTERVAL 0x70
#define BEACON_INTERVAL_MASK ((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)| \
(1<<6)|(1<<7)|(1<<8)|(1<<9))
#define ATIM_MASK ((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7)| \
(1<<8)|(1<<9))
#define ATIM 0x72
#define EPROM_CS_SHIFT 3
#define EPROM_CK_SHIFT 2
#define PHY_DELAY 0x78
#define PHY_CONFIG 0x80
#define PHY_ADR 0x7c
#define PHY_READ 0x7e
#define CARRIER_SENSE_COUNTER 0x79 
#define SECURITY 0x5f
#define SECURITY_WEP_TX_ENABLE_SHIFT 1
#define SECURITY_WEP_RX_ENABLE_SHIFT 0
#define SECURITY_ENCRYP_104 1
#define SECURITY_ENCRYP_SHIFT 4
#define SECURITY_ENCRYP_MASK ((1<<4)|(1<<5))
#define KEY0 0x90
#define CONFIG2_ANTENNA_SHIFT 6
#define TX_BEACON_RING_ADDR 0x4c
#define CONFIG0_WEP40_SHIFT 7
#define CONFIG0_WEP104_SHIFT 6
#define AGCRESET_SHIFT 5

#ifdef SW_ANTE_DIVERSITY
#define RF_PARAM 0x19
#define RF_PARAM_DIGPHY_SHIFT 0
#define RF_PARAM_ANTBDEFAULT_SHIFT 1
#define EEPROM_VERSION                                  0x3c
#define EEPROM_CONFIG2                                  0x18
#define EEPROM_CS_THRESHOLD                             0x2F
#define EEPROM_RF_PARAM                         0x08
#define EEPROM_SW_AD_MASK                       0x0300
#define EEPROM_SW_AD_ENABLE                     0x0100
#define EEPROM_DEF_ANT_MASK                     0x0C00
#define EEPROM_DEF_ANT_1                        0x0400

#define RCR_EnCS1			BIT29				
#define RCR_EnCS2			BIT30				
#endif


#define IDR0 0x0000
#define IDR1 0x0001
#define IDR2 0x0002
#define IDR3 0x0003
#define IDR4 0x0004
#define IDR5 0x0005


#define MAR0 0x0008
#define MAR1 0x0009
#define MAR2 0x000A
#define MAR3 0x000B
#define MAR4 0x000C
#define MAR5 0x000D
#define MAR6 0x000E
#define MAR7 0x000F


#define TSFTR 0x0018
#define TSFTR_END 0x001F

#define TLPDA 0x0020
#define TLPDA_END 0x0023
#define TNPDA 0x0024
#define TNPDA_END 0x0027
#define THPDA 0x0028
#define THPDA_END 0x002B

#define BRSR 0x002C
#define BRSR_END 0x002D

#define BSSID 0x002E
#define BSSID_END 0x0033


#define CR 0x0037


#define IMR 0x003C
#define IMR_END 0x003D

#define ISR 0x003E
#define ISR_END 0x003F

#define TCR 0x0040
#define TCR_END 0x0043

#define RCR 0x0044
#define RCR_END 0x0047

#define TimerInt 0x0048
#define TimerInt_END 0x004B

#define TBDA 0x004C
#define TBDA_END 0x004F

#define CR9346 0x0050

#define CONFIG0 0x0051
#define CONFIG1 0x0052
#define CONFIG2 0x0053

#define ANA_PARM 0x0054
#define ANA_PARM_END 0x0x0057

#define MSR 0x0058

#define CONFIG3 0x0059
#define CONFIG4 0x005A

#define TESTR 0x005B


#define PSR 0x005E

#define SCR 0x005F


#define BcnItv 0x0070
#define BcnItv_END 0x0071

#define AtimWnd 0x0072
#define AtimWnd_END 0x0073

#define BintrItv 0x0074
#define BintrItv_END 0x0075

#define AtimtrItv 0x0076
#define AtimtrItv_END 0x0077

#define PhyDelay 0x0078

#define CRCount 0x0079


#define PhyAddr 0x007C
#define PhyDataW 0x007D
#define PhyDataR 0x007E

#define PhyCFG 0x0080
#define PhyCFG_END 0x0083

#define RFPinsOutput 0x80
#define RFPinsEnable 0x82
#define RF_TIMING 0x8c 
#define RFPinsSelect 0x84
#define ANAPARAM2 0x60
#define RF_PARA 0x88
#define RFPinsInput 0x86
#define GP_ENABLE 0x90
#define GPIO 0x91
#define SW_CONTROL_GPIO 0x400
#define TX_ANTENNA 0x9f
#define TX_GAIN_OFDM 0x9e
#define TX_GAIN_CCK 0x9d
#define WPA_CONFIG 0xb0
#define TX_AGC_CTL 0x9c
#define TX_AGC_CTL_PERPACKET_GAIN_SHIFT 0
#define TX_AGC_CTL_PERPACKET_ANTSEL_SHIFT 1
#define TX_AGC_CTL_FEEDBACK_ANT 2
#define RESP_RATE 0x34
#ifdef SW_ANTE_DIVERSITY
#define TXAGC_CTL		0x9C
#define CCK_TXAGC		0x9D
#define OFDM_TXAGC		0x9E
#define ANTSEL			0x9F
#endif
#define SIFS 0xb4
#define DIFS 0xb5
#define EIFS 0x35
#define SLOT 0xb6
#define CW_VAL 0xbd
#define CW_CONF 0xbc
#define CW_CONF_PERPACKET_RETRY_SHIFT 1
#define CW_CONF_PERPACKET_CW_SHIFT 0
#define MAX_RESP_RATE_SHIFT 4
#define MIN_RESP_RATE_SHIFT 0
#define RATE_FALLBACK 0xbe

#define Wakeup0 0x0084
#define Wakeup0_END 0x008B

#define Wakeup1 0x008C
#define Wakeup1_END 0x0093

#define Wakeup2LD 0x0094
#define Wakeup2LD_END 0x009B
#define Wakeup2HD 0x009C
#define Wakeup2HD_END 0x00A3

#define Wakeup3LD 0x00A4
#define Wakeup3LD_END 0x00AB
#define Wakeup3HD 0x00AC
#define Wakeup3HD_END 0x00B3

#define Wakeup4LD 0x00B4
#define Wakeup4LD_END 0x00BB
#define Wakeup4HD 0x00BC
#define Wakeup4HD_END 0x00C3

#define CRC0 0x00C4
#define CRC0_END 0x00C5
#define CRC1 0x00C6
#define CRC1_END 0x00C7
#define CRC2 0x00C8
#define CRC2_END 0x00C9
#define CRC3 0x00CA
#define CRC3_END 0x00CB
#define CRC4 0x00CC
#define CRC4_END 0x00CD






#define DK0 0x0090
#define DK0_END 0x009F
#define DK1 0x00A0
#define DK1_END 0x00AF
#define DK2 0x00B0
#define DK2_END 0x00BF
#define DK3 0x00C0
#define DK3_END 0x00CF

#define CCK_FALSE_ALARM         0x00D0
#define OFDM_FALSE_ALARM        0x00D2


#define CONFIG5 0x00D8

#define TPPoll 0x00D9


#define CWR 0x00DC
#define CWR_END 0x00DD

#define RetryCTR 0x00DE


#define RDSAR 0x00E4
#define RDSAR_END 0x00E7



#define FER 0x00F0
#define FER_END 0x00F3

#define FEMR 0x00F4
#define FEMR_END 0x00F7

#define FPSR 0x00F8
#define FPSR_END 0x00FB

#define FFER 0x00FC
#define FFER_END 0x00FF




#define BRSR_BPLCP  ((1<< 8))
#define BRSR_MBR    ((1<< 1)|(1<< 0))
#define BRSR_MBR_8185 ((1<< 11)|(1<< 10)|(1<< 9)|(1<< 8)|(1<< 7)|(1<< 6)|(1<< 5)|(1<< 4)|(1<< 3)|(1<< 2)|(1<< 1)|(1<< 0))
#define BRSR_MBR0   ((1<< 0))
#define BRSR_MBR1   ((1<< 1))

#define CR_RST      ((1<< 4))
#define CR_RE       ((1<< 3))
#define CR_TE       ((1<< 2))
#define CR_MulRW    ((1<< 0))

#define IMR_TXFOVW  ((1<<15))
#define IMR_TimeOut ((1<<14))
#define IMR_BcnInt  ((1<<13))
#define IMR_ATIMInt ((1<<12))
#define IMR_TBDER   ((1<<11))
#define IMR_TBDOK   ((1<<10))
#define IMR_THPDER  ((1<< 9))
#define IMR_THPDOK  ((1<< 8))
#define IMR_TNPDER  ((1<< 7))
#define IMR_TNPDOK  ((1<< 6))
#define IMR_RXFOVW  ((1<< 5))
#define IMR_RDU     ((1<< 4))
#define IMR_TLPDER  ((1<< 3))
#define IMR_TLPDOK  ((1<< 2))
#define IMR_RER     ((1<< 1))
#define IMR_ROK     ((1<< 0))

#define ISR_TXFOVW  ((1<<15))
#define ISR_TimeOut ((1<<14))
#define ISR_BcnInt  ((1<<13))
#define ISR_ATIMInt ((1<<12))
#define ISR_TBDER   ((1<<11))
#define ISR_TBDOK   ((1<<10))
#define ISR_THPDER  ((1<< 9))
#define ISR_THPDOK  ((1<< 8))
#define ISR_TNPDER  ((1<< 7))
#define ISR_TNPDOK  ((1<< 6))
#define ISR_RXFOVW  ((1<< 5))
#define ISR_RDU     ((1<< 4))
#define ISR_TLPDER  ((1<< 3))
#define ISR_TLPDOK  ((1<< 2))
#define ISR_RER     ((1<< 1))
#define ISR_ROK     ((1<< 0))

#define HW_VERID_R8180_F 3
#define HW_VERID_R8180_ABCD 2
#define HW_VERID_R8185_ABC 4
#define HW_VERID_R8185_D 5

#define TCR_CWMIN   ((1<<31))
#define TCR_SWSEQ   ((1<<30))
#define TCR_HWVERID_MASK ((1<<27)|(1<<26)|(1<<25))
#define TCR_HWVERID_SHIFT 25
#define TCR_SWPLCPLEN     ((1<<24))
#define TCR_PLCP_LEN TCR_SAT 
#define TCR_MXDMA_MASK   ((1<<23)|(1<<22)|(1<<21)) 
#define TCR_MXDMA_1024 6
#define TCR_MXDMA_2048 7
#define TCR_MXDMA_SHIFT  21
#define TCR_DISCW   ((1<<20))
#define TCR_ICV     ((1<<19))
#define TCR_LBK     ((1<<18)|(1<<17))
#define TCR_LBK1    ((1<<18))
#define TCR_LBK0    ((1<<17))
#define TCR_CRC     ((1<<16))
#define TCR_DPRETRY_MASK   ((1<<15)|(1<<14)|(1<<13)|(1<<12)|(1<<11)|(1<<10)|(1<<9)|(1<<8))
#define TCR_RTSRETRY_MASK   ((1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7))
#define TCR_PROBE_NOTIMESTAMP_SHIFT 29 

#define RCR_ONLYERLPKT ((1<<31))
#define RCR_CS_SHIFT   29
#define RCR_CS_MASK    ((1<<30) | (1<<29))
#define RCR_ENMARP     ((1<<28))
#define RCR_CBSSID     ((1<<23))
#define RCR_APWRMGT    ((1<<22))
#define RCR_ADD3       ((1<<21))
#define RCR_AMF        ((1<<20))
#define RCR_ACF        ((1<<19))
#define RCR_ADF        ((1<<18))
#define RCR_RXFTH      ((1<<15)|(1<<14)|(1<<13))
#define RCR_RXFTH2     ((1<<15))
#define RCR_RXFTH1     ((1<<14))
#define RCR_RXFTH0     ((1<<13))
#define RCR_AICV       ((1<<12))
#define RCR_MXDMA      ((1<<10)|(1<< 9)|(1<< 8))
#define RCR_MXDMA2     ((1<<10))
#define RCR_MXDMA1     ((1<< 9))
#define RCR_MXDMA0     ((1<< 8))
#define RCR_9356SEL    ((1<< 6))
#define RCR_ACRC32     ((1<< 5))
#define RCR_AB         ((1<< 3))
#define RCR_AM         ((1<< 2))
#define RCR_APM        ((1<< 1))
#define RCR_AAP        ((1<< 0))



#define CR9346_EEM     ((1<<7)|(1<<6))
#define CR9346_EEM1    ((1<<7))
#define CR9346_EEM0    ((1<<6))
#define CR9346_EECS    ((1<<3))
#define CR9346_EESK    ((1<<2))
#define CR9346_EED1    ((1<<1))
#define CR9346_EED0    ((1<<0))

#define CONFIG0_WEP104     ((1<<6))
#define CONFIG0_LEDGPO_En  ((1<<4))
#define CONFIG0_Aux_Status ((1<<3))
#define CONFIG0_GL         ((1<<1)|(1<<0))
#define CONFIG0_GL1        ((1<<1))
#define CONFIG0_GL0        ((1<<0))

#define CONFIG1_LEDS       ((1<<7)|(1<<6))
#define CONFIG1_LEDS1      ((1<<7))
#define CONFIG1_LEDS0      ((1<<6))
#define CONFIG1_LWACT      ((1<<4))
#define CONFIG1_MEMMAP     ((1<<3))
#define CONFIG1_IOMAP      ((1<<2))
#define CONFIG1_VPD        ((1<<1))
#define CONFIG1_PMEn       ((1<<0))

#define CONFIG2_LCK        ((1<<7))
#define CONFIG2_ANT        ((1<<6))
#define CONFIG2_DPS        ((1<<3))
#define CONFIG2_PAPE_sign  ((1<<2))
#define CONFIG2_PAPE_time  ((1<<1)|(1<<0))
#define CONFIG2_PAPE_time1 ((1<<1))
#define CONFIG2_PAPE_time0 ((1<<0))

#define CONFIG3_GNTSel     ((1<<7))
#define CONFIG3_PARM_En    ((1<<6))
#define CONFIG3_Magic      ((1<<5))
#define CONFIG3_CardB_En   ((1<<3))
#define CONFIG3_CLKRUN_En  ((1<<2))
#define CONFIG3_FuncRegEn  ((1<<1))
#define CONFIG3_FBtbEn     ((1<<0))

#define CONFIG4_VCOPDN     ((1<<7))
#define CONFIG4_PWROFF     ((1<<6))
#define CONFIG4_PWRMGT     ((1<<5))
#define CONFIG4_LWPME      ((1<<4))
#define CONFIG4_LWPTN      ((1<<2))
#define CONFIG4_RFTYPE     ((1<<1)|(1<<0))
#define CONFIG4_RFTYPE1    ((1<<1))
#define CONFIG4_RFTYPE0    ((1<<0))

#define CONFIG5_TX_FIFO_OK ((1<<7))
#define CONFIG5_RX_FIFO_OK ((1<<6))
#define CONFIG5_CALON      ((1<<5))
#define CONFIG5_EACPI      ((1<<2))
#define CONFIG5_LANWake    ((1<<1))
#define CONFIG5_PME_STS    ((1<<0))

#define MSR_LINK_MASK      ((1<<2)|(1<<3))
#define MSR_LINK_MANAGED   2
#define MSR_LINK_NONE      0
#define MSR_LINK_SHIFT     2
#define MSR_LINK_ADHOC     1
#define MSR_LINK_MASTER    3

#define PSR_GPO            ((1<<7))
#define PSR_GPI            ((1<<6))
#define PSR_LEDGPO1        ((1<<5))
#define PSR_LEDGPO0        ((1<<4))
#define PSR_UWF            ((1<<1))
#define PSR_PSEn           ((1<<0))

#define SCR_KM             ((1<<5)|(1<<4))
#define SCR_KM1            ((1<<5))
#define SCR_KM0            ((1<<4))
#define SCR_TXSECON        ((1<<1))
#define SCR_RXSECON        ((1<<0))

#define BcnItv_BcnItv      (0x01FF)

#define AtimWnd_AtimWnd    (0x01FF)

#define BintrItv_BintrItv  (0x01FF)

#define AtimtrItv_AtimtrItv (0x01FF)

#define PhyDelay_PhyDelay  ((1<<2)|(1<<1)|(1<<0))

#define TPPoll_BQ    ((1<<7))
#define TPPoll_HPQ   ((1<<6))
#define TPPoll_NPQ   ((1<<5))
#define TPPoll_LPQ   ((1<<4))
#define TPPoll_SBQ   ((1<<3))
#define TPPoll_SHPQ  ((1<<2))
#define TPPoll_SNPQ  ((1<<1))
#define TPPoll_SLPQ  ((1<<0))

#define CWR_CW       (0x01FF)

#define FER_INTR     ((1<<15))
#define FER_GWAKE    ((1<< 4))

#define FEMR_INTR    ((1<<15))
#define FEMR_WKUP    ((1<<14))
#define FEMR_GWAKE   ((1<< 4))

#define FPSR_INTR    ((1<<15))
#define FPSR_GWAKE   ((1<< 4))

#define FFER_INTR    ((1<<15))
#define FFER_GWAKE   ((1<< 4))

#define Tally_Cnt    0xFFFA
#define TR_SW_MASK_TX_8187 BIT5 
#define TR_SW_MASK_RX_8187 BIT6
#define TR_SW_MASK_8187 (TR_SW_MASK_TX_8187 | TR_SW_MASK_RX_8187)

#endif
