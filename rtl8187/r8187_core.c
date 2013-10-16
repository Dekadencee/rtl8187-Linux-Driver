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

#ifndef CONFIG_FORCE_HARD_FLOAT
double __floatsidf (int i) { return i; }
unsigned int __fixunsdfsi (double d) { return d; }
double __adddf3(double a, double b) { return a+b; }
double __addsf3(float a, float b) { return a+b; }
double __subdf3(double a, double b) { return a-b; }
double __extendsfdf2(float a) {return a;}
#endif

#undef LOOP_TEST
#undef DUMP_RX
#undef DUMP_TX
#undef DEBUG_TX_DESC2
#undef RX_DONT_PASS_UL
#undef DEBUG_EPROM
#undef DEBUG_RX_VERBOSE
#undef DUMMY_RX
#undef DEBUG_ZERO_RX
#undef DEBUG_RX_SKB
#undef DEBUG_TX_FRAG
#undef DEBUG_RX_FRAG
#undef DEBUG_TX_FILLDESC
#undef DEBUG_TX
#undef DEBUG_IRQ
#undef DEBUG_RX
#undef DEBUG_RXALLOC
#undef DEBUG_REGISTERS
#undef DEBUG_RING
#undef DEBUG_IRQ_TASKLET
#undef DEBUG_TX_ALLOC
#undef DEBUG_TX_DESC


#include "r8180_hw.h"
#include "r8187.h"
#include "r8180_rtl8225.h"
#include "r8180_93cx6.h"
#include "r8180_wx.h"
#include "r8180_dm.h"


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,7))
#define usb_kill_urb usb_unlink_urb
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
#ifdef CONFIG_PM
#include "r8180_pm.h"
#endif
#endif

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,39))
#define HAVE_NET_DEVICE_OPS
#endif

#ifndef USB_VENDOR_ID_REALTEK
#define USB_VENDOR_ID_REALTEK       0x0bda
#endif
#ifndef USB_VENDOR_ID_NETGEAR
#define USB_VENDOR_ID_NETGEAR       0x0846
#endif

#define IEEE80211_WATCH_DOG_TIME    2000

static struct usb_device_id rtl8187_usb_id_tbl[] = {
    {USB_DEVICE(0x1B75, 0x8187)},
    {USB_DEVICE(USB_VENDOR_ID_REALTEK, 0x8187)},
    {USB_DEVICE(USB_VENDOR_ID_NETGEAR, 0x6100)},
    {USB_DEVICE(USB_VENDOR_ID_NETGEAR, 0x6a00)},

    {}
};

static char* ifname = "wlan%d";
#if 0
static int hwseqnum = 0;
static int hwwep = 0;
#endif
static int channels = 0x3fff;

CHANNEL_LIST    DefaultChannelPlan[] = {
    {{1,2,3,4,5,6,7,8,9,10,11,36,40,44,48,52,56,60,64,149,153,157,161,165},24},
    {{1,2,3,4,5,6,7,8,9,10,11},11},
    {{1,2,3,4,5,6,7,8,9,10,11,12,13,36,40,44,48,52,56,60,64},21},
    {{1,2,3,4,5,6,7,8,9,10,11,12,13},13},
    {{1,2,3,4,5,6,7,8,9,10,11,12,13},13},
    {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,36,40,44,48,52,56,60,64},22},
    {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,36,40,44,48,52,56,60,64},22},
    {{1,2,3,4,5,6,7,8,9,10,11,12,13},13},
    {{1,2,3,4,5,6,7,8,9,10,11,12,13,14,36,40,44,48,52,56,60,64},22},};
#define eqMacAddr(a,b)                                          ( ((a)[0]==(b)[0] && (a)[1]==(b)[1] && (a)[2]==(b)[2] && (a)[3]==(b)[3] && (a)[4]==(b)[4] && (a)[5]==(b)[5]) ? 1:0 )
#define DEFAULT_RATE_ADAPTIVE_TIMER_PERIOD      300
#define IEEE80211_WATCH_DOG_TIME    2000
MODULE_LICENSE("GPL");
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
MODULE_VERSION("V 1.1");
#endif
MODULE_DEVICE_TABLE(usb, rtl8187_usb_id_tbl);
MODULE_AUTHOR("Andrea Merello <andreamrl@tiscali.it>");
MODULE_DESCRIPTION("Linux driver for Realtek RTL8187 WiFi cards");

#if 0
MODULE_PARM(ifname,"s");
MODULE_PARM_DESC(devname," Net interface name, wlan%d=default");

MODULE_PARM(hwseqnum,"i");
MODULE_PARM_DESC(hwseqnum," Try to use hardware 802.11 header sequence numbers. Zero=default");

MODULE_PARM(hwwep,"i");
MODULE_PARM_DESC(hwwep," Try to use hardware WEP support. Still broken and not available on all cards");

MODULE_PARM(channels,"i");
MODULE_PARM_DESC(channels," Channel bitmask for specific locales. NYI");
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 9)
module_param(ifname, charp, S_IRUGO|S_IWUSR );
module_param(channels,int, S_IRUGO|S_IWUSR);
#else
MODULE_PARM(ifname, "s");
MODULE_PARM(channels,"i");
#endif

MODULE_PARM_DESC(devname," Net interface name,ath%d=default");
MODULE_PARM_DESC(channels," Channel bitmask for specific locales. NYI");

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
static int __devinit rtl8187_usb_probe(struct usb_interface *intf,
             const struct usb_device_id *id);
static void __devexit rtl8187_usb_disconnect(struct usb_interface *intf);
#else
static void *__devinit rtl8187_usb_probe(struct usb_device *udev,unsigned int ifnum,
             const struct usb_device_id *id);
static void __devexit rtl8187_usb_disconnect(struct usb_device *udev, void *ptr);
#endif

static struct usb_driver rtl8187_usb_driver = {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 15)
    .owner      = THIS_MODULE,
#endif
    .name       = RTL8187_MODULE_NAME,
    .id_table   = rtl8187_usb_id_tbl,
    .probe      = rtl8187_usb_probe,
    .disconnect = rtl8187_usb_disconnect,
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
#ifdef CONFIG_PM
    .suspend    = rtl8187_suspend,
    .resume     = rtl8187_resume,
#endif
#endif
};


void write_nic_byte_E(struct net_device *dev, int indx, u8 data)
{

    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct usb_device *udev = priv->udev;

    usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
                   RTL8187_REQ_SET_REGS, RTL8187_REQT_WRITE,
                   indx|0xfe00, 0, &data, 1, HZ / 2);
}


void write_nic_byte(struct net_device *dev, int indx, u8 data)
{

    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct usb_device *udev = priv->udev;

    usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
                   RTL8187_REQ_SET_REGS, RTL8187_REQT_WRITE,
                   indx|0xff00, 0, &data, 1, HZ / 2);
}


void write_nic_word(struct net_device *dev, int indx, u16 data)
{

    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct usb_device *udev = priv->udev;

    usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
                   RTL8187_REQ_SET_REGS, RTL8187_REQT_WRITE,
                   indx|0xff00, 0, &data, 2, HZ / 2);
}


void write_nic_dword(struct net_device *dev, int indx, u32 data)
{

    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct usb_device *udev = priv->udev;

    usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
                   RTL8187_REQ_SET_REGS, RTL8187_REQT_WRITE,
                   indx|0xff00, 0, &data, 4, HZ / 2);
}



u8 read_nic_byte(struct net_device *dev, int indx)
{
    u8 data;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct usb_device *udev = priv->udev;

    usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
                   RTL8187_REQ_GET_REGS, RTL8187_REQT_READ,
                   indx|0xff00, 0, &data, 1, HZ / 2);
    return data;
}

u8 read_nic_byte_E(struct net_device *dev, int indx)
{
    u8 data;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct usb_device *udev = priv->udev;

    usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
                   RTL8187_REQ_GET_REGS, RTL8187_REQT_READ,
                   indx|0xfe00, 0, &data, 1, HZ / 2);
    return data;
}


u16 read_nic_word(struct net_device *dev, int indx)
{
    u16 data;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct usb_device *udev = priv->udev;

    usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
                   RTL8187_REQ_GET_REGS, RTL8187_REQT_READ,
                   indx|0xff00, 0, &data, 2, HZ / 2);
    return data;
}


u32 read_nic_dword(struct net_device *dev, int indx)
{
    u32 data;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct usb_device *udev = priv->udev;

    usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
                   RTL8187_REQ_GET_REGS, RTL8187_REQT_READ,
                   indx|0xff00, 0, &data, 4, HZ / 2);
    return data;
}

inline void force_pci_posting(struct net_device *dev)
{
}


static struct net_device_stats *rtl8180_stats(struct net_device *dev);
void rtl8180_commit(struct net_device *dev);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_restart(struct work_struct *work);
void rtl8180_rq_tx_ack(struct work_struct *work);
#else
 void rtl8180_restart(struct net_device *dev);
void rtl8180_rq_tx_ack(struct net_device *dev);
#endif

/****************************************************************************
   -----------------------------PROCFS STUFF-------------------------
*****************************************************************************/

static struct proc_dir_entry *rtl8180_proc = NULL;

static int proc_get_registers(char *page, char **start,
              off_t offset, int count,
              int *eof, void *data)
{
    struct net_device *dev = data;

    int len = 0;
    int i,n;

    int max=0xff;

    for(n=0;n<=max;)
    {
        len += snprintf(page + len, count - len,
            "\nD:  %2x > ",n);

        for(i=0;i<16 && n<=max;i++,n++)
        len += snprintf(page + len, count - len,
            "%2x ",read_nic_byte(dev,n));

    }
    len += snprintf(page + len, count - len,"\n");



    *eof = 1;
    return len;

}

#if 0
static int proc_get_stats_hw(char *page, char **start,
              off_t offset, int count,
              int *eof, void *data)
{
    struct net_device *dev = data;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    int len = 0;

    len += snprintf(page + len, count - len,
        "NIC int: %lu\n"
        "Total int: %lu\n",
        priv->stats.ints,
        priv->stats.shints);

    *eof = 1;
    return len;
}
#endif

static int proc_get_stats_tx(char *page, char **start,
              off_t offset, int count,
              int *eof, void *data)
{
    struct net_device *dev = data;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    int len = 0;
    unsigned long totalOK;
    totalOK=priv->stats.txnpokint+priv->stats.txhpokint+priv->stats.txlpokint;


    len += snprintf(page + len, count - len,
        "TX OK: %lu\n"
        "TX Error: %lu\n"
        "TX Retry: %lu\n"
        "TX beacon OK: %lu\n"
        "TX beacon error: %lu\n",
        priv->stats.txok,
        priv->stats.txerr,
        priv->stats.txretry,
        priv->stats.txbeaconokint,
        priv->stats.txbeaconerr

        );

    *eof = 1;
    return len;
}



static int proc_get_stats_rx(char *page, char **start,
              off_t offset, int count,
              int *eof, void *data)
{
    struct net_device *dev = data;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    int len = 0;

    len += snprintf(page + len, count - len,
        "RX OK: %lu\n"
        "RX Retry: %lu\n"
        "RX CRC Error(0-500): %lu\n"
        "RX CRC Error(500-1000): %lu\n"
        "RX CRC Error(>1000): %lu\n"
        "RX ICV Error: %lu\n",
        priv->stats.rxok,
        priv->stats.rxstaterr,
        priv->stats.rxcrcerrmin,
        priv->stats.rxcrcerrmid,
        priv->stats.rxcrcerrmax,
        priv->stats.rxicverr

    );

    *eof = 1;
    return len;
}

#if WIRELESS_EXT < 17
static struct iw_statistics *r8180_get_wireless_stats(struct net_device *dev)
{
       struct r8180_priv *priv = ieee80211_priv(dev);

       return &priv->wstats;
}
#endif

void rtl8180_proc_module_init(void)
{
    DMESG("Initializing proc filesystem");
#if(LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
    rtl8180_proc=create_proc_entry(RTL8187_MODULE_NAME, S_IFDIR, proc_net);
#else
    rtl8180_proc=create_proc_entry(RTL8187_MODULE_NAME, S_IFDIR, init_net.proc_net);
#endif
}


void rtl8180_proc_module_remove(void)
{
#if(LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
    remove_proc_entry(RTL8187_MODULE_NAME, proc_net);
#else
    remove_proc_entry(RTL8187_MODULE_NAME, init_net.proc_net);
#endif
}


void rtl8180_proc_remove_one(struct net_device *dev)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    printk("=====================>%s\n", __func__);
    if (priv->dir_dev) {
        remove_proc_entry("stats-tx", priv->dir_dev);
        remove_proc_entry("stats-rx", priv->dir_dev);
        remove_proc_entry("registers", priv->dir_dev);
        remove_proc_entry("wlan0", rtl8180_proc);
        priv->dir_dev = NULL;
    }
}


void rtl8180_proc_init_one(struct net_device *dev)
{
    struct proc_dir_entry *e;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    printk("=====================>%s\n", __func__);
    priv->dir_dev = create_proc_entry(dev->name,
                      S_IFDIR | S_IRUGO | S_IXUGO,
                      rtl8180_proc);
    if (!priv->dir_dev) {
        DMESGE("Unable to initialize /proc/net/rtl8187/%s\n",
              dev->name);
        return;
    }
    #if 0
    e = create_proc_read_entry("stats-hw", S_IFREG | S_IRUGO,
                   priv->dir_dev, proc_get_stats_hw, dev);

    if (!e) {
        DMESGE("Unable to initialize "
              "/proc/net/rtl8187/%s/stats-hw\n",
              dev->name);
    }
    #endif
    e = create_proc_read_entry("stats-rx", S_IFREG | S_IRUGO,
                   priv->dir_dev, proc_get_stats_rx, dev);

    if (!e) {
        DMESGE("Unable to initialize "
              "/proc/net/rtl8187/%s/stats-rx\n",
              dev->name);
    }


    e = create_proc_read_entry("stats-tx", S_IFREG | S_IRUGO,
                   priv->dir_dev, proc_get_stats_tx, dev);

    if (!e) {
        DMESGE("Unable to initialize "
              "/proc/net/rtl8187/%s/stats-tx\n",
              dev->name);
    }
    #if 0
    e = create_proc_read_entry("stats-ieee", S_IFREG | S_IRUGO,
                   priv->dir_dev, proc_get_stats_ieee, dev);

    if (!e) {
        DMESGE("Unable to initialize "
              "/proc/net/rtl8187/%s/stats-ieee\n",
              dev->name);
    }


    e = create_proc_read_entry("stats-ap", S_IFREG | S_IRUGO,
                   priv->dir_dev, proc_get_stats_ap, dev);

    if (!e) {
        DMESGE("Unable to initialize "
              "/proc/net/rtl8187/%s/stats-ap\n",
              dev->name);
    }
    #endif

    e = create_proc_read_entry("registers", S_IFREG | S_IRUGO,
                   priv->dir_dev, proc_get_registers, dev);

    if (!e) {
        DMESGE("Unable to initialize "
              "/proc/net/rtl8187/%s/registers\n",
              dev->name);
    }
}
/****************************************************************************
   -----------------------------MISC STUFF-------------------------
*****************************************************************************/

void print_buffer(u32 *buffer, int len)
{
    int i;
    u8 *buf =(u8*)buffer;

    printk("ASCII BUFFER DUMP (len: %x):\n",len);

    for(i=0;i<len;i++)
        printk("%c",buf[i]);

    printk("\nBINARY BUFFER DUMP (len: %x):\n",len);

    for(i=0;i<len;i++)
        printk("%x",buf[i]);

    printk("\n");
}

short check_nic_enought_desc(struct net_device *dev, priority_t priority)
{
    struct r8180_priv *priv = ieee80211_priv(dev);

    int used = atomic_read((priority == NORM_PRIORITY) ?
        &priv->tx_np_pending : &priv->tx_lp_pending);

    return (used < MAX_TX_URB);
}

void tx_timeout(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    printk(KERN_INFO "Timeout pending %d!\n",atomic_read(&priv->tx_lp_pending));
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
    schedule_work(&priv->reset_wq);
#else
    schedule_task(&priv->reset_wq);
#endif
}


void dump_eprom(struct net_device *dev)
{
    int i;
    for(i=0; i<63; i++)
        DMESG("EEPROM addr %x : %x", i, eprom_read(dev,i));
}

void rtl8180_dump_reg(struct net_device *dev)
{
    int i;
    int n;
    int max=0xff;

    DMESG("Dumping NIC register map");

    for(n=0;n<=max;)
    {
        printk( "\nD: %2x> ", n);
        for(i=0;i<16 && n<=max;i++,n++)
            printk("%2x ",read_nic_byte(dev,n));
    }
    printk("\n");
}

/****************************************************************************
      ------------------------------HW STUFF---------------------------
*****************************************************************************/


void rtl8180_irq_enable(struct net_device *dev)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    write_nic_word(dev,INTA_MASK, priv->irq_mask);
}


void rtl8180_irq_disable(struct net_device *dev)
{

    write_nic_word(dev,INTA_MASK,0);
    force_pci_posting(dev);
}


void rtl8180_set_mode(struct net_device *dev,int mode)
{
    u8 ecmd;
    ecmd=read_nic_byte(dev, EPROM_CMD);
    ecmd=ecmd &~ EPROM_CMD_OPERATING_MODE_MASK;
    ecmd=ecmd | (mode<<EPROM_CMD_OPERATING_MODE_SHIFT);
    ecmd=ecmd &~ (1<<EPROM_CS_SHIFT);
    ecmd=ecmd &~ (1<<EPROM_CK_SHIFT);
    write_nic_byte(dev, EPROM_CMD, ecmd);
}


void rtl8180_update_msr(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    u8 msr;

    msr  = read_nic_byte(dev, MSR);
    msr &= ~ MSR_LINK_MASK;

    if (priv->ieee80211->state == IEEE80211_LINKED){

        if (priv->ieee80211->iw_mode == IW_MODE_INFRA)
            msr |= (MSR_LINK_MANAGED<<MSR_LINK_SHIFT);
        else if (priv->ieee80211->iw_mode == IW_MODE_ADHOC)
            msr |= (MSR_LINK_ADHOC<<MSR_LINK_SHIFT);
        else if (priv->ieee80211->iw_mode == IW_MODE_MASTER)
            msr |= (MSR_LINK_MASTER<<MSR_LINK_SHIFT);

    }else
        msr |= (MSR_LINK_NONE<<MSR_LINK_SHIFT);

    write_nic_byte(dev, MSR, msr);
}

void rtl8180_set_chan(struct net_device *dev,short ch)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    u32 tx;
    priv->chan=ch;
    #if 0
    if(priv->ieee80211->iw_mode == IW_MODE_ADHOC ||
        priv->ieee80211->iw_mode == IW_MODE_MASTER){

            priv->ieee80211->link_state = WLAN_LINK_ASSOCIATED;
            priv->ieee80211->master_chan = ch;
            rtl8180_update_beacon_ch(dev);
        }
    #endif

    tx = read_nic_dword(dev,TX_CONF);
    tx &= ~TX_LOOPBACK_MASK;

#ifndef LOOP_TEST
    write_nic_dword(dev,TX_CONF, tx |( TX_LOOPBACK_MAC<<TX_LOOPBACK_SHIFT));

    priv->rf_set_chan(dev,priv->chan);
    write_nic_dword(dev,TX_CONF,tx | (TX_LOOPBACK_NONE<<TX_LOOPBACK_SHIFT));
#endif
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
void rtl8187_rx_isr(struct urb *rx_urb,struct pt_regs *regs);
#else
void rtl8187_rx_isr(struct urb *rx_urb);
#endif


void rtl8187_rx_urbsubmit(struct net_device *dev, struct urb* rx_urb)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    int err;
#if 0
    if(priv->ieee80211->bHwRadioOff == true)
        return;
#endif


    usb_fill_bulk_urb(rx_urb,priv->udev,usb_rcvbulkpipe(priv->udev,0x81),
              rx_urb->transfer_buffer,RX_URB_SIZE,rtl8187_rx_isr,dev);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    err = usb_submit_urb(rx_urb, GFP_ATOMIC);
#else
    err = usb_submit_urb(rx_urb);
#endif

    if(err && err != -EPERM){
        DMESGE("cannot submit RX command. URB_STATUS %x",rx_urb->status);

    }

}


void rtl8187_rx_initiate(struct net_device *dev)
{
    int i;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    if(!priv->rx_urb)
        DMESGE("Cannot intiate RX urb mechanism");
    for(i=0;i<MAX_RX_URB;i++)
        rtl8187_rx_urbsubmit(dev,priv->rx_urb[i]);
    priv->tx_urb_index = 0;

}

void rtl8187_set_rxconf(struct net_device *dev)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    u32 rxconf;

    rxconf=read_nic_dword(dev,RX_CONF);
    rxconf = rxconf &~ MAC_FILTER_MASK;
    rxconf = rxconf | (1<<ACCEPT_MNG_FRAME_SHIFT);
    rxconf = rxconf | (1<<ACCEPT_DATA_FRAME_SHIFT);
    rxconf = rxconf | (1<<ACCEPT_BCAST_FRAME_SHIFT);
    rxconf = rxconf | (1<<ACCEPT_MCAST_FRAME_SHIFT);
    rxconf = rxconf | (1<<ACCEPT_CTL_FRAME_SHIFT);
#ifdef SW_ANTE_DIVERSITY
        rxconf = rxconf | priv->EEPROMCSMethod;
#endif

    if (dev->flags & IFF_PROMISC) DMESG ("NIC in promisc mode");

    if(priv->ieee80211->iw_mode == IW_MODE_MONITOR || \
       dev->flags & IFF_PROMISC){
        rxconf = rxconf | (1<<ACCEPT_ALLMAC_FRAME_SHIFT);
    }else{
        rxconf = rxconf | (1<<ACCEPT_NICMAC_FRAME_SHIFT);
        rxconf = rxconf | (1<<RX_CHECK_BSSID_SHIFT);
    }


    if(priv->ieee80211->iw_mode == IW_MODE_MONITOR){
        rxconf = rxconf | (1<<ACCEPT_ICVERR_FRAME_SHIFT);
        rxconf = rxconf | (1<<ACCEPT_PWR_FRAME_SHIFT);
    }

    if( priv->crcmon == 1 && priv->ieee80211->iw_mode == IW_MODE_MONITOR)
        rxconf = rxconf | (1<<ACCEPT_CRCERR_FRAME_SHIFT);


    rxconf = rxconf &~ RX_FIFO_THRESHOLD_MASK;
    rxconf = rxconf | (RX_FIFO_THRESHOLD_NONE<<RX_FIFO_THRESHOLD_SHIFT);


    rxconf = rxconf | (1<<RX_AUTORESETPHY_SHIFT);

    rxconf = rxconf &~ MAX_RX_DMA_MASK;
    rxconf = rxconf | (MAX_RX_DMA_2048<<MAX_RX_DMA_SHIFT);

    rxconf = rxconf | RCR_ONLYERLPKT;


    write_nic_dword(dev, RX_CONF, rxconf);


    #ifdef DEBUG_RX
    DMESG("rxconf: %x %x",rxconf ,read_nic_dword(dev,RX_CONF));
    #endif
}

void rtl8180_rx_enable(struct net_device *dev)
{
    u8 cmd;


    rtl8187_rx_initiate(dev);

    rtl8187_set_rxconf(dev);

    cmd=read_nic_byte(dev,CMD);
    write_nic_byte(dev,CMD,cmd | (1<<CMD_RX_ENABLE_SHIFT));


}


void rtl8180_tx_enable(struct net_device *dev)
{
    u8 cmd;
    u8 byte;
    u32 txconf;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    byte = read_nic_byte(dev,CW_CONF);
    byte &= ~(1<<CW_CONF_PERPACKET_CW_SHIFT);
    byte &= ~(1<<CW_CONF_PERPACKET_RETRY_SHIFT);
    write_nic_byte(dev, CW_CONF, byte);

    byte = read_nic_byte(dev, TX_AGC_CTL);
    byte &= ~(1<<TX_AGC_CTL_PERPACKET_GAIN_SHIFT);
    byte &= ~(1<<TX_AGC_CTL_PERPACKET_ANTSEL_SHIFT);
    byte &= ~(1<<TX_AGC_CTL_FEEDBACK_ANT);
    write_nic_byte(dev, TX_AGC_CTL, byte);

    txconf= read_nic_dword(dev,TX_CONF);

    #if 0
    if(priv->card_8185){

        txconf = txconf &~ (1<<TCR_PROBE_NOTIMESTAMP_SHIFT);

    }else{

        if(priv->ieee80211->hw_seq)
            txconf= txconf &~ (1<<TX_CONF_HEADER_AUTOICREMENT_SHIFT);
        else
            txconf= txconf | (1<<TX_CONF_HEADER_AUTOICREMENT_SHIFT);
    }
    #endif

    txconf = txconf &~ TX_LOOPBACK_MASK;

#ifndef LOOP_TEST
    txconf = txconf | (TX_LOOPBACK_NONE<<TX_LOOPBACK_SHIFT);
#else
    txconf = txconf | (TX_LOOPBACK_BASEBAND<<TX_LOOPBACK_SHIFT);
#endif
    txconf = txconf &~ TCR_DPRETRY_MASK;
    txconf = txconf &~ TCR_RTSRETRY_MASK;

    txconf = txconf | (priv->retry_data<<TX_DPRETRY_SHIFT);
    txconf = txconf | (priv->retry_rts<<TX_RTSRETRY_SHIFT);

    txconf = txconf &~ (1<<TX_NOCRC_SHIFT);

    txconf = txconf &~ TCR_MXDMA_MASK;
    txconf = txconf | (TCR_MXDMA_2048<<TCR_MXDMA_SHIFT);

    txconf = txconf | TCR_CWMIN;
    txconf = txconf | TCR_DISCW;
    txconf = txconf &~ TCR_SWPLCPLEN;

    txconf=txconf | (1<<TX_NOICV_SHIFT);

    write_nic_dword(dev,TX_CONF,txconf);



#ifdef DEBUG_TX
    DMESG("txconf: %x %x",txconf,read_nic_dword(dev,TX_CONF));
#endif

    cmd=read_nic_byte(dev,CMD);
    write_nic_byte(dev,CMD,cmd | (1<<CMD_TX_ENABLE_SHIFT));

}

#if 0
void rtl8180_beacon_tx_enable(struct net_device *dev)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    priv->dma_poll_mask &=~(1<<TX_DMA_STOP_BEACON_SHIFT);
    rtl8180_set_mode(dev,EPROM_CMD_CONFIG);
    write_nic_byte(dev,TX_DMA_POLLING,priv->dma_poll_mask);
    rtl8180_set_mode(dev,EPROM_CMD_NORMAL);
}


void rtl8180_
_disable(struct net_device *dev)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    priv->dma_poll_mask |= (1<<TX_DMA_STOP_BEACON_SHIFT);
    rtl8180_set_mode(dev,EPROM_CMD_CONFIG);
    write_nic_byte(dev,TX_DMA_POLLING,priv->dma_poll_mask);
    rtl8180_set_mode(dev,EPROM_CMD_NORMAL);
}

#endif


void rtl8180_rtx_disable(struct net_device *dev)
{
    u8 cmd;

    cmd=read_nic_byte(dev,CMD);
    write_nic_byte(dev, CMD, cmd &~ \
               ((1<<CMD_RX_ENABLE_SHIFT)|(1<<CMD_TX_ENABLE_SHIFT)));
    force_pci_posting(dev);
    mdelay(10);
#if 0
    if(priv->rx_urb){
        for(i=0;i<MAX_RX_URB;i++)
        usb_kill_urb(priv->rx_urb[i]);
    }
#endif
}


int alloc_tx_beacon_desc_ring(struct net_device *dev, int count)

{
    #if 0
    int i;
    u32 *tmp;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    priv->txbeaconring = (u32*)pci_alloc_consistent(priv->pdev,
                      sizeof(u32)*8*count,
                      &priv->txbeaconringdma);
    if (!priv->txbeaconring) return -1;
    for (tmp=priv->txbeaconring,i=0;i<count;i++){
        *tmp = *tmp &~ (1<<31);
        if(i+1<count)
            *(tmp+4) = (u32)priv->txbeaconringdma+((i+1)*8*4);
        else
            *(tmp+4) = (u32)priv->txbeaconringdma;

        tmp=tmp+8;
    }
    #endif
    return 0;
}


void rtl8180_reset(struct net_device *dev)
{

    u8 cr;

    rtl8180_set_anaparam(dev, RTL8225_ANAPARAM_ON);
    rtl8185_set_anaparam2(dev, RTL8225_ANAPARAM2_ON);

    rtl8180_irq_disable(dev);

    mdelay(200);
    write_nic_byte_E(dev,0x18,0x10);
    write_nic_byte_E(dev,0x18,0x11);
    write_nic_byte_E(dev,0x18,0x00);
    mdelay(200);

    cr=read_nic_byte(dev,CMD);
    cr = cr & 2;
    cr = cr | (1<<CMD_RST_SHIFT);
    write_nic_byte(dev,CMD,cr);

    force_pci_posting(dev);

    mdelay(200);

    if(read_nic_byte(dev,CMD) & (1<<CMD_RST_SHIFT))
        DMESGW("Card reset timeout!");
    else
        DMESG("Card successfully reset");

    rtl8180_set_mode(dev,EPROM_CMD_LOAD);
    force_pci_posting(dev);
    mdelay(200);

    rtl8180_set_anaparam(dev, RTL8225_ANAPARAM_ON);
    rtl8185_set_anaparam2(dev, RTL8225_ANAPARAM2_ON);

}

inline u16 ieeerate2rtlrate(int rate)
{
    switch(rate){
    case 10:
    return 0;
    case 20:
    return 1;
    case 55:
    return 2;
    case 110:
    return 3;
    case 60:
    return 4;
    case 90:
    return 5;
    case 120:
    return 6;
    case 180:
    return 7;
    case 240:
    return 8;
    case 360:
    return 9;
    case 480:
    return 10;
    case 540:
    return 11;
    default:
    return 3;

    }
}
static u16 rtl_rate[] = {10,20,55,110,60,90,120,180,240,360,480,540};
inline u16 rtl8180_rate2rate(short rate)
{
    if (rate >11) return 0;
    return rtl_rate[rate];
}


inline u8 rtl8180_IsWirelessBMode(u16 rate)
{
    if( ((rate <= 110) && (rate != 60) && (rate != 90)) || (rate == 220) )
        return 1;
    else return 0;
}

u16 N_DBPSOfRate(u16 DataRate)
{
     u16 N_DBPS = 24;

     switch(DataRate)
     {
     case 60:
      N_DBPS = 24;
      break;

     case 90:
      N_DBPS = 36;
      break;

     case 120:
      N_DBPS = 48;
      break;

     case 180:
      N_DBPS = 72;
      break;

     case 240:
      N_DBPS = 96;
      break;

     case 360:
      N_DBPS = 144;
      break;

     case 480:
      N_DBPS = 192;
      break;

     case 540:
      N_DBPS = 216;
      break;

     default:
      break;
     }

     return N_DBPS;
}

u16 ComputeTxTime(
    u16     FrameLength,
    u16     DataRate,
    u8      bManagementFrame,
    u8      bShortPreamble
)
{
    u16 FrameTime;
    u16 N_DBPS;
    u16 Ceiling;

    if( rtl8180_IsWirelessBMode(DataRate) )
    {
        if( bManagementFrame || !bShortPreamble || DataRate == 10 )
        {
            FrameTime = (u16)(144+48+(FrameLength*8/(DataRate/10)));
        }
        else
        {
            FrameTime = (u16)(72+24+(FrameLength*8/(DataRate/10)));
        }
        if( ( FrameLength*8 % (DataRate/10) ) != 0 )
                FrameTime ++;
    } else {
        N_DBPS = N_DBPSOfRate(DataRate);
        Ceiling = (16 + 8*FrameLength + 6) / N_DBPS
                + (((16 + 8*FrameLength + 6) % N_DBPS) ? 1 : 0);
        FrameTime = (u16)(16 + 4 + 4*Ceiling + 6);
    }
    return FrameTime;
}



#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
void rtl8187_rx_isr(struct urb *rx_urb,struct pt_regs *regs)
#else
void rtl8187_rx_isr(struct urb *rx_urb)
#endif
{
    struct net_device *dev = (struct net_device*)rx_urb->context;
    struct r8180_priv *priv = ieee80211_priv(dev);

    priv->rxurb_task = rx_urb;
    tasklet_schedule(&priv->irq_rx_tasklet);
}

#if 0
void rtl8180_tx_queues_stop(struct net_device *dev)
{
    u8 dma_poll_mask = (1<<TX_DMA_STOP_LOWPRIORITY_SHIFT);
    dma_poll_mask |= (1<<TX_DMA_STOP_HIPRIORITY_SHIFT);
    dma_poll_mask |= (1<<TX_DMA_STOP_NORMPRIORITY_SHIFT);
    dma_poll_mask |= (1<<TX_DMA_STOP_BEACON_SHIFT);

    rtl8180_set_mode(dev,EPROM_CMD_CONFIG);
    write_nic_byte(dev,TX_DMA_POLLING,dma_poll_mask);
    rtl8180_set_mode(dev,EPROM_CMD_NORMAL);
}
#endif

void rtl8180_data_hard_stop(struct net_device *dev)
{
    #if 0
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    priv->dma_poll_mask |= (1<<TX_DMA_STOP_LOWPRIORITY_SHIFT);
    rtl8180_set_mode(dev,EPROM_CMD_CONFIG);
    write_nic_byte(dev,TX_DMA_POLLING,priv->dma_poll_mask);
    rtl8180_set_mode(dev,EPROM_CMD_NORMAL);
    #endif
}


void rtl8180_data_hard_resume(struct net_device *dev)
{
    #if 0
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    priv->dma_poll_mask &= ~(1<<TX_DMA_STOP_LOWPRIORITY_SHIFT);
    rtl8180_set_mode(dev,EPROM_CMD_CONFIG);
    write_nic_byte(dev,TX_DMA_POLLING,priv->dma_poll_mask);
    rtl8180_set_mode(dev,EPROM_CMD_NORMAL);
    #endif
}


void rtl8180_hard_data_xmit(struct sk_buff *skb, struct net_device *dev, int rate)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    short morefrag = 0;
    unsigned long flags;
    struct ieee80211_hdr *h = (struct ieee80211_hdr  *) skb->data;

    if (le16_to_cpu(h->frame_ctl) & IEEE80211_FCTL_MOREFRAGS)
        morefrag = 1;
    spin_lock_irqsave(&priv->tx_lock,flags);

    if(priv->ieee80211->bHwRadioOff)
    {
        spin_unlock_irqrestore(&priv->tx_lock,flags);
        return ;
    }

    if (!check_nic_enought_desc(dev, LOW_PRIORITY)){
        DMESG("Error: no TX slot ");
        ieee80211_stop_queue(priv->ieee80211);
    }
#ifdef LED_SHIN
    priv->ieee80211->ieee80211_led_contorl(dev,LED_CTL_TX);
#endif

    rtl8180_tx(dev,skb->data, skb->len, LOW_PRIORITY, morefrag,
        ieeerate2rtlrate(rate));

    priv->stats.txdatapkt++;

    if (!check_nic_enought_desc(dev, LOW_PRIORITY))
        ieee80211_stop_queue(priv->ieee80211);

    spin_unlock_irqrestore(&priv->tx_lock,flags);

}
#if 0
int rtl8180_hard_start_xmit(struct sk_buff *skb,struct net_device *dev)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    int ret;
    unsigned long flags;

    spin_lock_irqsave(&priv->tx_lock,flags);

    ret = rtl8180_tx(dev, (u32*)skb->data, skb->len, NORM_PRIORITY, 0,DEFAULT_BASICRATE);
    priv->ieee80211->stats.tx_bytes+=skb->len;
    priv->ieee80211->stats.tx_packets++;

    spin_unlock_irqrestore(&priv->tx_lock,flags);

    dev_kfree_skb_any(skb);
    return ret;
}
#endif

#if 0
u16 rtl8180_len2duration(u32 len, short rate,short* ext)
{
    u16 duration;
    u16 drift;
    *ext=0;

    switch(rate){
    case 0:
        *ext=0;
        duration = ((len+4)<<4) /0x2;
        drift = ((len+4)<<4) % 0x2;
        if(drift ==0 ) break;
        duration++;
        break;

    case 1:
        *ext=0;
        duration = ((len+4)<<4) /0x4;
        drift = ((len+4)<<4) % 0x4;
        if(drift ==0 ) break;
        duration++;
        break;

    case 2:
        *ext=0;
        duration = ((len+4)<<4) /0xb;
        drift = ((len+4)<<4) % 0xb;
        if(drift ==0 )
            break;
        duration++;
        break;

    default:
    case 3:
        *ext=0;
        duration = ((len+4)<<4) /0x16;
        drift = ((len+4)<<4) % 0x16;
        if(drift ==0 )
            break;
        duration++;
        if(drift > 6)
            break;
        *ext=1;
        break;
    }

    return duration;
}
#endif

void rtl8180_try_wake_queue(struct net_device *dev, int pri);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
void rtl8187_lptx_isr(struct urb *tx_urb, struct pt_regs *regs)
#else
void rtl8187_lptx_isr(struct urb* tx_urb)
#endif
{
    struct net_device *dev = (struct net_device*)tx_urb->context;
    struct r8180_priv *priv = ieee80211_priv(dev);

        if(tx_urb->status == 0)
        priv->stats.txlpokint++;
    else
        priv->stats.txlperr++;
    atomic_dec(&priv->tx_lp_pending);
    rtl8180_try_wake_queue(dev,LOW_PRIORITY);
}


void rtl8187_beacon_stop(struct net_device *dev)
{
    u8 msr, msrm, msr2;
    msr  = read_nic_byte(dev, MSR);
    msrm = msr & MSR_LINK_MASK;
    msr2 = msr & ~MSR_LINK_MASK;
    if ((msrm == (MSR_LINK_ADHOC<<MSR_LINK_SHIFT) ||
        (msrm == (MSR_LINK_MASTER<<MSR_LINK_SHIFT)))){
        write_nic_byte(dev, MSR, msr2 | MSR_LINK_NONE);
        write_nic_byte(dev, MSR, msr);
    }


}


void rtl8187_net_update(struct net_device *dev)
{

    struct r8180_priv *priv = ieee80211_priv(dev);
    struct ieee80211_network *net;
    net = & priv->ieee80211->current_network;


    write_nic_dword(dev,BSSID,((u32*)net->bssid)[0]);
    write_nic_word(dev,BSSID+4,((u16*)net->bssid)[2]);

    rtl8180_update_msr(dev);

    write_nic_word(dev, AtimWnd, 2);
    write_nic_word(dev, AtimtrItv, 100);
    write_nic_word(dev, BEACON_INTERVAL, net->beacon_interval);
    write_nic_word(dev, BcnIntTime, 100);


}

void rtl8187_beacon_tx(struct net_device *dev)
{

    struct r8180_priv *priv = ieee80211_priv(dev);
    struct sk_buff *skb;
    int i = 0;

    rtl8187_net_update(dev);

    skb = ieee80211_get_beacon(priv->ieee80211);


    if(!skb){
        DMESG("not enought memory for allocating beacon");
        return;
    }

#if 0
    while(MAX_TX_URB!=atomic_read(&priv->tx_np_pending)){
        msleep_interruptible_rtl(HZ/2);
        if(i++ > 20){
            DMESG("get stuck to wait EP3 become ready");
            return ;
        }
    }
#endif
    write_nic_byte(dev, BQREQ, read_nic_byte(dev, BQREQ) | (1<<7));

    i=0;
    while( (read_nic_byte(dev, BQREQ) & (1<<7)) == 0 )
    {
        msleep_interruptible_rtl(HZ/2);
        if(i++ > 10){
            DMESG("get stuck to wait HW beacon to be ready");
            return ;
        }
    }

    rtl8180_tx(dev, skb->data, skb->len, NORM_PRIORITY,
        0, priv->ieee80211->basic_rate);

    priv->stats.txbeaconerr++;

    dev_kfree_skb_any(skb);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
void rtl8187_nptx_isr(struct urb *tx_urb, struct pt_regs *regs)
#else
void rtl8187_nptx_isr(struct urb* tx_urb)
#endif
{
    struct net_device *dev = (struct net_device*)tx_urb->context;
    struct r8180_priv *priv = ieee80211_priv(dev);
    if(tx_urb->status == 0)
        priv->stats.txnpokint++;
    else
        priv->stats.txnperr++;
    atomic_dec(&priv->tx_np_pending);
}



short rtl8180_tx(struct net_device *dev, u8* txbuf, int len, priority_t priority,
         short morefrag, short rate)
{
    u8 *tx;
    u8 *pdata;
    u8 seg = ((u32)txbuf % 4);
    u32 *pdata32;
    u32 *ptxbuf32;
    int i;
    int pend;
    int status;
    struct urb *tx_urb;
    int urb_len;
    u16 AckCtsTime;
    u16 FrameTime;
    u16 duration;

    struct r8180_priv *priv = ieee80211_priv(dev);

        pend = atomic_read((priority == NORM_PRIORITY)? &priv->tx_np_pending : &priv->tx_lp_pending);
    if( pend > MAX_TX_URB){
        if(priority == NORM_PRIORITY)
            priv->stats.txnpdrop++;
        else
            priv->stats.txlpdrop++;
                printk("pending > MAX_TX_URB\n");
        return -1;
    }

#if 0
    urb_len = len + 4*3;
    if((0 == urb_len%64)||(0 == urb_len%512)) {
      urb_len += 1;
    }
    tx = kmalloc(urb_len, GFP_ATOMIC);
    if(!tx) return -ENOMEM;
    tx_urb = usb_alloc_urb(0,GFP_ATOMIC);

    if(!tx_urb){

        kfree(tx);
        return -ENOMEM;
    }
#else
    urb_len = len + 4*3;
    if((0 == urb_len%64)||(0 == urb_len%512)) {
      urb_len += 1;
    }

    tx_urb = priv->tx_context[priv->tx_urb_index].tx_urb;
    tx = priv->tx_context[priv->tx_urb_index].ptalign_buf + seg;
    pdata = tx + 12;
    priv->tx_urb_index = (priv->tx_urb_index + 1) % MAX_TX_URB;
#endif
#if 0
    memcpy(tx+3,txbuf,len);
    tx[0] = 0;
    tx[0] |= len & 0xfff;
    tx[0] |= (1<<15);
    if (priv->ieee80211->current_network.capability&WLAN_CAPABILITY_SHORT_PREAMBLE)
        if (priv->plcp_preamble_mode==1 && rate!=0) {
            tx[0] |= (1<<16);
        }

    if ( (len>priv->rts) && priv->rts && priority==LOW_PRIORITY){
            tx[0] |= (1<<23);
            AckCtsTime = ComputeTxTime(14, 10,0, 0);
            FrameTime = ComputeTxTime(len + 4, rtl8180_rate2rate(rate), 0, tx[0]&(1<<16));
            duration = FrameTime + 3*10 + 2*AckCtsTime;
            tx[1] |= duration;
            printk(KERN_INFO "duration %d!\n",duration);
    }
    else
        tx[1]=0;
    if(morefrag) tx[0] |= (1<<17);
    tx[0] |= (ieeerate2rtlrate(priv->ieee80211->basic_rate) << 19);
    tx[0] |= (rate << 24);



    tx[2] = 3;
    tx[2] |= (7<<4);
    tx[2] |= (11<<8);


    #ifdef DUMP_TX
    int i;
    printk("<Tx pkt>--rate %x---",rate);
    for (i = 0; i < (len + 3); i++)
        printk("%2x", ((u8*)tx)[i]);
    printk("---------------\n");
    #endif
#endif
    if (seg) {
        for (i = 0; i < 4 - seg; i++) {
            *pdata++ = *txbuf++;
        }
    }

    pdata32 = (u32 *)pdata;
    ptxbuf32 = (u32 *)txbuf;
#if 0
    memcpy(pdata32, (u32 *)txbuf, (len+4)/4);
#else
    for(i = 0; i < (len - seg + 4)/4; i++) {
        *pdata32++ = *ptxbuf32++;
    }
#endif

    tx[2] = 0;
    tx[3] = 0;

    tx[0] = len & 0xff;
    tx[1] = (len & 0x0f00) >> 8;
    tx[1] |= (1<<7);

    if (priv->ieee80211->current_network.capability&WLAN_CAPABILITY_SHORT_PREAMBLE)
        if (priv->plcp_preamble_mode==1 && rate!=0) {
            tx[2] |= 1;

        }

    if ( (len>priv->rts) && priv->rts && priority==LOW_PRIORITY){
            tx[2] |= (1<<7);
            AckCtsTime = ComputeTxTime(14, 10,0, 0);
            FrameTime = ComputeTxTime(len + 4, rtl8180_rate2rate(rate), 0, tx[2]&1);
            duration = FrameTime + 3*10 + 2*AckCtsTime;
            tx[4] |= duration & 0xff;
            tx[5] |= (duration & 0xff00) >> 8;
            printk(KERN_INFO "duration %d!\n",duration);
    }
    else {
        tx[4] = 0;
        tx[5] = 0;
    }

    if(morefrag) tx[2] |= (1<<1);
    tx[2] |= (ieeerate2rtlrate(priv->ieee80211->basic_rate) << 3);
    tx[3] |= rate;

    tx[8] = 3;
    tx[8] |= (7<<4);
    tx[9] |= 11;



    usb_fill_bulk_urb(tx_urb,priv->udev,
                          usb_sndbulkpipe(priv->udev,(priority == LOW_PRIORITY)?2:3), tx,
              urb_len, ((priority == LOW_PRIORITY)?rtl8187_lptx_isr:rtl8187_nptx_isr), dev);

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    status = usb_submit_urb(tx_urb, GFP_ATOMIC);
#else
    status = usb_submit_urb(tx_urb);
#endif

        if (!status){
        atomic_inc((priority == NORM_PRIORITY)? &priv->tx_np_pending : &priv->tx_lp_pending);
        priv->stats.txok++;
        return 0;
    }else{
        priv->stats.txerr++;
        printk("Error TX URB %d, error %d",
                atomic_read((priority == NORM_PRIORITY)? &priv->tx_np_pending : &priv->tx_lp_pending),
                status);
        return -1;
    }
}



void rtl8180_irq_rx_tasklet(struct r8180_priv * priv);

short rtl8187_usb_initendpoints(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    int i,j;

    priv->rx_urb = (struct urb**) kmalloc (sizeof(struct urb*) * MAX_RX_URB, GFP_KERNEL);

    for(i=0;i<MAX_RX_URB;i++){
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        priv->rx_urb[i] = usb_alloc_urb(0,GFP_KERNEL);
#else
        priv->rx_urb[i] = usb_alloc_urb(0);
#endif
        if(!priv->rx_urb[i])
            goto destroy;

        priv->rx_urb[i]->transfer_buffer = kmalloc(RX_URB_SIZE, GFP_KERNEL);
        if(!priv->rx_urb[i]->transfer_buffer)
            goto destroy1;

        priv->rx_urb[i]->transfer_buffer_length = RX_URB_SIZE;
    }

    for(j=0; j < MAX_TX_URB; j++){
        struct tx_urb_context *ptrcontext = &priv->tx_context[j];
        u8 seg_size;

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        ptrcontext->tx_urb = usb_alloc_urb(0,GFP_KERNEL);
#else
        ptrcontext->tx_urb = usb_alloc_urb(0);
#endif
        if(!ptrcontext->tx_urb)
            goto destroy_tx;

        ptrcontext->transfer_buffer = kmalloc(TX_URB_SIZE, GFP_KERNEL);
        if(!ptrcontext->transfer_buffer)
            goto destroy1_tx;
        seg_size = (u32)ptrcontext->transfer_buffer % 4;
        ptrcontext->ptalign_buf = ptrcontext->transfer_buffer + ((seg_size > 0)? (4 - seg_size):0);
    }

    return 0;

destroy1_tx:
    usb_free_urb(priv->tx_context[j].tx_urb);

destroy_tx:
    while (--j >= 0){
        kfree(priv->tx_context[j].transfer_buffer);
        usb_free_urb(priv->tx_context[j].tx_urb);
    }

destroy1:
    usb_free_urb(priv->rx_urb[i]);

destroy:
    while (--i >= 0){
        kfree(priv->rx_urb[i]->transfer_buffer);
        usb_free_urb(priv->rx_urb[i]);
    }

    kfree(priv->rx_urb);

    priv->rx_urb = NULL;
    DMESGE("Endpoint Alloc Failure");
    return -ENOMEM;

}

void rtl8187_usb_deleteendpoints(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    int i,j;
    if(priv->rx_urb){
        for(i=0;i<MAX_RX_URB;i++){
            usb_kill_urb(priv->rx_urb[i]);
            kfree(priv->rx_urb[i]->transfer_buffer);
            usb_free_urb(priv->rx_urb[i]);
        }
        kfree(priv->rx_urb);
        priv->rx_urb = NULL;

    }
    for(j=0; j < MAX_TX_URB; j++){
        struct tx_urb_context *ptrcontext = &priv->tx_context[j];

        usb_kill_urb(ptrcontext->tx_urb);
        kfree(ptrcontext->transfer_buffer);
        ptrcontext->transfer_buffer = NULL;
        usb_free_urb(ptrcontext->tx_urb);
        ptrcontext->tx_urb = NULL;
    }
}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
EXPORT_SYMBOL(rtl8187_usb_deleteendpoints);
#else
EXPORT_SYMBOL_NOVERS(rtl8187_usb_deleteendpoints);
#endif

void rtl8187_set_rate(struct net_device *dev)
{
    int i;
    u16 word;
    int basic_rate,min_rr_rate,max_rr_rate;


    basic_rate = ieeerate2rtlrate(240);
    min_rr_rate = ieeerate2rtlrate(60);
    max_rr_rate = ieeerate2rtlrate(240);


    write_nic_byte(dev, RESP_RATE,
            max_rr_rate<<MAX_RESP_RATE_SHIFT| min_rr_rate<<MIN_RESP_RATE_SHIFT);

    word  = read_nic_word(dev, BRSR);
    word &= ~BRSR_MBR_8185;


    for(i=0;i<=basic_rate;i++)
        word |= (1<<i);

    write_nic_word(dev, BRSR, word);
}


void rtl8187_link_change(struct net_device *dev)
{

    struct r8180_priv *priv = ieee80211_priv(dev);
    rtl8187_net_update(dev);
    rtl8180_set_chan(dev, priv->chan);
    rtl8187_set_rxconf(dev);
}

#define HW_WAKE_DELAY 5

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_hw_wakeup(struct work_struct * work)
{
    struct ieee80211_device *ieee = container_of(work, struct ieee80211_device, hw_wakeup_wq);
    struct r8180_priv *priv = (r8180_priv *)ieee->priv;
    struct net_device *dev = priv->dev;
#else
void rtl8180_hw_wakeup(struct net_device *dev)
{

    struct r8180_priv *priv = ieee80211_priv(dev);
#endif
    printk(KERN_INFO "enter rtl8180_hw_wakeup!\n");
    if (!priv->ps_sleep_finish) {
        printk(KERN_INFO "last time sleep unfinished!\n");
        return;
    }

        if(priv->rf_wakeup)
            priv->rf_wakeup(dev);
        priv->ps_sleep_finish=0;
    printk(KERN_INFO "leave wakeup!\n");
}



void timer_hw_wakeup_wq(unsigned long data)
{
    struct r8180_priv *priv = ieee80211_priv((struct net_device *)data);
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
    schedule_work(&priv->rtl8180_hw_wakeup_wq);
#else
    schedule_task(&priv->rtl8180_hw_wakeup_wq);
#endif
    printk(KERN_INFO "timer wakup schedule!\n");
}

#if LINUX_VERSION_CODE >=KERNEL_VERSION(2,6,20)
void rtl8180_rq_tx_ack(struct work_struct * work)
{
    struct ieee80211_device *ieee = container_of(work, struct ieee80211_device, ps_request_tx_ack_wq);
    struct r8180_priv *priv = (struct r8180_priv *)ieee->priv;
#else
void rtl8180_rq_tx_ack(struct net_device *dev){

    struct r8180_priv *priv = ieee80211_priv(dev);
#endif
    priv->ack_tx_to_ieee = 1;
}

#if LINUX_VERSION_CODE >=KERNEL_VERSION(2,6,20)
void rtl8180_hw_sleep(struct work_struct * work)
{
    struct ieee80211_device *ieee = container_of(work, struct ieee80211_device, hw_sleep_wq);
    struct r8180_priv *priv = (struct r8180_priv *)ieee->priv;
        struct net_device *dev = priv->dev;
#else
void rtl8180_hw_sleep(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
#endif
    u32 rb,tl;
    u32  timeout;
    if (priv->ps_sleep_finish) {
        printk(KERN_INFO "last time wake unfinished!\n");
        return;
    }
    rb = read_nic_dword(dev, TSFTR);

    tl = priv->ieee80211->ps_tl;
    tl -= HW_WAKE_DELAY*1000;

    if(tl == 0) tl = 1;


#ifdef TONY_PS
    printk(KERN_INFO "tl %lu, rb %lu!\n",tl,rb);
#endif


    if(((rb<tl)&& (tl-rb) < MIN_SLEEP_TIME)
        ||((rb>tl)&& (rb-tl) < MIN_SLEEP_TIME))
        return;


    if(((tl > rb) && ((tl-rb) > MAX_SLEEP_TIME))||
        ((tl < rb) && ((rb-tl) > MAX_SLEEP_TIME)))
        return;

    if (rb>tl)
        timeout = (rb-tl)>>10;
    else
        timeout = (tl-rb)>>10;

    mod_timer(&priv->ps_timer, jiffies+timeout);

    if(priv->rf_sleep)
        priv->rf_sleep(dev);
    priv->ps_sleep_finish=1;
    printk(KERN_INFO "leave sleep!\n");
}

short rtl8180_is_tx_queue_empty(struct net_device *dev){

    struct r8180_priv *priv = ieee80211_priv(dev);

    int used;
#ifdef TONY_TRACK
    DMESG("enter rtl8180_is_tx_queue_empty!\n");
#endif

    used = atomic_read(&priv->tx_np_pending);
    if (used) return 0;
    used = atomic_read(&priv->tx_lp_pending);
    if (used) return 0;

    return 1;
}

void sta_rateadaptive8187(  struct net_device *dev)
{
        struct r8180_priv *priv = ieee80211_priv(dev);
    struct ieee80211_device* ieee = priv->ieee80211;
    u8          CurrTxokCnt;
    u16         CurrRetryCnt;
    u16         CurrRetryRate;
    u8          CurrRxokCnt;
    long            CurrSignalStrength;


    priv->CurrRetryCnt = read_nic_word(dev, Tally_Cnt);

    CurrRetryCnt = priv->CurrRetryCnt - priv->LastRetryCnt ;
        CurrTxokCnt  = (priv->stats.txbeaconokint + priv->stats.txnpokint +
                                        priv->stats.txhpokint + priv->stats.txlpokint )- priv->LastTxokCnt;
        CurrRxokCnt  =  priv->stats.rxok - priv->LastRxokCnt;
    CurrSignalStrength = priv->RecvSignalPower;
        priv->CurrentOperaRate = priv->ieee80211->rate / 5;

    if ((CurrTxokCnt>50) || (CurrRxokCnt>50))
    {
    priv->ScanAllowedNow = 0;
    }
    else
    {
    priv->ScanAllowedNow = 1;
    }

    if (CurrTxokCnt>0)
    {
        CurrRetryRate = (u16)(CurrRetryCnt*100/CurrTxokCnt);
    }
    else
    {
        CurrRetryRate = 0;
    }
#if 0
    printk("\n(0) priv->CurrRetryCnt: %d \n",priv->CurrRetryCnt);
    printk("(1) priv->LastRetryRate: %d \n",priv->LastRetryRate);
    printk("(2) CurrRetryCnt = %d  \n", CurrRetryCnt);
    printk("(3) TxokCnt = %d \n", CurrTxokCnt);
    printk("(3) RxokCnt = %d \n", CurrRxokCnt);
    printk("(4) CurrRetryRate = %d \n", CurrRetryRate);
    printk("(5) SignalStrength = %d \n",priv->RecvSignalPower);
#endif

        priv->LastRetryCnt = priv->CurrRetryCnt ;
        priv->LastTxokCnt  = (priv->stats.txbeaconokint + priv->stats.txnpokint +
                                        priv->stats.txhpokint + priv->stats.txlpokint );
        priv->LastRxokCnt = priv->stats.rxok;
        priv->CurrRetryCnt = 0;

#if 1
    if ((&ieee->current_network)->stats.signalstrength > 85 )
    {
        priv->CurrentOperaRate = 108;
    }
#endif

    if ( (CurrTxokCnt==0)|| (CurrRetryRate==0))
    {
        priv->TryupingCountNoData++;
#if 1
        if (priv->TryupingCountNoData>10)
        {
            priv->TryupingCountNoData=0;
            if ( priv->CurrentOperaRate == 96 ) {
                priv->CurrentOperaRate = 108;
            }else if (priv->CurrentOperaRate == 72   ) {
                priv->CurrentOperaRate = 96;
            }else if (priv->CurrentOperaRate == 48   ) {
                priv->CurrentOperaRate = 72;
            }else if (priv->CurrentOperaRate == 36   ) {
                priv->CurrentOperaRate = 48;
            }else if (priv->CurrentOperaRate == 22   ) {
                priv->CurrentOperaRate = 36;
            }else if (priv->CurrentOperaRate == 11   ) {
                priv->CurrentOperaRate = 22;
            }else if (priv->CurrentOperaRate == 4   ) {
                priv->CurrentOperaRate = 11;
            }else if (priv->CurrentOperaRate == 2   ) {
                priv->CurrentOperaRate = 4;
            }

        }
#endif
        goto end;
    }
    else
    {
        priv->TryupingCountNoData=0;
    }


    if (priv->bTryuping == true)
    {
        priv->bTryuping = false;
        priv->TryupingCount=0;
        if ( (CurrRetryRate>15)&&(priv->CurrRetryRate < CurrRetryRate) )
        {
            priv->CurrentOperaRate = priv->CurrTxRate;
            priv->LastRetryRate = CurrRetryRate;
            goto end;
        }
    }

    if(CurrTxokCnt< 1 && CurrRetryRate > 100)
    {

        switch(priv->CurrentOperaRate)
        {
        case 108:
            priv->CurrentOperaRate = 96;
            break;

        case 96:
            priv->CurrentOperaRate = 72;
            break;

        case 72:
            priv->CurrentOperaRate = 48;
            break;

        case 48:
            priv->CurrentOperaRate = 36;
            break;

        case 36:
            priv->CurrentOperaRate = 24;
            break;

        case 24:
            priv->CurrentOperaRate = 22;
            break;

        default:
            priv->CurrentOperaRate = 22;
            break;
        }
    }
    else if ( priv->CurrentOperaRate == 108 )
    {
        if ( (CurrRetryRate>40)&&(priv->LastRetryRate>40) )
        {
            priv->CurrentOperaRate = 96;
        }
    }
    else if ( priv->CurrentOperaRate == 96 )
    {
        if ( ((CurrRetryRate>40) && (priv->LastRetryRate>40)) )
        {
            priv->CurrentOperaRate = 72;
        }
        else if ( (CurrRetryRate<15) && (priv->LastRetryRate<15) )
        {
            priv->CurrentOperaRate = 108;
        }
    }
    else if ( priv->CurrentOperaRate == 72 )
    {
        if ( (CurrRetryRate>99) && (priv->LastRetryRate>99) )
        {
            priv->CurrentOperaRate = 48;
        }
        else if ( (CurrRetryRate<15) )
        {
            priv->TryupingCount++;
            if ( (priv->bTryuping==false)&& (priv->TryupingCount>20) )
            {
                priv->TryupingCount = 0;
                priv->bTryuping = true;
                priv->CurrTxRate =  priv->CurrentOperaRate;
                priv->CurrRetryRate = CurrRetryRate;

                priv->CurrentOperaRate = 96;
            }
        }
    }
    else if ( priv->CurrentOperaRate == 48 )
    {
        if ( ((CurrRetryRate>99) && (priv->LastRetryRate>99) )
            )
        {
            priv->CurrentOperaRate = 36;


            if (priv->InitialGain != 4 )
            {
                priv->InitialGain = 4;
                UpdateInitialGain(dev);
            }
        }
        else if ( (CurrRetryRate<15) && (priv->LastRetryRate<15) )
        {
            priv->CurrentOperaRate = 72;
        }

    }
    else if ( priv->CurrentOperaRate == 36 )
    {
        if ( ((CurrRetryRate>99) && (priv->LastRetryRate>99)) )
        {
            priv->CurrentOperaRate = 22;


            if (priv->InitialGain != 4 )
            {
                priv->InitialGain = 4;
                UpdateInitialGain(dev);
            }
        }
        else if ( (CurrRetryRate<15) && (priv->LastRetryRate<15) )
        {
            priv->CurrentOperaRate = 48;

            if (priv->InitialGain != 5 )
            {
                priv->InitialGain = 5;
                UpdateInitialGain(dev);
            }
        }
    }
    else if ( priv->CurrentOperaRate == 22 )
    {
        if (CurrRetryRate>299 )
        {
            priv->TryDownCountLowData++;
            if (priv->TryDownCountLowData>3)
            {
                priv->TryDownCountLowData = 0;
                priv->CurrentOperaRate = 11;


                if (priv->InitialGain != 3 )
                {
                    priv->InitialGain = 3;
                    UpdateInitialGain(dev);
                }
            }
        }
        else if ( (CurrRetryRate<45) )
        {
            priv->TryDownCountLowData = 0;
            priv->TryupingCount++;
            if ( (priv->bTryuping==false)&& (priv->TryupingCount>20) )
            {
                priv->TryupingCount = 0;
                priv->bTryuping = true;
                priv->CurrTxRate =  priv->CurrentOperaRate;
                priv->CurrRetryRate = CurrRetryRate;

                priv->CurrentOperaRate = 36;
            }
        }
    }
    else if ( priv->CurrentOperaRate == 11 )
    {
        if (CurrRetryRate>199 )
        {
            priv->TryDownCountLowData++;
            if (priv->TryDownCountLowData>3)
            {
                priv->TryDownCountLowData = 0;
                priv->CurrentOperaRate = 4;


                if (priv->InitialGain != 3 )
                {
                    priv->InitialGain = 3;
                    UpdateInitialGain(dev);
                }
            }
        }
        else if ( (CurrRetryRate<30) && (priv->LastRetryRate<30) )
        {
            priv->TryDownCountLowData = 0;
            priv->CurrentOperaRate = 22;

            if (priv->InitialGain  != 4 )
            {
                priv->InitialGain  = 4;
                UpdateInitialGain(dev);
            }
        }
    }
    else if ( priv->CurrentOperaRate == 4 )
    {
        if((CurrRetryRate>99) && (priv->LastRetryRate>99) )
        {
            priv->TryDownCountLowData++;
            if (priv->TryDownCountLowData>3)
            {
                priv->CurrentOperaRate = 2;


                if (priv->InitialGain != 3 )
                {
                    priv->InitialGain = 3;
                    UpdateInitialGain(dev);
                }
            }
        }
        else if ( (CurrRetryRate<50) && (priv->LastRetryRate<50) )
        {
            priv->TryDownCountLowData=0;
            priv->CurrentOperaRate = 11;
        }
    }
    else if ( priv->CurrentOperaRate == 2 )
    {
        if ( (CurrRetryRate<50) && (priv->LastRetryRate<50) )
        {
            priv->CurrentOperaRate = 4;
        }
    }

end:
        priv->LastRetryRate = CurrRetryRate;
        priv->ieee80211->rate = priv->CurrentOperaRate * 5;
}



#ifdef RATE_ADAPTIVE
#if LINUX_VERSION_CODE >=KERNEL_VERSION(2,6,20)
void rtl8180_rate_adapter(struct work_struct * work)
{
    struct delayed_work *dwork = container_of(work,struct delayed_work,work);
    struct ieee80211_device *ieee = container_of(dwork,struct ieee80211_device,rate_adapter_wq);
    struct net_device *dev = ieee->dev;
#else
    void rtl8180_rate_adapter(struct net_device *dev)
    {

#endif
        sta_rateadaptive8187(dev);
}
#endif

#ifdef RATE_ADAPTIVE
void timer_rate_adaptive(unsigned long data)
{
    struct r8180_priv* priv = ieee80211_priv((struct net_device *)data);
        if(!priv->up)
        {
                return;
        }
        if(     (priv->ieee80211->mode != IEEE_B) &&
                (priv->ieee80211->iw_mode != IW_MODE_MASTER)
                && (priv->ieee80211->state == IEEE80211_LINKED))
        {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
        queue_delayed_work(priv->ieee80211->wq, &priv->ieee80211->rate_adapter_wq, 0);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        queue_work(priv->ieee80211->wq, (void *)&priv->ieee80211->rate_adapter_wq);
#else
        schedule_task(&priv->ieee80211->rate_adapter_wq);
#endif
        }
        mod_timer(&priv->rateadapter_timer, jiffies + MSECS(DEFAULT_RATE_ADAPTIVE_TIMER_PERIOD));
}
#endif

void
InactivePowerSave(
    struct net_device *dev
    )
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    priv->bSwRfProcessing = true;
    MgntActSet_RF_State(dev, priv->eInactivePowerState, RF_CHANGE_BY_IPS);

#if 0
    while( index < 4 )
    {
        if( ( pMgntInfo->SecurityInfo.PairwiseEncAlgorithm == WEP104_Encryption ) ||
            (pMgntInfo->SecurityInfo.PairwiseEncAlgorithm == WEP40_Encryption) )
        {
            if( pMgntInfo->SecurityInfo.KeyLen[index] != 0)
            pAdapter->HalFunc.SetKeyHandler(pAdapter, index, 0, FALSE, pMgntInfo->SecurityInfo.PairwiseEncAlgorithm, TRUE, FALSE);

        }
        index++;
    }
#endif
    priv->bSwRfProcessing = false;
}

void
IPSEnter(
    struct net_device *dev
    )
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    RT_RF_POWER_STATE rtState;
    printk("==============================>enter IPS\n");
    if (priv->bInactivePs)
    {
        rtState = priv->eRFPowerState;

        if (rtState == eRfOn && !priv->bSwRfProcessing && (priv->ieee80211->iw_mode != IW_MODE_ADHOC)
            && (priv->ieee80211->state != IEEE80211_LINKED ))
        {
            priv->eInactivePowerState = eRfOff;
            InactivePowerSave(dev);
        }
    }
}
void
IPSLeave(
    struct net_device *dev
    )
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    RT_RF_POWER_STATE rtState;
    if (priv->bInactivePs)
    {
        rtState = priv->eRFPowerState;
        if ((rtState == eRfOff || rtState == eRfSleep) && (!priv->bSwRfProcessing) && priv->RfOffReason <= RF_CHANGE_BY_IPS)
        {
            priv->eInactivePowerState = eRfOn;
            InactivePowerSave(dev);
        }
    }
}
#ifdef DIG
#if 1
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_watch_dog_wq (struct work_struct *work)
{
        struct delayed_work *dwork = container_of(work,struct delayed_work,work);
        struct ieee80211_device *ieee = container_of(dwork,struct ieee80211_device,watch_dog_wq);
        struct net_device *dev = ieee->dev;
#else
void rtl8180_watch_dog_wq(struct net_device *dev)
{
#endif
#endif


    struct r8180_priv *priv = ieee80211_priv(dev);
    bool bEnterPS;
    bool bBusyTraffic;

    bEnterPS = false;
    bBusyTraffic = false;
#if 1
    if(priv->ieee80211->actscanning == false){
        if((priv->ieee80211->state == IEEE80211_NOLINK) && (priv->ieee80211->beinretry == false) && (priv->eRFPowerState == eRfOn) && (priv->ieee80211->iw_mode == IW_MODE_INFRA )){
        down(&priv->ieee80211->ips_sem);
        up(&priv->ieee80211->ips_sem);
}
    }
#endif
#if 0
    if(priv->ieee80211->state == IEEE80211_LINKED){
        if( priv->NumRxOkInPeriod> 666 ||
            priv->NumTxOkInPeriod > 666 )
        {
            bBusyTraffic = true;
        }

        if((priv->ieee80211->NumRxData + priv->NumTxOkInPeriod)<8)
        {
            bEnterPS= true;
        }
        if(bEnterPS)
        {
            LeisurePSEnter(priv);
        }
        else
        {
            LeisurePSLeave(priv);
        }
    }
    else
    {
        LeisurePSLeave(priv);
    }

    priv->NumRxOkInPeriod = 0;
    priv->NumTxOkInPeriod = 0;
    priv->ieee80211->NumRxData = 0;
#endif
}
#endif
#ifdef DIG
void watch_dog_adaptive(unsigned long data)
{
    struct r8180_priv* priv = ieee80211_priv((struct net_device *)data);
    struct net_device* dev = priv->ieee80211->dev;
    if(!priv->up)
    {
        return;
    }
    if(CheckHighPower(dev))
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
        queue_delayed_work(priv->ieee80211->wq, &priv->ieee80211->tx_pw_wq, 0);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        queue_work(priv->ieee80211->wq, (void *)&priv->ieee80211->tx_pw_wq);
#else
        schedule_task(&priv->ieee80211->tx_pw_wq);
#endif
    }

    if(CheckDig(dev) == true)
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
        queue_delayed_work(priv->ieee80211->wq, &priv->ieee80211->hw_dig_wq, 0);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        queue_work(priv->ieee80211->wq, (void *)&priv->ieee80211->hw_dig_wq);
#else
        schedule_task(&priv->ieee80211->hw_dig_wq);
#endif

    }

    mod_timer(&priv->watch_dog_timer, jiffies + MSECS(IEEE80211_WATCH_DOG_TIME));
}
#endif

#ifdef POLLING_METHOD_FOR_RADIO
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void GPIOChangeRFWorkItemCallBack(struct work_struct *work);
#else
void GPIOChangeRFWorkItemCallBack(struct net_device *dev);
#endif
void gpio_change_polling(unsigned long data);
#endif


void rtl8180_irq_rx_tasklet(struct r8180_priv *priv);
short rtl8180_init(struct net_device *dev)
{

    struct r8180_priv *priv = ieee80211_priv(dev);
    int i, j;
    u16 word;
    int ch;
    u8 chan_plan_index;
#ifdef SW_ANTE_DIVERSITY
    u16 EEPROMId;
#endif


#if 0
    if(!channels){
        DMESG("No channels, aborting");
        return -1;
    }
    ch=channels;
    priv->channel_plan = 0;

    for (i=1; i<=14; i++) {
        (priv->ieee80211->channel_map)[i] = (u8)(ch & 0x01);
        ch >>= 1;
    }
#endif


    priv->stats.txrdu=0;
    priv->stats.txnperr=0;
    priv->stats.txresumed=0;
    priv->stats.rxerr=0;
    priv->stats.rxint=0;
    priv->stats.txnpokint=0;
    priv->stats.txhpokint=0;
    priv->stats.txhperr=0;
    priv->stats.rxurberr=0;
    priv->stats.rxstaterr=0;
    priv->stats.txoverflow=0;
    priv->stats.rxok=0;
    priv->stats.txok=0;
    priv->stats.txerr=0;
    priv->stats.txbeaconerr=0;
    priv->stats.txbeaconokint=0;
    priv->stats.txretry=0;
    priv->stats.txlperr=0;
    priv->stats.txlpokint=0;
    priv->stats.rxretry=0;
    priv->stats.rxcrcerrmin=0;
    priv->stats.rxcrcerrmid=0;
    priv->stats.rxcrcerrmax=0;
    priv->stats.rxicverr=0;

    priv->ps_sleep_finish=0;
    priv->ieee80211->iw_mode = IW_MODE_INFRA;

    priv->retry_rts = DEFAULT_RETRY_RTS;
    priv->retry_data = DEFAULT_RETRY_DATA;
    priv->ieee80211->rate = 110;
    priv->CurrentOperaRate=priv->ieee80211->rate/5;
    priv->ieee80211->short_slot = 1;
    priv->ieee80211->mode = IEEE_G|IEEE_B;
    priv->promisc = (dev->flags & IFF_PROMISC) ? 1:0;
    spin_lock_init(&priv->tx_lock);
    spin_lock_init(&priv->rf_ps_lock);
        priv->LastSignalStrengthInPercent=0;
        priv->SignalStrength=0;
        priv->SignalQuality=0;
        priv->antenna_flag=0;
        priv->flag_beacon = false;
    priv->radion = 1;
    priv->ScanAllowedNow = 1;
    priv->CurrRetryCnt=0;
    priv->LastRetryCnt=0;
        priv->LastTxokCnt=0;
        priv->LastRxokCnt=0;
        priv->LastRetryRate=0;
        priv->bTryuping=0;
        priv->CurrTxRate=0;
        priv->CurrRetryRate=0;
        priv->TryupingCount=0;
        priv->TryupingCountNoData=0;
        priv->TryDownCountLowData=0;
        priv->RecvSignalPower=0;
        priv->LastTxOKBytes=0;
        priv->LastFailTxRate=0;
        priv->LastFailTxRateSS=0;
        priv->FailTxRateCount=0;
        priv->LastTxThroughput=0;
        priv->txokbytestotal=0;
        priv->RFChangeInProgress = false;
        priv->SetRFPowerStateInProgress = false;
        priv->RFProgType = 0;
        priv->bInHctTest = false;
        priv->bInactivePs = true;
        priv->bSwRfProcessing = false;
        priv->eRFPowerState = eRfOff;
        priv->RfOffReason = 0;
        priv->NumRxOkInPeriod = 0;
        priv->NumTxOkInPeriod = 0;
        priv->bLeisurePs = true;
    priv->ps_mode = false;

    priv->bDigMechanism = 1;
    priv->bCCKThMechanism = 0;
    priv->InitialGain = 0;
    priv->StageCCKTh = 0;
    priv->RegBModeGainStage = 2;
    priv->RegDigOfdmFaUpTh = 0x0c;
    priv->RegBModeGainStage = 0x02;
    priv->DIG_NumberFallbackVote = 0;
    priv->DIG_NumberUpgradeVote = 0;
    priv->CCKUpperTh = 0x100;
    priv->CCKLowerTh = 0x20;
    priv->bRegHighPowerMechanism = true;
    priv->bToUpdateTxPwr = false;

    priv->Z2HiPwrUpperTh = 77;
    priv->Z2HiPwrLowerTh = 75;
    priv->Z2RSSIHiPwrUpperTh = 70;
    priv->Z2RSSIHiPwrLowerTh = 20;
    priv->wMacRegRfPinsOutput = 0x0480;
    priv->wMacRegRfPinsSelect = 0x2488;
    priv->TrSwitchState = TR_HW_CONTROLLED;

#ifdef SW_ANTE_DIVERSITY
    priv->antb=0;
    priv->diversity=1;
        priv->LastRxPktAntenna = 0;
    priv->AdMinCheckPeriod = 5;
    priv->AdMaxCheckPeriod = 10;
    priv->AdMaxRxSsThreshold = 30;
    priv->AdRxSsThreshold = 20;
    priv->AdCheckPeriod = priv->AdMinCheckPeriod;
    priv->AdTickCount = 0;
    priv->AdRxSignalStrength = -1;
    priv->RegSwAntennaDiversityMechanism = 0;
    priv->RegDefaultAntenna = 0;
    priv->SignalStrength = 0;
    priv->AdRxOkCnt = 0;
    priv->CurrAntennaIndex = 0;
    priv->AdRxSsBeforeSwitched = 0;
    init_timer(&priv->SwAntennaDiversityTimer);
    priv->SwAntennaDiversityTimer.data = (unsigned long)dev;
    priv->SwAntennaDiversityTimer.function = (void *)SwAntennaDiversityTimerCallback;
#endif

#ifdef POLLING_METHOD_FOR_RADIO
    priv->wlan_first_up_flag1 = 0;
    priv->iwpriv_radio_on = 1;
#endif
#ifdef LED
    priv->ieee80211->ieee80211_led_contorl = LedControl8187;
#endif
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
#ifdef POLLING_METHOD_FOR_RADIO
    INIT_DELAYED_WORK(&priv->ieee80211->GPIOChangeRFWorkItem,(void*) GPIOChangeRFWorkItemCallBack);
#endif

#ifdef SW_ANTE_DIVERSITY
    INIT_DELAYED_WORK(&priv->ieee80211->SwAntennaWorkItem,(void*) SwAntennaWorkItemCallback);
#endif


#ifdef RATE_ADAPTIVE
    INIT_DELAYED_WORK(&priv->ieee80211->rate_adapter_wq,(void*)rtl8180_rate_adapter);
#endif

#ifdef DIG
    INIT_DELAYED_WORK(&priv->ieee80211->watch_dog_wq,(void*)rtl8180_watch_dog_wq);
    INIT_DELAYED_WORK(&priv->ieee80211->hw_dig_wq,(void*)rtl8180_hw_dig_wq);
    INIT_DELAYED_WORK(&priv->ieee80211->tx_pw_wq,(void*)rtl8180_tx_pw_wq);
#endif
    INIT_WORK(&priv->ieee80211->ps_request_tx_ack_wq,(void*)rtl8180_rq_tx_ack);
    INIT_WORK(&priv->ieee80211->hw_wakeup_wq,(void*)rtl8180_hw_wakeup);
    INIT_WORK(&priv->ieee80211->hw_sleep_wq,(void*)rtl8180_hw_sleep);
    INIT_WORK(&priv->reset_wq,(void*) rtl8180_restart);

#else

#ifdef POLLING_METHOD_FOR_RADIO
    INIT_WORK(&priv->ieee80211->GPIOChangeRFWorkItem,(void*) GPIOChangeRFWorkItemCallBack, dev);
#endif

#ifdef SW_ANTE_DIVERSITY
    INIT_WORK(&priv->ieee80211->SwAntennaWorkItem,(void*) SwAntennaWorkItemCallback, dev);
#endif

#ifdef RATE_ADAPTIVE
    INIT_WORK(&priv->ieee80211->rate_adapter_wq,(void*)rtl8180_rate_adapter,dev);
#endif

#ifdef DIG
    INIT_WORK(&priv->ieee80211->watch_dog_wq,(void*)rtl8180_watch_dog_wq,dev);
    INIT_WORK(&priv->ieee80211->hw_dig_wq,(void*)rtl8180_hw_dig_wq,dev);
    INIT_WORK(&priv->ieee80211->tx_pw_wq,(void*)rtl8180_tx_pw_wq,dev);
#endif
    INIT_WORK(&priv->ieee80211->ps_request_tx_ack_wq,(void*)rtl8180_rq_tx_ack,dev);
    INIT_WORK(&priv->ieee80211->hw_wakeup_wq,(void*)rtl8180_hw_wakeup,dev);
    INIT_WORK(&priv->ieee80211->hw_sleep_wq,(void*)rtl8180_hw_sleep,dev);
    INIT_WORK(&priv->reset_wq,(void*) rtl8180_restart,dev);
#endif

#else
#ifdef POLLING_METHOD_FOR_RADIO
    tq_init(&priv->ieee80211->GPIOChangeRFWorkItem,(void*) GPIOChangeRFWorkItemCallBack, dev);
#endif

#ifdef SW_ANTE_DIVERSITY
    tq_init(&priv->ieee80211->SwAntennaWorkItem,(void*) SwAntennaWorkItemCallback, dev);
#endif

#ifdef RATE_ADAPTIVE
    tq_init(&priv->ieee80211->rate_adapter_wq,(void*)rtl8180_rate_adapter,dev);
#endif

#ifdef DIG
    tq_init(&priv->ieee80211->watch_dog_wq,(void*)rtl8180_watch_dog_wq,dev);
    tq_init(&priv->ieee80211->hw_dig_wq,(void*)rtl8180_hw_dig_wq,dev);
    tq_init(&priv->ieee80211->tx_pw_wq,(void*)rtl8180_tx_pw_wq,dev);
#endif

    tq_init(&priv->ieee80211->ps_request_tx_ack_wq,(void*)rtl8180_rq_tx_ack,dev);
    tq_init(&priv->ieee80211->hw_wakeup_wq,(void*)rtl8180_hw_wakeup,dev);
    tq_init(&priv->ieee80211->hw_sleep_wq,(void*)rtl8180_hw_sleep,dev);
    tq_init(&priv->reset_wq,(void*) rtl8180_restart,dev);
#endif

#ifdef POLLING_METHOD_FOR_RADIO
    init_timer(&priv->gpio_polling_timer);
    priv->gpio_polling_timer.data = (unsigned long)dev;
    priv->gpio_polling_timer.function = gpio_change_polling;
#endif
#ifdef RATE_ADAPTIVE
    init_timer(&priv->rateadapter_timer);
        priv->rateadapter_timer.data = (unsigned long)dev;
        priv->rateadapter_timer.function = timer_rate_adaptive;
#endif
#ifdef DIG
    init_timer(&priv->watch_dog_timer);
    priv->watch_dog_timer.data = (unsigned long)dev;
    priv->watch_dog_timer.function = watch_dog_adaptive;
#endif
    sema_init(&priv->wx_sem,1);
    sema_init(&priv->power_sem,1);

    tasklet_init(&priv->irq_rx_tasklet,
             (void(*)(unsigned long))rtl8180_irq_rx_tasklet,
             (unsigned long)priv);

    init_timer(&priv->ps_timer);
    priv->ps_timer.data = (unsigned long)dev;
    priv->ps_timer.function = timer_hw_wakeup_wq;

    priv->ieee80211->current_network.beacon_interval = DEFAULT_BEACONINTERVAL;
    priv->ieee80211->iw_mode = IW_MODE_INFRA;
    priv->ieee80211->softmac_features  = IEEE_SOFTMAC_SCAN |
        IEEE_SOFTMAC_ASSOCIATE | IEEE_SOFTMAC_PROBERQ |
        IEEE_SOFTMAC_PROBERS | IEEE_SOFTMAC_TX_QUEUE |
        IEEE_SOFTMAC_BEACONS |
        IEEE_SOFTMAC_SINGLE_QUEUE;

    priv->ieee80211->active_scan = 1;
    priv->ieee80211->rate = 110;
    priv->CurrentOperaRate=priv->ieee80211->rate/5;
    priv->ieee80211->modulation = IEEE80211_CCK_MODULATION | IEEE80211_OFDM_MODULATION;
    priv->ieee80211->host_encrypt = 1;
    priv->ieee80211->host_decrypt = 1;
    priv->ieee80211->ps_is_queue_empty = rtl8180_is_tx_queue_empty;
    priv->ieee80211->start_send_beacons = NULL;
    priv->ieee80211->stop_send_beacons = NULL;
    priv->ieee80211->softmac_hard_start_xmit = NULL;
    priv->ieee80211->set_chan = rtl8180_set_chan;
    priv->ieee80211->link_change = rtl8187_link_change;
    priv->ieee80211->softmac_data_hard_start_xmit = rtl8180_hard_data_xmit;
    priv->ieee80211->data_hard_stop = rtl8180_data_hard_stop;
    priv->ieee80211->data_hard_resume = rtl8180_data_hard_resume;

    priv->ieee80211->fts = DEFAULT_FRAG_THRESHOLD;

    priv->card_8185 = 2;
    priv->phy_ver = 2;
    priv->card_type = USB;

    priv->card_8187 = NIC_8187;
    if (NIC_8187 == priv->card_8187){
        u8 hw_version;
        hw_version =( read_nic_dword(dev, TCR) & TCR_HWVERID_MASK)>>TCR_HWVERID_SHIFT;
        switch (hw_version) {
            case 0x06:
                break;
            case 0x05:
                priv->card_8187_Bversion = VERSION_8187_D;
                break;
            default:
                priv->card_8187_Bversion = VERSION_8187_B;
                break;
        }
    }

#ifdef SW_ANTE_DIVERSITY
        EEPROMId = eprom_read(dev, 0);
        if(EEPROMId != 0x8129)
        {
            priv->AutoloadFailFlag = true;
        }
        else
        {
            priv->AutoloadFailFlag = false;
        }

    priv->EEPROMVersion = eprom_read(dev, (u16) (EEPROM_VERSION>>1) );

#if 0
    if( (!priv->card_8185) && priv->EEPROMVersion < 0x0101)
    {
        priv->diversity=1;
    }
    else
    {
        if(!priv->card_8185)
        {
            u8 rfparam = eprom_read(dev,RF_PARAM);
            priv->antb =  rfparam & (1<<RF_PARAM_ANTBDEFAULT_SHIFT) ? 1:0;
            priv->diversity = (read_nic_byte(dev,CONFIG2)&(1<<CONFIG2_ANTENNA_SHIFT)) ? 1:0;
        }
    }

    {
        if(priv->antb)
            DMESG ("Antenna B is default antenna");
        else
            DMESG ("Antenna A is default antenna");

        if(priv->diversity)
            DMESG ("Antenna diversity is enabled");
        else
            DMESG("Antenna diversity is disabled");
    }
#endif
    if(priv->EEPROMVersion <= 0x0101)
    {
        priv->EEPROMAntennaDiversity=true;
        priv->EEPROMCSThreshold=0x0c;
        priv->EEPROMDefaultAntennaB=false;
        priv->EEPROMCSMethod=RCR_EnCS1;
        priv->EEPROMDigitalPhy=true;

    }
    else
    {
        u16 usValue;

        usValue = eprom_read(dev, (u16) (EEPROM_CONFIG2>>1) ) & 0xff;
        if(usValue&0x40)
            priv->EEPROMAntennaDiversity=true;
        else
            priv->EEPROMAntennaDiversity=false;

        usValue = eprom_read(dev, (u16) (EEPROM_CS_THRESHOLD>>1) ) >> 8;
        priv->EEPROMCSThreshold=usValue;

        if(priv->EEPROMVersion<=0x0104)
            usValue = eprom_read(dev, (u16) (EEPROM_RF_PARAM>>1) ) & 0xff;
        else
            usValue = eprom_read(dev, (u16) (EEPROM_RF_PARAM+4)>>1) & 0xff;

        if(usValue&0x01)
            priv->EEPROMDigitalPhy=false;
        else
            priv->EEPROMDigitalPhy=true;
        if(usValue&0x02)
            priv->EEPROMDefaultAntennaB=true;
        else
            priv->EEPROMDefaultAntennaB=false;
        switch((usValue&0x0c)>>2)
        {
        case 1:
            priv->EEPROMCSMethod=RCR_EnCS1;
            break;
        case 2:
            priv->EEPROMCSMethod=RCR_EnCS2;
            break;
        default:
            priv->EEPROMCSMethod=0;
            break;
        }

        if( priv->EEPROMVersion>=0x0103 )
        {
            if( !priv->AutoloadFailFlag )
            {
                priv->EEPROMGEPRFOffState = (usValue&0x10)?1:0;
            }
        }
    }
#endif
    #if 0
    hw_version =( read_nic_dword(dev, TCR) & TCR_HWVERID_MASK)>>TCR_HWVERID_SHIFT;

    switch (hw_version){
        case HW_VERID_R8185_ABC:
            DMESG("MAC controller is a RTL8185 b/g");
            priv->card_8185 = 1;
            priv->phy_ver = 2;
            break;

        case HW_VERID_R8185_D:
            DMESG("MAC controller is a RTL8185 b/g (V. D)");
            priv->card_8185 = 2;
            priv->phy_ver = 2;
            break;

        case HW_VERID_R8180_ABCD:
            DMESG("MAC controller is a RTL8180");
            priv->card_8185 = 0;
            break;

        case HW_VERID_R8180_F:
            DMESG("MAC controller is a RTL8180 (v. F)");
            priv->card_8185 = 0;
            break;

        default:
            DMESGW("MAC chip not recognized: version %x. Assuming RTL8180",hw_version);
            priv->card_8185 = 0;
            break;
    }


    priv->card_8185_Bversion = 0;

    config3 = read_nic_byte(dev, CONFIG3);
    if(config3 & 0x8){
        priv->card_type = CARDBUS;
        DMESG("This is a CARDBUS NIC");
    }
    else if( config3 & 0x4){
        priv->card_type = MINIPCI;
        DMESG("This is a MINI-PCI NIC");
    }else{
        priv->card_type = PCI;
        DMESG("This is a PCI NIC");
    }
    #endif
    priv->enable_gpio0 = 0;
    priv->plcp_preamble_mode = 2;



    #if 0

    u8 txcr, txreg50;
    u32 txreg54, txreg60;

    txcr = read_nic_byte(dev, 0x59);
    write_nic_byte(dev, 0x59, 0x44);
    txcr = read_nic_byte(dev, 0x59);

        txreg50 = read_nic_byte(dev, 0x50);
        write_nic_byte(dev, 0x50, 0xc0);
        txreg50 = read_nic_byte(dev, 0x50);


        txreg54 = read_nic_dword(dev, 0x54);
        txreg54 = 0xa0000a59;
        write_nic_dword(dev, 0x54, txreg54);
        txreg54 = read_nic_dword(dev, 0x54);

        txreg60 = read_nic_dword(dev, 0x60);

        write_nic_byte(dev, 0x50, 0x0);
        txcr = read_nic_byte(dev, 0x50);

#endif

    if (RCR_9356SEL & read_nic_dword(dev, RCR)){
        priv->epromtype=EPROM_93c56;
        DMESG("Reported EEPROM chip is a 93c56 (2Kbit)");
    }else{
        priv->epromtype=EPROM_93c46;
        DMESG("Reported EEPROM chip is a 93c46 (1Kbit)");
    }

    chan_plan_index = eprom_read(dev,EPROM_CHANNEL_PLAN) & 0xff;
    if(chan_plan_index & 0x80) {
        chan_plan_index &= 0x7f;
        if ((DefaultChannelPlan[chan_plan_index].Len != 0)&&(chan_plan_index < sizeof(DefaultChannelPlan)/sizeof(DefaultChannelPlan[0]))){
            for (i=0;i<DefaultChannelPlan[chan_plan_index].Len;i++)
                priv->ieee80211->channel_map[DefaultChannelPlan[chan_plan_index].Channel[i]] = 1;
        } else {
            DMESG("No channels, aborting");
            return -1;
        }
    } else {
        if(!channels){
            DMESG("No channels, aborting");
            return -1;
        }
        ch=channels;
        for (i=1; i<=14; i++) {
            (priv->ieee80211->channel_map)[i] = (u8)(ch & 0x01);
            ch >>= 1;
        }
    }

    dev->dev_addr[0]=eprom_read(dev,MAC_ADR) & 0xff;
    dev->dev_addr[1]=(eprom_read(dev,MAC_ADR) & 0xff00)>>8;
    dev->dev_addr[2]=eprom_read(dev,MAC_ADR+1) & 0xff;
    dev->dev_addr[3]=(eprom_read(dev,MAC_ADR+1) & 0xff00)>>8;
    dev->dev_addr[4]=eprom_read(dev,MAC_ADR+2) & 0xff;
    dev->dev_addr[5]=((eprom_read(dev,MAC_ADR+2) & 0xff00)>>8) + 1;

    DMESG("Card MAC address is "MAC_FMT, MAC_ARG(dev->dev_addr));

    for(i=1,j=0; i<6; i+=2,j++){

        word = eprom_read(dev,EPROM_TXPW0 + j);
        priv->chtxpwr[i]=word & 0xf;
        priv->chtxpwr_ofdm[i]=(word & 0xf0)>>4;
        priv->chtxpwr[i+1]=(word & 0xf00)>>8;
        priv->chtxpwr_ofdm[i+1]=(word & 0xf000)>>12;
    }

    for(i=1,j=0; i<4; i+=2,j++){

        word = eprom_read(dev,EPROM_TXPW1 + j);
        priv->chtxpwr[i+6]=word & 0xf;
        priv->chtxpwr_ofdm[i+6]=(word & 0xf0)>>4;
        priv->chtxpwr[i+6+1]=(word & 0xf00)>>8;
        priv->chtxpwr_ofdm[i+6+1]=(word & 0xf000)>>12;
    }

    for(i=1,j=0; i<4; i+=2,j++){

        word = eprom_read(dev,EPROM_TXPW2 + j);
        priv->chtxpwr[i+6+4]=word & 0xf;
        priv->chtxpwr_ofdm[i+6+4]=(word & 0xf0)>>4;
        priv->chtxpwr[i+6+4+1]=(word & 0xf00)>>8;
        priv->chtxpwr_ofdm[i+6+4+1]=(word & 0xf000)>>12;
    }


    priv->rf_chip = 0xff & eprom_read(dev,EPROM_RFCHIPID);

    word = eprom_read(dev,EPROM_TXPW_BASE);
    priv->cck_txpwr_base = word & 0xf;
    priv->ofdm_txpwr_base = (word>>4) & 0xf;


    switch (priv->rf_chip) {

        case EPROM_RFCHIPID_RTL8225U:

        DMESG("Card reports RF frontend Realtek 8225");
        DMESGW("**PLEASE** REPORT SUCCESS/INSUCCESS TO Realtek");
        if(rtl8225_is_V_z2(dev)){
            priv->rf_init = rtl8225z2_rf_init;
            priv->rf_set_chan = rtl8225z2_rf_set_chan;
            priv->rf_set_sens = NULL;
                        priv->rf_chip = RF_ZEBRA2;
            DMESG("This seems a new V2 radio: rtl8225_zebra2");
        }else{
            priv->rf_init = rtl8225_rf_init;
            priv->rf_set_chan = rtl8225_rf_set_chan;
            priv->rf_set_sens = rtl8225_rf_set_sens;
            priv->rf_wakeup = rtl8225_rf_wakeup;
            priv->rf_sleep = rtl8225_rf_sleep;

                        priv->rf_chip = RF_ZEBRA;

            DMESG("This seems a legacy 1st version radio:rtl8225_zebra");
        }
        priv->rf_close = rtl8225_rf_close;

        priv->max_sens = RTL8225_RF_MAX_SENS;
        priv->sens = RTL8225_RF_DEF_SENS;
        break;

        default:
        DMESGW("Unknown RF module %x",priv->rf_chip);
        DMESGW("Exiting...");
        return -1;

    }

    DMESG("PAPE from CONFIG2: %x",read_nic_byte(dev,CONFIG2)&0x7);

    if(rtl8187_usb_initendpoints(dev)!=0){
        DMESG("Endopoints initialization failed");
        return -ENOMEM;
    }
#if 0
    if (0!=alloc_rx_desc_ring(dev, priv->rxbuffersize, priv->rxringcount))
        return -ENOMEM;

    if (0!=alloc_tx_desc_ring(dev, priv->txbuffsize, priv->txringcount,
                  TX_NORMPRIORITY_RING_ADDR))
        return -ENOMEM;

    if (0!=alloc_tx_desc_ring(dev, priv->txbuffsize, priv->txringcount,
                  TX_HIGHPRIORITY_RING_ADDR))
        return -ENOMEM;

    if (0!=alloc_tx_desc_ring(dev, priv->txbuffsize, priv->txringcount,
                  TX_LOWPRIORITY_RING_ADDR))
        return -ENOMEM;


    if (0!=alloc_tx_beacon_desc_ring(dev, priv->txbeaconcount))
        return -ENOMEM;
#endif

#ifdef LED
    InitSwLeds(dev);
#endif
#ifdef DEBUG_EPROM
    dump_eprom(dev);
#endif
    return 0;

}

void rtl8185_rf_pins_enable(struct net_device *dev)
{
    write_nic_word(dev, RFPinsEnable, 0x1ff7);
}


void rtl8185_set_anaparam2(struct net_device *dev, u32 a)
{
    u8 conf3;

    rtl8180_set_mode(dev, EPROM_CMD_CONFIG);

    conf3 = read_nic_byte(dev, CONFIG3);
    write_nic_byte(dev, CONFIG3, conf3 | (1<<CONFIG3_ANAPARAM_W_SHIFT));

    write_nic_dword(dev, ANAPARAM2, a);

    conf3 = read_nic_byte(dev, CONFIG3);
    write_nic_byte(dev, CONFIG3, conf3 &~(1<<CONFIG3_ANAPARAM_W_SHIFT));

    rtl8180_set_mode(dev, EPROM_CMD_NORMAL);

}


void rtl8180_set_anaparam(struct net_device *dev, u32 a)
{
    u8 conf3;

    rtl8180_set_mode(dev, EPROM_CMD_CONFIG);

    conf3 = read_nic_byte(dev, CONFIG3);
    write_nic_byte(dev, CONFIG3, conf3 | (1<<CONFIG3_ANAPARAM_W_SHIFT));

    write_nic_dword(dev, ANAPARAM, a);

    conf3 = read_nic_byte(dev, CONFIG3);
    write_nic_byte(dev, CONFIG3, conf3 &~(1<<CONFIG3_ANAPARAM_W_SHIFT));

    rtl8180_set_mode(dev, EPROM_CMD_NORMAL);

}


void rtl8185_tx_antenna(struct net_device *dev, u8 ant)
{
    write_nic_byte(dev, TX_ANTENNA, ant);
    force_pci_posting(dev);
    mdelay(1);
}


void rtl8187_write_phy(struct net_device *dev, u8 adr, u32 data)
{
    u32 phyw;

    adr |= 0x80;

    phyw= ((data<<8) | adr);



    write_nic_byte(dev, 0x7f, ((phyw & 0xff000000) >> 24));
    write_nic_byte(dev, 0x7e, ((phyw & 0x00ff0000) >> 16));
    write_nic_byte(dev, 0x7d, ((phyw & 0x0000ff00) >> 8));
    write_nic_byte(dev, 0x7c, ((phyw & 0x000000ff) ));

#if 0
    for(i=0;i<10;i++){
        write_nic_dword(dev, PHY_ADR, 0xffffff7f & phyw);
        phyr = read_nic_byte(dev, PHY_READ);
        if(phyr == (data&0xff)) break;

    }
#endif
    mdelay(1);
}


inline void write_phy_ofdm (struct net_device *dev, u8 adr, u32 data)
{
    data = data & 0xff;
    rtl8187_write_phy(dev, adr, data);
}


void write_phy_cck (struct net_device *dev, u8 adr, u32 data)
{
    data = data & 0xff;
    rtl8187_write_phy(dev, adr, data | 0x10000);
}


void rtl8180_adapter_start(struct net_device *dev)
{
        struct r8180_priv *priv = ieee80211_priv(dev);

    rtl8180_reset(dev);

    write_nic_byte(dev,0x85,0);
    write_nic_byte(dev,0x91,0);

    write_nic_byte(dev,0x85,4);

    priv->irq_mask = 0xffff;

    rtl8180_set_mode(dev, EPROM_CMD_CONFIG);
    write_nic_dword(dev, MAC0, ((u32*)dev->dev_addr)[0]);
    write_nic_word(dev, MAC4, ((u32*)dev->dev_addr)[1] & 0xffff );

    rtl8180_set_mode(dev, EPROM_CMD_NORMAL);
    rtl8180_update_msr(dev);

    write_nic_dword(dev,INT_TIMEOUT,0);

#ifdef DEBUG_REGISTERS
    rtl8180_dump_reg(dev);
#endif


    write_nic_byte(dev, WPA_CONFIG, 0);

    write_nic_byte(dev, RATE_FALLBACK, 0x81);
    rtl8187_set_rate(dev);

    priv->rf_init(dev);

    if(priv->rf_set_sens != NULL)
        priv->rf_set_sens(dev,priv->sens);

    write_nic_word(dev,0x5e,1);

    #if 1
    write_nic_word(dev,0xfe,0x10);
    #endif
    write_nic_byte(dev, TALLY_SEL, 0x80);

    write_nic_byte(dev, 0xff, 0x60);

    write_nic_word(dev,0x5e,0);

    write_nic_byte(dev,0x85,4);

    rtl8180_irq_enable(dev);


}



#if 0
void rtl8180_start_tx_beacon(struct net_device *dev)
{
    int i;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    u16 word;
    DMESG("Enabling beacon TX");
    rtl8180_irq_disable(dev);
    rtl8180_set_mode(dev,EPROM_CMD_CONFIG);


    word  = read_nic_word(dev, BcnItv);
    word &= ~BcnItv_BcnItv;
    write_nic_word(dev, BcnItv, word);

    write_nic_word(dev, AtimWnd,
               read_nic_word(dev, AtimWnd) &~ AtimWnd_AtimWnd);

    word  = read_nic_word(dev, BintrItv);
    word &= ~BintrItv_BintrItv;


    write_nic_word(dev, BintrItv, word);

    for(i=0; i<ETH_ALEN; i++)
        write_nic_byte(dev, BSSID+i, priv->ieee80211->beacon_cell_ssid[i]);




    rtl8180_set_mode(dev, EPROM_CMD_NORMAL);

    rtl8180_irq_enable(dev);

}
#endif
/***************************************************************************
    -------------------------------NET STUFF---------------------------
***************************************************************************/
static struct net_device_stats *rtl8180_stats(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);

    return &priv->ieee80211->stats;
}


int _rtl8180_up(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);

    priv->up=1;


    rtl8180_adapter_start(dev);

#ifdef LED
    if(priv->ieee80211->bHwRadioOff == false)
        priv->ieee80211->ieee80211_led_contorl(dev,LED_CTL_POWER_ON);
#endif

    rtl8180_rx_enable(dev);

    rtl8180_tx_enable(dev);
        timer_rate_adaptive((unsigned long)dev);
    watch_dog_adaptive((unsigned long)dev);

#ifdef SW_ANTE_DIVERSITY
            DMESG("SW Antenna Diversity Enable!");
            SwAntennaDiversityTimerCallback(dev);
#endif

    ieee80211_softmac_start_protocol(priv->ieee80211);


    ieee80211_reset_queue(priv->ieee80211);

    if(!netif_queue_stopped(dev))
        netif_start_queue(dev);
    else
        netif_wake_queue(dev);


    return 0;
}


int rtl8180_open(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    int ret;

    if(priv->ieee80211->bHwRadioOff == false){
        DMESG("rtl8180_open process");
        down(&priv->wx_sem);
        ret = rtl8180_up(dev);
        up(&priv->wx_sem);
        return ret;
    }else{
        DMESG("ifconfig wlan0 up will be denied because Radio Off");
        DMESG("Please turn Radio On example: Fn+F1");
        return -1;
    }
}


int rtl8180_up(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);

    if (priv->up == 1) return -1;

    return _rtl8180_up(dev);
}


int rtl8180_close(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    int ret = 0;

    if (priv->up == 0) return -1;
#if 1
    down(&priv->wx_sem);
    DMESG("rtl8180_close process");
    ret = rtl8180_down(dev);
    up(&priv->wx_sem);
#endif
    return ret;

}

bool SetZebraRFPowerState8187(struct net_device *dev,RT_RF_POWER_STATE  eRFPowerState);
int rtl8180_down(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);

    if (priv->up == 0) return -1;

#ifdef LED
    priv->ieee80211->ieee80211_led_contorl(dev,LED_CTL_POWER_OFF);
#endif
    priv->up=0;
    ieee80211_softmac_stop_protocol(priv->ieee80211);

    if (!netif_queue_stopped(dev))
        netif_stop_queue(dev);

        priv->ieee80211->wpa_ie_len = 0;
        if(priv->ieee80211->wpa_ie)
            kfree(priv->ieee80211->wpa_ie);
        priv->ieee80211->wpa_ie = NULL;

    rtl8180_rtx_disable(dev);
    rtl8180_irq_disable(dev);
#ifdef RATE_ADAPTIVE
        del_timer_sync(&priv->rateadapter_timer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        cancel_delayed_work(&priv->ieee80211->rate_adapter_wq);
#endif
#endif
#ifdef DIG
    del_timer_sync(&priv->watch_dog_timer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    cancel_delayed_work(&priv->ieee80211->watch_dog_wq);
    cancel_delayed_work(&priv->ieee80211->hw_dig_wq);
    cancel_delayed_work(&priv->ieee80211->tx_pw_wq);
#endif
#endif
#ifdef SW_ANTE_DIVERSITY
    del_timer_sync(&priv->SwAntennaDiversityTimer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    cancel_delayed_work(&priv->ieee80211->SwAntennaWorkItem);
#endif
#endif
    if(priv->ieee80211->bHwRadioOff == false){
        SetZebraRFPowerState8187(dev, eRfOff);
    }
    memset(&(priv->ieee80211->current_network), 0, sizeof(struct ieee80211_network));
    priv->ieee80211->state = IEEE80211_NOLINK;

    return 0;
}


void rtl8180_commit(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);

    if (priv->up == 0) return ;

    ieee80211_softmac_stop_protocol(priv->ieee80211);
    sema_init(&priv->power_sem,1);
#ifdef RATE_ADAPTIVE
        del_timer_sync(&priv->rateadapter_timer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        cancel_delayed_work(&priv->ieee80211->rate_adapter_wq);
#endif
#endif
#ifdef DIG
        del_timer_sync(&priv->watch_dog_timer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        cancel_delayed_work(&priv->ieee80211->watch_dog_wq);
        cancel_delayed_work(&priv->ieee80211->hw_dig_wq);
        cancel_delayed_work(&priv->ieee80211->tx_pw_wq);
#endif
#endif
#ifdef SW_ANTE_DIVERSITY
    del_timer_sync(&priv->SwAntennaDiversityTimer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    cancel_delayed_work(&priv->ieee80211->SwAntennaWorkItem);
#endif
#endif
    ieee80211_softmac_stop_protocol(priv->ieee80211);

    rtl8180_irq_disable(dev);
    rtl8180_rtx_disable(dev);
    _rtl8180_up(dev);

#if 1
    if (priv->eRFPowerState == eRfOff)
    {
        printk("as commit need to set RF to re on\n");
    }
#endif
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void rtl8180_restart(struct work_struct *work)
{
    struct r8180_priv *priv = container_of(work, struct r8180_priv, reset_wq);
    struct net_device *dev = priv->dev;
#else
void rtl8180_restart(struct net_device *dev)
{

    struct r8180_priv *priv = ieee80211_priv(dev);
#endif
    down(&priv->wx_sem);

    rtl8180_commit(dev);

    up(&priv->wx_sem);
}

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,39))
static void r8180_set_multicast(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    short promisc;



    promisc = (dev->flags & IFF_PROMISC) ? 1:0;

    if (promisc != priv->promisc)

    priv->promisc = promisc;

}
#endif

int r8180_set_mac_adr(struct net_device *dev, void *mac)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    struct sockaddr *addr = mac;

    down(&priv->wx_sem);

    memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0))
    schedule_work(&priv->reset_wq);
#else
    schedule_task(&priv->reset_wq);
#endif

    up(&priv->wx_sem);

    return 0;
}

struct rx_desc_rtl8187 {

#ifdef _LINUX_BYTEORDER_LITTLE_ENDIAN_H

unsigned int    rxlen:12;
unsigned int    icv:1;
unsigned int    crc32:1;
unsigned int    pwrmgt:1;
unsigned int    res:1;
unsigned int    bar:1;
unsigned int    pam:1;
unsigned int    mar:1;
unsigned int    qos:1;
unsigned int    rxrate:4;
unsigned int    trsw:1;
unsigned int    splcp:1;
unsigned int    fovf:1;
unsigned int    dmafail:1;
unsigned int    last:1;
unsigned int    first:1;
unsigned int    eor:1;
unsigned int    own:1;

unsigned char   sq;
unsigned char   rssi:7;
unsigned char   antenna:1;

unsigned char   agc;
unsigned char   decrypted:1;
unsigned char   wakeup:1;
unsigned char   shift:1;
unsigned char   rsvd0:5;

unsigned int tsftl;

unsigned int tsfth;



#else

#error "please modify tx_desc to your own\n"

#endif


}__attribute__((packed));



union rx_desc {

struct  rx_desc_rtl8187  desc_87;

}__attribute__((packed));

#if 0
void PerformUndecoratedSignalSmoothing8187(struct net_device *dev, struct ieee80211_rx_stats *stats)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    bool bCckRate = rtl8180_IsWirelessBMode(rtl8180_rate2rate(stats->rate));

    if(NIC_8187 == priv->card_8187) {
        if(priv->UndecoratedSmoothedSS >= 0)
        {
            priv->UndecoratedSmoothedSS = ((priv->UndecoratedSmoothedSS * 50) + (stats->signalstrength * 11)) / 60;
        }
        else
        {
            priv->UndecoratedSmoothedSS = stats->signalstrength;
        }
    } else {
        priv->bCurCCKPkt = bCckRate;

        if(priv->UndecoratedSmoothedSS >= 0)
        {
            priv->UndecoratedSmoothedSS = ((priv->UndecoratedSmoothedSS * 5) + (stats->signalstrength * 10)) / 6;
        }
        else
        {
            priv->UndecoratedSmoothedSS = stats->signalstrength * 10;
        }

        if(priv->CurCCKRSSI >= 0 && bCckRate)
        {
            priv->CurCCKRSSI = stats->signal;
        }
        else
        {
            priv->CurCCKRSSI = 0;
        }
    }
}
#endif

void rtl8180_irq_rx_tasklet(struct r8180_priv *priv)
{
    struct urb *rx_urb = priv->rxurb_task;
    struct net_device *dev = (struct net_device*)rx_urb->context;
    int status,len,flen;
    struct sk_buff *skb;


    u32 SignalStrength = 0;
    bool bCckRate = false;
    union   rx_desc *rx_desc;
#ifdef SW_ANTE_DIVERSITY
        u8 Antenna = 0;
#endif
    struct ieee80211_rx_stats stats = {
        .signal = 0,
        .noise = -98,
        .rate = 0,
        .freq = IEEE80211_24GHZ_BAND,
    };

    status = rx_urb->status;
    if(status == 0){


        len = rx_urb->actual_length;
        len -= 4*4;


        rx_desc = rx_urb->transfer_buffer + len;

        flen = rx_desc->desc_87.rxlen ;

        if( flen <= rx_urb->actual_length){

#if 1

            rx_desc = (union rx_desc *)(rx_urb->transfer_buffer + len);

            if(flen <= rx_urb->actual_length){
                stats.signal = rx_desc->desc_87.rssi;
                stats.noise = rx_desc->desc_87.sq;
                stats.rate = rx_desc->desc_87.rxrate;
                stats.mac_time[0] = rx_desc->desc_87.tsftl;
                stats.mac_time[1] = rx_desc->desc_87.tsfth;
                SignalStrength = (rx_desc->desc_87.agc&0xfe) >> 1;
                priv->RecvSignalPower = (priv->RecvSignalPower * 5 + SignalStrength - 1) / 6;

#ifdef SW_ANTE_DIVERSITY
                                Antenna = rx_desc->desc_87.antenna;
#endif
                if( ((stats.rate <= 22) && (stats.rate != 12) && (stats.rate != 18))\
                        || (stats.rate == 44) )
                    bCckRate= true;
                if (!bCckRate)
                {
                    if (SignalStrength > 90) SignalStrength = 90;
                    else if (SignalStrength < 25) SignalStrength = 25;
                    SignalStrength = ((90 - SignalStrength)*100)/65;
                }
                else
                {
                    if (SignalStrength >95) SignalStrength = 95;
                    else if (SignalStrength < 30) SignalStrength = 30;
                    SignalStrength = ((95 - SignalStrength)*100)/65;
                }

                SignalStrength = (SignalStrength*75)/100 + 25;
                stats.signalstrength = (u8)SignalStrength;

#endif

                skb = dev_alloc_skb(flen-4);
                if(skb){

                    struct ieee80211_hdr *hdr;
                    hdr = (struct ieee80211_hdr *)skb->data;
                    if(hdr->addr3[5] == 0xE3)
                    {
                    }
                    memcpy(skb_put(skb,flen-4),
                            rx_urb->transfer_buffer,flen -4);


                    priv->stats.rxok++;
                    if(!ieee80211_rx(priv->ieee80211, skb, &stats))
                        dev_kfree_skb_any(skb);}

#ifdef SW_ANTE_DIVERSITY
                                        priv->LastRxPktAntenna = Antenna ? 1:0;
                    priv->SignalStrength = SignalStrength;
                    SwAntennaDiversityRxOk8185(dev, priv->SignalStrength);
#endif
            }
        }else {
            priv->stats.rxurberr++;
        }
    }else{
        priv->stats.rxstaterr++;
        priv->ieee80211->stats.rx_errors++;

    }

    if(status != -ENOENT)rtl8187_rx_urbsubmit(dev,rx_urb);
    else DMESG("RX process aborted due to explicit shutdown");
}

int rtl8180_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    struct iwreq *wrq = (struct iwreq *)rq;
    int ret=-1;

    down(&priv->wx_sem);


    switch (cmd) {
        case RTL_IOCTL_WPA_SUPPLICANT:
        ret = ieee80211_wpa_supplicant_ioctl(priv->ieee80211, &wrq->u.data);
        break;

        default:
        ret = -EOPNOTSUPP;
        break;
    }

    up(&priv->wx_sem);

    return ret;
}


#if 0
void rtl8187_rx_isr(struct urb *rx_urb, struct pt_regs *regs)
{
#if 0
    struct urb *rx_urb = priv->rxurb_task;
    struct net_device *dev = (struct net_device*)rx_urb->context;
#else
    struct net_device *dev = (struct net_device*)rx_urb->context;
    struct r8180_priv *priv = ieee80211_priv(dev);
#endif
    int status,len,flen;
    struct sk_buff *skb;
    u8 *desc;
    u8 signal,quality,rate;


    struct ieee80211_rx_stats stats = {
        .signal = 0,
        .noise = -98,
        .rate = 0,
        .freq = IEEE80211_24GHZ_BAND,
    };


    status = rx_urb->status;
    if(status == 0){

        len = rx_urb->actual_length;
        len -= 4*4;
#if 0
        desc = (u32*)(rx_urb->transfer_buffer + len);

        flen = desc[0] & 0xfff;

        if( flen <= rx_urb->actual_length){

            signal=(desc[1]& (0xff0000))>>16;
            signal=(signal&0xfe)>>1;

            quality=(desc[1] & (0xff));

            rate=(desc[0] &((1<<23)|(1<<22)|(1<<21)|(1<<20)))>>20;
            stats.rate = rtl8180_rate2rate(rate);
            stats.mac_time[0] = desc[2];
            stats.mac_time[1] = desc[3];
#endif

        desc = rx_urb->transfer_buffer + len;

        flen = ((desc[1] & 0x0f) << 8) + (desc[0] & 0xff);

        if( flen <= rx_urb->actual_length){

            signal=(desc[6]& 0xfe)>>1;

            quality=desc[4] & 0xff;

            rate=(desc[2] & 0xf0)>>4;
            stats.rate = rtl8180_rate2rate(rate);
            stats.mac_time[0] = desc[8] + (desc[9]<<8) + (desc[10]<<16) + (desc[11]<<24);
            stats.mac_time[1] = desc[12] + (desc[13]<<8) + (desc[14]<<16) + (desc[15]<<24);


            if(!rtl8180_IsWirelessBMode(stats.rate) )
            {
                if(signal>90)
                    signal=90;
                else if(signal<25)
                    signal=25;
                signal = (90-signal)*100/65;
            }
            else
            {
                if(signal>95)
                    signal = 95;
                else if(signal<30)
                    signal = 30;
                signal =(95-signal )*100/65;
            }
            priv->wstats.qual.level = signal;
            if(quality > 64)
                priv ->wstats.qual.qual = 0;
            else
                priv ->wstats.qual.qual = ((64-quality) * 100) / 64;
            priv->wstats.qual.noise = 100 - priv ->wstats.qual.qual;
            priv->wstats.qual.updated = 7;
            skb = dev_alloc_skb(flen-4);
            if(skb){
            memcpy(skb_put(skb,flen-4),
                rx_urb->transfer_buffer,flen -4);

            #ifdef DUMP_RX
            int i;
            for(i=0;i<flen-4;i++)
                printk("%2x ",((u8*)(rx_urb->transfer_buffer))[i]);
            printk("------RATE %x:w---------------\n",stats.rate);

            #endif
            priv->stats.rxok++;

            if(!ieee80211_rx(priv->ieee80211,
                skb, &stats))
            dev_kfree_skb_any(skb);
            }
        }else priv->stats.rxurberr++;
    }else{
        priv->stats.rxstaterr++;
        priv->ieee80211->stats.rx_errors++;
    }

    if(status != -ENOENT)rtl8187_rx_urbsubmit(dev,rx_urb);
    else DMESG("RX process aborted due to explicit shutdown");
}
#endif


/****************************************************************************
     ---------------------------- USB_STUFF---------------------------
*****************************************************************************/

#ifdef HAVE_NET_DEVICE_OPS
static const struct net_device_ops rtl8180_netdev_ops = {
    .ndo_open = rtl8180_open,
    .ndo_stop = rtl8180_close,
    .ndo_get_stats = rtl8180_stats,
    .ndo_tx_timeout = tx_timeout,
    .ndo_do_ioctl = rtl8180_ioctl,
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,39))
    .ndo_set_multicast_list = r8180_set_multicast,
#endif
    .ndo_set_mac_address = r8180_set_mac_adr,
    .ndo_start_xmit = ieee80211_xmit,
};
#endif


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
static int __devinit rtl8187_usb_probe(struct usb_interface *intf,
             const struct usb_device_id *id)
#else
static void * __devinit rtl8187_usb_probe(struct usb_device *udev,
                            unsigned int ifnum,
                      const struct usb_device_id *id)
#endif
{
    struct net_device *dev = NULL;
    struct r8180_priv *priv= NULL;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    struct usb_device *udev = interface_to_usbdev(intf);
#endif

    dev = alloc_ieee80211(sizeof(struct r8180_priv));

#if(LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
    SET_MODULE_OWNER(dev);
#endif

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    usb_set_intfdata(intf, dev);
    SET_NETDEV_DEV(dev, &intf->dev);
#endif

    priv = ieee80211_priv(dev);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    priv->ieee80211 = netdev_priv(dev);
#else
    priv->ieee80211 = (struct ieee80211_device *)dev->priv;
#endif
    priv->dev = dev;
    priv->udev=udev;

#ifdef HAVE_NET_DEVICE_OPS
    dev->netdev_ops = &rtl8180_netdev_ops;
#else
    dev->open = rtl8180_open;
    dev->stop = rtl8180_close;
    dev->get_stats = rtl8180_stats;
    dev->hard_start_xmit = ieee80211_xmit;
    dev->tx_timeout = tx_timeout;
    dev->wireless_handlers = &r8180_wx_handlers_def;
    dev->do_ioctl = rtl8180_ioctl;
    dev->set_multicast_list = r8180_set_multicast;
    dev->set_mac_address = r8180_set_mac_adr;
#endif

#if WIRELESS_EXT >= 12
#if WIRELESS_EXT < 17
    dev->get_wireless_stats = r8180_get_wireless_stats;
#endif
    dev->wireless_handlers = (struct iw_handler_def *) &r8180_wx_handlers_def;
#endif
    dev->type=ARPHRD_ETHER;

    if (dev_alloc_name(dev, ifname) < 0){
                DMESG("Oops: devname already taken! Trying wlan%%d...\n");
        ifname = "wlan%d";
        dev_alloc_name(dev, ifname);
        }


    if(rtl8180_init(dev)!=0){
        DMESG("Initialization failed");
        goto fail;
    }

    netif_carrier_off(dev);
    netif_stop_queue(dev);

    register_netdev(dev);

    rtl8180_proc_init_one(dev);

#ifdef POLLING_METHOD_FOR_RADIO
    gpio_change_polling((unsigned long) dev);
#endif

    DMESG("Driver probe completed\n");
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
    return dev;
#else
    return 0;
#endif

fail:
    free_ieee80211(dev);

    DMESG("wlan driver load failed\n");

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
    return NULL;
#else
    return -ENODEV;
#endif

}

#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
static void __devexit rtl8187_usb_disconnect(struct usb_interface *intf)
#else
static void __devexit rtl8187_usb_disconnect(struct usb_device *udev, void *ptr)
#endif
{
    struct r8180_priv *priv;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
    struct net_device *dev = usb_get_intfdata(intf);
#else
    struct net_device *dev = (struct net_device *)ptr;
#endif
    if(dev){

        unregister_netdev(dev);

        priv=ieee80211_priv(dev);

#ifdef POLLING_METHOD_FOR_RADIO
            del_timer_sync(&priv->gpio_polling_timer);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        cancel_delayed_work(&priv->ieee80211->GPIOChangeRFWorkItem);
#endif
#endif

        rtl8180_proc_remove_one(dev);

        rtl8180_down(dev);
        priv->rf_close(dev);
        rtl8187_usb_deleteendpoints(dev);
#ifdef LED
        DeInitSwLeds(dev);
#endif
        rtl8180_irq_disable(dev);
        rtl8180_reset(dev);
        mdelay(10);

    }


    free_ieee80211(dev);
    DMESG("wlan driver removed\n");
}

#ifdef BUILT_IN_IEEE80211
/* fun with the built-in ieee80211 stack... */
extern int ieee80211_init(void);
extern void ieee80211_exit(void);
extern int ieee80211_crypto_init(void);
extern void ieee80211_crypto_deinit(void);
extern int ieee80211_crypto_tkip_init(void);
extern void ieee80211_crypto_tkip_exit(void);
extern int ieee80211_crypto_ccmp_init(void);
extern void ieee80211_crypto_ccmp_exit(void);
extern int ieee80211_crypto_wep_init(void);
extern void ieee80211_crypto_wep_exit(void);
#endif

static int __init rtl8187_usb_module_init(void)
{
#ifdef BUILT_IN_IEEE80211
    int ret;

    ret = ieee80211_init();
    if (ret) {
        printk(KERN_ERR "ieee80211_init() failed %d\n", ret);
        return ret;
    }
    ret = ieee80211_crypto_init();
    if (ret) {
        printk(KERN_ERR "ieee80211_crypto_init() failed %d\n", ret);
        return ret;
    }
    ret = ieee80211_crypto_tkip_init();
    if (ret) {
        printk(KERN_ERR "ieee80211_crypto_tkip_init() failed %d\n", ret);
        return ret;
    }
    ret = ieee80211_crypto_ccmp_init();
    if (ret) {
        printk(KERN_ERR "ieee80211_crypto_ccmp_init() failed %d\n", ret);
        return ret;
    }
    ret = ieee80211_crypto_wep_init();
    if (ret) {
        printk(KERN_ERR "ieee80211_crypto_wep_init() failed %d\n", ret);
        return ret;
    }
#endif

    printk(KERN_INFO "\nLinux kernel driver for RTL8187L based WLAN cards\n");
    printk(KERN_INFO "Copyright (c) 2004-2005, Realtek\n");
    DMESG("Initializing module");
    DMESG("Wireless extensions version %d", WIRELESS_EXT);
    rtl8180_proc_module_init();
    return usb_register(&rtl8187_usb_driver);
}


static void __exit rtl8187_usb_module_exit(void)
{
    usb_deregister(&rtl8187_usb_driver);

    rtl8180_proc_module_remove();
#ifdef BUILT_IN_IEEE80211
        ieee80211_crypto_tkip_exit();
        ieee80211_crypto_ccmp_exit();
        ieee80211_crypto_wep_exit();
        ieee80211_crypto_deinit();
        ieee80211_exit();
#endif

    DMESG("Exiting");
}


void rtl8180_try_wake_queue(struct net_device *dev, int pri)
{
    unsigned long flags;
    short enough_desc;
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);

    spin_lock_irqsave(&priv->tx_lock,flags);
    enough_desc = check_nic_enought_desc(dev,pri);
        spin_unlock_irqrestore(&priv->tx_lock,flags);

    if(enough_desc)
        ieee80211_wake_queue(priv->ieee80211);
}


bool SetZebraRFPowerState8187(
    struct net_device *dev,
    RT_RF_POWER_STATE   eRFPowerState
    )
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    u8          btCR9346, btConfig3;
    bool            bResult = true;
    int             i;



    btCR9346 = read_nic_byte(dev, CR9346);
    write_nic_byte(dev, CR9346, (btCR9346|0xC0) );
    btConfig3 = read_nic_byte(dev, CONFIG3);
    write_nic_byte(dev, CONFIG3, (btConfig3|CONFIG3_PARM_En) );

    switch(eRFPowerState)
    {
    case eRfOn:
        DMESG("Now Radio On");
        write_nic_dword(dev,  0xff94, 0x3dc00002);
        write_nic_dword(dev,  ANAPARAM, ANAPARM_ON);
        write_nic_dword(dev,   ANAPARAM2, ANAPARM2_ON);
        write_nic_byte(dev, CONFIG4, (priv->RFProgType));

        write_rtl8225(dev, 0x4, 0x9FF);
        break;

    case eRfSleep:
        for(i = 0; i < 500; i++)
        {
            if(read_nic_byte(dev, TFPC) == 0)
                break;
            else
                udelay(10);
        }
        write_rtl8225(dev, 0x4, 0xDFF);
        write_nic_byte(dev, CONFIG4, (priv->RFProgType|Config4_PowerOff));
        write_nic_dword(dev,  0xff94, 0x3d000002);

        write_nic_dword(dev, ANAPARAM, 0xa00faa59);
        write_nic_dword(dev, ANAPARAM2, ANAPARM2_OFF);
        break;

    case eRfOff:
        DMESG("Now Radio Off");
        for(i = 0; i < 500; i++)
        {
            if(read_nic_byte(dev, TFPC) == 0)
                break;
            else
                udelay(10);
        }
        write_rtl8225(dev, 0x4, 0x1FF);
        write_nic_byte(dev, CONFIG4, (priv->RFProgType|Config4_PowerOff));
        write_nic_dword(dev,  0xff94, 0x3d000002);

        write_nic_dword(dev, ANAPARAM,  0xa00faa59);
        write_nic_dword(dev, ANAPARAM2, ANAPARM2_OFF);
        break;

    default:
        bResult = false;
        printk("SetZebraRFPowerState8187(): unknow state to set: 0x%X!!!\n", eRFPowerState);
        break;
    }

    btConfig3 &= ~(CONFIG3_PARM_En);
    write_nic_byte(dev, CONFIG3, btConfig3);
    btCR9346 &= ~(0xC0);
    write_nic_byte(dev, CR9346, btCR9346);

    if(bResult)
    {
        priv->eRFPowerState = eRFPowerState;
#ifdef LED
        switch(priv->eRFPowerState)
        {
        case eRfOff:

            if(priv->RfOffReason==BIT28)
            {
            }
            else
            {
                priv->ieee80211->ieee80211_led_contorl(dev,LED_CTL_POWER_OFF);
            }
            break;

        case eRfOn:
            if(priv->up == 1)
                priv->ieee80211->ieee80211_led_contorl(dev,LED_CTL_POWER_ON);
            break;

        default:
            break;
        }
#endif
    }

    return bResult;
}


bool
SetRFPowerState(
    struct net_device *dev,
    RT_RF_POWER_STATE   eRFPowerState
    )
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    bool            bResult = false;

    if(eRFPowerState == priv->eRFPowerState)
    {
        DMESG("SetRFPowerState(): discard the request for eRFPowerState(%d) is the same.", eRFPowerState);
        return bResult;
    }

    switch(priv->rf_chip)
    {
        case RF_ZEBRA:
        case RF_ZEBRA2:
            bResult = SetZebraRFPowerState8187(dev, eRFPowerState);
            break;

        default:
            printk("SetRFPowerState8185(): unknown RFChipID: 0x%X!!!\n", priv->rf_chip);
            break;;
    }

    return bResult;
}

bool MgntActSet_RF_State(
    struct net_device *dev,
    RT_RF_POWER_STATE StateToSet,
    u32 ChangeSource)
{
    struct r8180_priv *priv = (struct r8180_priv *)ieee80211_priv(dev);
    bool                bActionAllowed = false;
    bool                bConnectBySSID = false;
    RT_RF_POWER_STATE   rtState;
    u16             RFWaitCounter = 0;
    unsigned long flag;


    while(true)
    {
        spin_lock_irqsave(&priv->rf_ps_lock,flag);
        if(priv->RFChangeInProgress)
        {
            printk("MgntActSet_RF_State(): RF Change in progress! Wait to set..StateToSet(%d).\n", StateToSet);
            spin_unlock_irqrestore(&priv->rf_ps_lock,flag);
            while(priv->RFChangeInProgress)
            {
                RFWaitCounter ++;
                printk("MgntActSet_RF_State(): Wait 1 ms (%d times)...\n", RFWaitCounter);
                udelay(1000);

                if(RFWaitCounter > 1000)
                {
                    printk("MgntActSet_RF_State(): Wait too long to set RF\n");
                    return false;
                }
            }
        }
        else
        {
            priv->RFChangeInProgress = true;
            spin_unlock_irqrestore(&priv->rf_ps_lock,flag);
            break;
        }
    }

    rtState = priv->eRFPowerState;

    switch(StateToSet)
    {
    case eRfOn:
        priv->RfOffReason &= (~ChangeSource);

        if(! priv->RfOffReason)
        {
            priv->RfOffReason = 0;
            bActionAllowed = true;

            if(rtState == eRfOff && ChangeSource >=RF_CHANGE_BY_HW && !priv->bInHctTest)
            {
                bConnectBySSID = true;
            }
        }
        else
            printk("MgntActSet_RF_State - eRfon reject pMgntInfo->RfOffReason= 0x%x, ChangeSource=0x%X\n", priv->RfOffReason, ChangeSource);
        break;

    case eRfOff:
            if (priv->RfOffReason > RF_CHANGE_BY_IPS)
            {

            }

        ieee80211_disassociate(priv->ieee80211);

        priv->RfOffReason |= ChangeSource;
        bActionAllowed = true;
        break;

    case eRfSleep:
        priv->RfOffReason |= ChangeSource;
        bActionAllowed = true;
        break;

    default:
        break;
    }

    if(bActionAllowed)
    {
        SetRFPowerState(dev, StateToSet);
        if(StateToSet == eRfOn)
        {
            if(bConnectBySSID)
            {
            }
        }
        else if(StateToSet == eRfOff)
        {
        }
    }
    else
    {
        printk("MgntActSet_RF_State(): Action is rejected.... StateToSet(%d), ChangeSource(%#X), RfOffReason(%#X)\n", StateToSet, ChangeSource, priv->RfOffReason);
    }

    spin_lock_irqsave(&priv->rf_ps_lock,flag);
    priv->RFChangeInProgress = false;
    spin_unlock_irqrestore(&priv->rf_ps_lock,flag);
    return bActionAllowed;
}


#ifdef POLLING_METHOD_FOR_RADIO
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
void GPIOChangeRFWorkItemCallBack(struct work_struct *work)
{
    struct delayed_work *dwork = container_of(work, struct delayed_work, work);
        struct ieee80211_device *ieee = container_of(dwork,struct ieee80211_device,GPIOChangeRFWorkItem);
        struct net_device *dev = ieee->dev;
    struct r8180_priv *priv = ieee80211_priv(dev);
#else
void GPIOChangeRFWorkItemCallBack(struct net_device *dev)
{
    struct r8180_priv *priv = ieee80211_priv(dev);
    struct ieee80211_device *ieee = priv->ieee80211;
#endif

    u8 tmp1byte;
    RT_RF_POWER_STATE   eRfPowerStateToSet;
    bool    bActuallySet=false;

    char *argv[3];
        static char *RadioPowerPath = "/etc/acpi/events/RadioPower.sh";
        static char *envp[] = {"HOME=/", "TERM=linux", "PATH=/usr/bin:/bin", NULL};

#if 0
    if(priv->up == 0)
        {
            printk("\nDo nothing...");
            goto out;
        }
    else
#endif
        {


            tmp1byte = read_nic_byte(dev,GPE);
            tmp1byte &= ~BIT1;
            write_nic_byte(dev,GPE,tmp1byte);

            tmp1byte = read_nic_byte(dev,GPI);

            eRfPowerStateToSet = (tmp1byte&BIT1) ?  eRfOn : eRfOff;


            if((priv->ieee80211->bHwRadioOff == true) && (eRfPowerStateToSet == eRfOn)){
                priv->ieee80211->bHwRadioOff = false;
                bActuallySet = true;
            }
            else if((priv->ieee80211->bHwRadioOff == false) && (eRfPowerStateToSet == eRfOff)){
                priv->ieee80211->bHwRadioOff = true;
                bActuallySet = true;
            }

            if(priv->wlan_first_up_flag1 == 0){
                bActuallySet = true;
                priv->wlan_first_up_flag1 = 1;
            }

            if(bActuallySet)
            {
                MgntActSet_RF_State(dev, eRfPowerStateToSet, RF_CHANGE_BY_HW);

                                if(priv->ieee80211->bHwRadioOff == true)
                                        argv[1] = "RFOFF";
                                else{
                                                argv[1] = "RFON";
                                }
                                argv[0] = RadioPowerPath;
                                argv[2] = NULL;

                                call_usermodehelper(RadioPowerPath,argv,envp,1);
            }

        }
}

void gpio_change_polling(unsigned long data)
{
    struct r8180_priv* priv = ieee80211_priv((struct net_device *)data);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
        queue_delayed_work(priv->ieee80211->wq,&priv->ieee80211->GPIOChangeRFWorkItem,0);
#elif LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        queue_work(priv->ieee80211->wq,&priv->ieee80211->GPIOChangeRFWorkItem);
#else
        schedule_task(&priv->ieee80211->GPIOChangeRFWorkItem);
#endif

    mod_timer(&priv->gpio_polling_timer, jiffies + MSECS(IEEE80211_WATCH_DOG_TIME));
}
#endif


/***************************************************************************
     ------------------- module init / exit stubs ----------------
****************************************************************************/
module_init(rtl8187_usb_module_init);
module_exit(rtl8187_usb_module_exit);
