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



#include "r8187.h"
#include "r8180_hw.h"
#include "../ieee80211/ieee80211.h"


u32 rtl8180_rates[] = {1000000,2000000,5500000,11000000,
	6000000,9000000,12000000,18000000,24000000,36000000,48000000,54000000};
#define RATE_COUNT (sizeof(rtl8180_rates)/sizeof(rtl8180_rates[0]))

static CHANNEL_LIST DefaultChannelPlan[] = {
	{{1,2,3,4,5,6,7,8,9,10,11,12,13,14},14},			
	{{1,2,3,4,5,6,7,8,9,10,11,36,40,44,48,52,56,60,64},19},  		
	{{1,2,3,4,5,6,7,8,9,10,11},11},                    				
	{{1,2,3,4,5,6,7,8,9,10,11,12,13,36,40,44,48,52,56,60,64},21},  	
	{{1,2,3,4,5,6,7,8,9,10,11,12,13,36,40,44,48,52,56,60,64},21},    
	{{1,2,3,4,5,6,7,8,9,10,11,12,13,36,40,44,48,52,56,60,64},21},  	
	{{14,36,40,44,48,52,56,60,64},9},						
	{{1,2,3,4,5,6,7,8,9,10,11,12,13,14, 36,40,44,48,52,56,60,64},22},
	{{1,2,3,4,5,6,7,8,9,10,11,12,13,36,40,44,48,52,56,60,64},21},	
	{{1,2,3,4,5,6,7,8,9,10,11,12,13,34,38,42,46},17}			
};	
#if 0
static int r8180_wx_get_freq(struct net_device *dev,
			     struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	return ieee80211_wx_get_freq(priv->ieee80211,a,wrqu,b);
}
#endif

static int r8180_wx_get_freq(struct net_device *dev,
			     struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	return ieee80211_wx_get_freq(priv->ieee80211,a,wrqu,b);
}
#if 0

static int r8180_wx_set_beaconinterval(struct net_device *dev, struct iw_request_info *aa,
			  union iwreq_data *wrqu, char *b)
{
	int *parms = (int *)b;
	int bi = parms[0];
	
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
	
	down(&priv->wx_sem);
	DMESG("setting beacon interval to %x",bi);
	
	priv->ieee80211->beacon_interval=bi;
	rtl8180_commit(dev);
	up(&priv->wx_sem);
		
	return 0;	
}


static int r8180_wx_set_forceassociate(struct net_device *dev, struct iw_request_info *aa,
			  union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv=ieee80211_priv(dev);	
	int *parms = (int *)extra;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	priv->ieee80211->force_associate = (parms[0] > 0);
	

	return 0;
}

#endif
static int r8180_wx_get_mode(struct net_device *dev, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	struct r8180_priv *priv=ieee80211_priv(dev);	

	return ieee80211_wx_get_mode(priv->ieee80211,a,wrqu,b);
}



static int r8180_wx_get_rate(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	return ieee80211_wx_get_rate(priv->ieee80211,info,wrqu,extra);
}



static int r8180_wx_set_rate(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	int ret;
	struct r8180_priv *priv = ieee80211_priv(dev);	
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);

	ret = ieee80211_wx_set_rate(priv->ieee80211,info,wrqu,extra);
	
	up(&priv->wx_sem);
	
	return ret;
}

static int r8180_wx_set_rawtx(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int ret;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);
	
	ret = ieee80211_wx_set_rawtx(priv->ieee80211, info, wrqu, extra);
	
	up(&priv->wx_sem);
	
	return ret;
	 
}

static int r8180_wx_set_rts(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
		
	if(priv->ieee80211->bHwRadioOff)
		return 0;
	
	if (wrqu->rts.disabled)
		priv->rts = DEFAULT_RTS_THRESHOLD;
	else {
		if (wrqu->rts.value < MIN_RTS_THRESHOLD ||
		    wrqu->rts.value > MAX_RTS_THRESHOLD)
			return -EINVAL;
		
		priv->rts = wrqu->rts.value;
	}

	return 0;
}

static int r8180_wx_get_rts(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	wrqu->rts.value = priv->rts;
	wrqu->rts.fixed = 0;	
	wrqu->rts.disabled = (wrqu->rts.value == 0);

	return 0;
}

static int r8180_wx_set_crcmon(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int *parms = (int *)extra;
	int enable = (parms[0] > 0);
	short prev = priv->crcmon;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
	
	down(&priv->wx_sem);
	
	if(enable) 
		priv->crcmon=1;
	else 
		priv->crcmon=0;

	DMESG("bad CRC in monitor mode are %s", 
	      priv->crcmon ? "accepted" : "rejected");

	if(prev != priv->crcmon && priv->up){
		rtl8180_down(dev);
		rtl8180_up(dev);
	}
	
	up(&priv->wx_sem);
	
	return 0;
}

static int r8180_wx_set_mode(struct net_device *dev, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int ret;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);
#if 0
	if(priv->bInactivePs){
		if(wrqu->mode == IW_MODE_ADHOC){
			down(&priv->ieee80211->ips_sem);
			IPSLeave(dev);	
			up(&priv->ieee80211->ips_sem);	
		}
	}
#endif	
	ret = ieee80211_wx_set_mode(priv->ieee80211,a,wrqu,b);
	
	rtl8187_set_rxconf(dev);
	
	up(&priv->wx_sem);
	return ret;
}

struct  iw_range_with_scan_capa
{
        __u32           throughput;     

        __u32           min_nwid;       
        __u32           max_nwid;       

        __u16           old_num_channels;
        __u8            old_num_frequency;

        __u8            scan_capa;       
};

static int rtl8180_wx_get_range(struct net_device *dev, 
				struct iw_request_info *info, 
				union iwreq_data *wrqu, char *extra)
{
	struct iw_range *range = (struct iw_range *)extra;
	struct r8180_priv *priv = ieee80211_priv(dev);
	u16 val;
	int i;

	wrqu->data.length = sizeof(*range);
	memset(range, 0, sizeof(*range));

	

	range->throughput = 5 * 1000 * 1000;     

	
	if(priv->rf_set_sens != NULL)
		range->sensitivity = priv->max_sens;	
	
	range->max_qual.qual = 100;
	range->max_qual.level = 0;
	range->max_qual.noise = -98;
	range->max_qual.updated = 7; 

	range->avg_qual.qual = 92; 
	range->avg_qual.level = 20 + -98;
	range->avg_qual.noise = 0;
	range->avg_qual.updated = 7; 

	range->num_bitrates = RATE_COUNT;
	
	for (i = 0; i < RATE_COUNT && i < IW_MAX_BITRATES; i++) {
		range->bitrate[i] = rtl8180_rates[i];
	}
	
	range->min_frag = MIN_FRAG_THRESHOLD;
	range->max_frag = MAX_FRAG_THRESHOLD;
	
	range->pm_capa = 0;

	range->we_version_compiled = WIRELESS_EXT;
	range->we_version_source = 16;


        range->num_channels = 14;

	for (i = 0, val = 0; i < 14; i++) {
		
		if ((priv->challow)[i+1]) {
		        range->freq[val].i = i + 1;
			range->freq[val].m = ieee80211_wlan_frequencies[i] * 100000;
			range->freq[val].e = 1;
			val++;
		} else {
		}
		
		if (val == IW_MAX_FREQUENCIES)
		break;
	}

	range->num_frequency = val;
	
#if WIRELESS_EXT > 17
	range->enc_capa = IW_ENC_CAPA_WPA | IW_ENC_CAPA_WPA2 
				| IW_ENC_CAPA_CIPHER_TKIP | IW_ENC_CAPA_CIPHER_CCMP;
#endif

	return 0;
}


static int r8180_wx_set_scan(struct net_device *dev, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	struct ieee80211_device *ieee = priv->ieee80211; 
	int ret;
	
	if(!priv->up) return -1;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;

#ifdef CONFIG_BIG_DATA_NOSCAN
	if ((priv->ieee80211->state == IEEE80211_LINKED) && (priv->ScanAllowedNow == 0))
		return 0;
#endif

#if WIRELESS_EXT > 17
	if (wrqu->data.flags & IW_SCAN_THIS_ESSID)
	{
		struct iw_scan_req* req = (struct iw_scan_req*)b;
		if (req->essid_len)
		{
			ieee->current_network.ssid_len = req->essid_len;
			memcpy(ieee->current_network.ssid, req->essid, req->essid_len); 
		}
	}
#endif

	down(&priv->wx_sem);

	if(priv->ieee80211->state != IEEE80211_LINKED){
		priv->ieee80211->scanning = 0;
		priv->ieee80211->sync_scan_hurryup = 0;
		ieee80211_start_scan_syncro(priv->ieee80211);
		ret = 0;
	}else	
		ret = ieee80211_wx_set_scan(priv->ieee80211,a,wrqu,b);
	
	up(&priv->wx_sem);
	return ret;
}


static int r8180_wx_get_scan(struct net_device *dev, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{

	int ret;
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(!priv->up) return -1;
			
	down(&priv->wx_sem);

	ret = ieee80211_wx_get_scan(priv->ieee80211,a,wrqu,b);
		
	up(&priv->wx_sem);
	
	return ret;
}


static int r8180_wx_set_essid(struct net_device *dev, 
			      struct iw_request_info *a,
			      union iwreq_data *wrqu, char *b)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	int ret;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);
	
	ret = ieee80211_wx_set_essid(priv->ieee80211,a,wrqu,b);
	
	up(&priv->wx_sem);
	return ret;
}


static int r8180_wx_get_essid(struct net_device *dev, 
			      struct iw_request_info *a,
			      union iwreq_data *wrqu, char *b)
{
	int ret;
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	down(&priv->wx_sem);
	
	ret = ieee80211_wx_get_essid(priv->ieee80211, a, wrqu, b);

	up(&priv->wx_sem);
	
	return ret;
}

#if 0
static int r8180_wx_set_freq(struct net_device *dev, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	int ret;
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);
	
	ret = ieee80211_wx_set_freq(priv->ieee80211, a, wrqu, b);
	
	up(&priv->wx_sem);
	return ret;
}
#endif
static int r8180_wx_set_freq(struct net_device *dev, struct iw_request_info *a,
			     union iwreq_data *wrqu, char *b)
{
	int ret;
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;

	down(&priv->wx_sem);
	
	ret = ieee80211_wx_set_freq(priv->ieee80211, a, wrqu, b);
	
	up(&priv->wx_sem);
	return ret;
}
static int r8180_wx_get_name(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	return ieee80211_wx_get_name(priv->ieee80211, info, wrqu, extra);
}


static int r8180_wx_set_frag(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
	
	if (wrqu->frag.disabled)
		priv->ieee80211->fts = DEFAULT_FRAG_THRESHOLD;
	else {
		if (wrqu->frag.value < MIN_FRAG_THRESHOLD ||
		    wrqu->frag.value > MAX_FRAG_THRESHOLD)
			return -EINVAL;
		
		priv->ieee80211->fts = wrqu->frag.value & ~0x1;
	}

	return 0;
}


static int r8180_wx_get_frag(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);

	wrqu->frag.value = priv->ieee80211->fts;
	wrqu->frag.fixed = 0;	
	wrqu->frag.disabled = (wrqu->frag.value == DEFAULT_FRAG_THRESHOLD);

	return 0;
}


static int r8180_wx_set_wap(struct net_device *dev,
			 struct iw_request_info *info,
			 union iwreq_data *awrq,
			 char *extra)
{
	int ret;
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);
	
	ret = ieee80211_wx_set_wap(priv->ieee80211,info,awrq,extra);
	
	up(&priv->wx_sem);
	return ret;
	
}
	

static int r8180_wx_get_wap(struct net_device *dev, 
			    struct iw_request_info *info, 
			    union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	return ieee80211_wx_get_wap(priv->ieee80211,info,wrqu,extra);
}


static int r8180_wx_get_enc(struct net_device *dev, 
			    struct iw_request_info *info, 
			    union iwreq_data *wrqu, char *key)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	return ieee80211_wx_get_encode(priv->ieee80211, info, wrqu, key);
}

static int r8180_wx_set_enc(struct net_device *dev, 
			    struct iw_request_info *info, 
			    union iwreq_data *wrqu, char *key)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int ret;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);
	
		DMESG("Setting SW wep key");
		ret = ieee80211_wx_set_encode(priv->ieee80211,info,wrqu,key);
			
	up(&priv->wx_sem);
	return ret;
}


static int r8180_wx_set_scan_type(struct net_device *dev, struct iw_request_info *aa, union
 iwreq_data *wrqu, char *p){
  
 	struct r8180_priv *priv = ieee80211_priv(dev);
	int *parms=(int*)p;
	int mode=parms[0];
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	priv->ieee80211->active_scan = mode;
	
	return 1;
}



static int r8180_wx_set_retry(struct net_device *dev, 
				struct iw_request_info *info, 
				union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int err = 0;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);
	
	if (wrqu->retry.flags & IW_RETRY_LIFETIME || 
	    wrqu->retry.disabled){
		err = -EINVAL;
		goto exit;
	}
	if (!(wrqu->retry.flags & IW_RETRY_LIMIT)){
		err = -EINVAL;
		goto exit;
	}

	if(wrqu->retry.value > R8180_MAX_RETRY){
		err= -EINVAL;
		goto exit;
	}
	if (wrqu->retry.flags & IW_RETRY_MAX) {
		priv->retry_rts = wrqu->retry.value;
		DMESG("Setting retry for RTS/CTS data to %d", wrqu->retry.value);
	
	}else {
		priv->retry_data = wrqu->retry.value;
		DMESG("Setting retry for non RTS/CTS data to %d", wrqu->retry.value);
	}
	

 	rtl8180_commit(dev);
exit:
	up(&priv->wx_sem);
	
	return err;
}

static int r8180_wx_get_retry(struct net_device *dev, 
				struct iw_request_info *info, 
				union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	

	wrqu->retry.disabled = 0; 

	if ((wrqu->retry.flags & IW_RETRY_TYPE) == 
	    IW_RETRY_LIFETIME) 
		return -EINVAL;
	
	if (wrqu->retry.flags & IW_RETRY_MAX) {
		wrqu->retry.flags = IW_RETRY_LIMIT & IW_RETRY_MAX;
		wrqu->retry.value = priv->retry_rts;
	} else {
		wrqu->retry.flags = IW_RETRY_LIMIT & IW_RETRY_MIN;
		wrqu->retry.value = priv->retry_data;
	}
	

	return 0;
}

static int r8180_wx_get_sens(struct net_device *dev, 
				struct iw_request_info *info, 
				union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	if(priv->rf_set_sens == NULL) 
		return -1; 
	wrqu->sens.value = priv->sens;
	return 0;
}


static int r8180_wx_set_sens(struct net_device *dev, 
				struct iw_request_info *info, 
				union iwreq_data *wrqu, char *extra)
{
	
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	short err = 0;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
	
	down(&priv->wx_sem);
	if(priv->rf_set_sens == NULL) {
		err= -1; 
		goto exit;
	}
	if(priv->rf_set_sens(dev, wrqu->sens.value) == 0)
		priv->sens = wrqu->sens.value;
	else
		err= -EINVAL;

exit:
	up(&priv->wx_sem);
	
	return err;
}


static int dummy(struct net_device *dev, struct iw_request_info *a,
		 union iwreq_data *wrqu,char *b)
{
	return -1;
}
static int r8180_wx_set_enc_ext(struct net_device *dev,
                                        struct iw_request_info *info,
                                        union iwreq_data *wrqu, char *extra)
{
	int ret=0;
	
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;

	down(&priv->wx_sem);

	ret = ieee80211_wx_set_encode_ext(priv->ieee80211, info, wrqu, extra);
	up(&priv->wx_sem);
#endif
	return ret;	

}
static int r8180_wx_set_auth(struct net_device *dev,
                                        struct iw_request_info *info,
                                        union iwreq_data *data, char *extra)
{

	int ret=0;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	struct r8180_priv *priv = ieee80211_priv(dev);

	if(priv->ieee80211->bHwRadioOff)
		return 0;

	down(&priv->wx_sem);


	ret = ieee80211_wx_set_auth(priv->ieee80211, info, &(data->param), extra);
	up(&priv->wx_sem);
#endif
	return ret;
}

static int r8180_wx_set_mlme(struct net_device *dev,
                                        struct iw_request_info *info,
                                        union iwreq_data *wrqu, char *extra)
{
	int ret=0;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)	
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;

	down(&priv->wx_sem);


	ret = ieee80211_wx_set_mlme(priv->ieee80211, info, wrqu, extra);

	up(&priv->wx_sem);
#endif
	return ret;
}
static int r8180_wx_set_gen_ie(struct net_device *dev,
                                        struct iw_request_info *info,
                                        union iwreq_data *data, char *extra)
{
	int ret=0;
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
        struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;

        down(&priv->wx_sem);



        ret = ieee80211_wx_set_gen_ie(priv->ieee80211, extra, data->data.length);

        up(&priv->wx_sem);
#endif
        return ret;


}

static int r8180_wx_get_power(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	int ret;
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	down(&priv->wx_sem);
	
	ret = ieee80211_wx_get_power(priv->ieee80211, info, wrqu, extra);
	
	up(&priv->wx_sem);
	
	return ret;
}

static int r8180_wx_set_power(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	int ret;
	struct r8180_priv *priv = ieee80211_priv(dev);
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
		
	down(&priv->wx_sem);
	if (wrqu->power.disabled==0) {
		wrqu->power.flags|=IW_POWER_ALL_R;
		wrqu->power.flags|=IW_POWER_TIMEOUT;
		wrqu->power.value =1000; 
	}
	
	ret = ieee80211_wx_set_power(priv->ieee80211, info, wrqu, extra);

	up(&priv->wx_sem);
	
	return ret;
}




static int r8180_wx_get_iwmode(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	struct ieee80211_device *ieee;
	int ret = 0;
	down(&priv->wx_sem);

	ieee = priv->ieee80211;

	strcpy(extra, "802.11");
	if(ieee->modulation & IEEE80211_CCK_MODULATION) {
		strcat(extra, "b");
		if(ieee->modulation & IEEE80211_OFDM_MODULATION)
			strcat(extra, "/g");
	} 
	else if(ieee->modulation & IEEE80211_OFDM_MODULATION)
		strcat(extra, "g");

	up(&priv->wx_sem);

	return ret;
}

static int r8180_wx_set_iwmode(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	struct ieee80211_device *ieee = priv->ieee80211;
	int *param = (int *)extra;
	int ret = 0;
	int modulation = 0, mode = 0;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
	
	down(&priv->wx_sem);

	if (*param == 1) {
		modulation |= IEEE80211_CCK_MODULATION;
		mode = IEEE_B;
	} else if (*param == 2) {
		modulation |= IEEE80211_OFDM_MODULATION;
		mode = IEEE_G;
	} else if (*param == 3) {
		modulation |= IEEE80211_CCK_MODULATION;
		modulation |= IEEE80211_OFDM_MODULATION;
		mode = IEEE_B|IEEE_G;
	}

	if(ieee->proto_started) {
		ieee80211_stop_protocol(ieee);
		ieee->mode = mode;
		ieee->modulation = modulation;
		ieee80211_start_protocol(ieee);
	} else {
		ieee->mode = mode;
		ieee->modulation = modulation;
	}

	up(&priv->wx_sem);

	return ret;
}

static int r8180_wx_set_preamble(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int ret = 0;	
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
	
	down(&priv->wx_sem);
	if (*extra<0||*extra>2)
		ret = -1;
	else
		priv->plcp_preamble_mode = *((short *)extra) ;	
	up(&priv->wx_sem);

	return ret;
}

static int r8180_wx_get_preamble(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	down(&priv->wx_sem);
	*extra = (char) priv->plcp_preamble_mode; 	
	up(&priv->wx_sem);

	return 0;
}

static int r8180_wx_get_siglevel(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int ret = 0;
	down(&priv->wx_sem);
	*((int *)extra) = priv->wstats.qual.level;
	up(&priv->wx_sem);

	return ret;
}

static int r8180_wx_get_sigqual(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int ret = 0;
	down(&priv->wx_sem);
	*((int *)extra) = priv->wstats.qual.qual;
	up(&priv->wx_sem);

	return ret;
}

static int r8180_wx_reset_stats(struct net_device *dev,
				struct iw_request_info *info,
				union iwreq_data *wrqu, char *extra)
{	
	struct r8180_priv *priv =ieee80211_priv(dev);
	
	down(&priv->wx_sem);

	priv->stats.txrdu = 0;
	priv->stats.txnperr = 0;
	priv->stats.txresumed = 0;
	priv->stats.rxerr = 0;
	priv->stats.rxint = 0; 

	priv->stats.txnpokint = 0;
	priv->stats.txhpokint = 0;
	priv->stats.txhperr = 0;
	priv->stats.txoverflow = 0;
	priv->stats.txretry=0;
	priv->stats.txok=0;
	priv->stats.txerr=0;
	priv->stats.txbeaconokint = 0;
	priv->stats.txbeaconerr = 0;
	priv->stats.txlpokint = 0;
	priv->stats.txlperr = 0;
	priv->stats.rxok=0;
	priv->stats.rxretry =0;
	priv->stats.rxcrcerrmin=0;
	priv->stats.rxcrcerrmid=0;
	priv->stats.rxcrcerrmax=0;
	priv->stats.rxicverr=0;

	up(&priv->wx_sem);

	return 0;

}

static int r8180_wx_radio_on(struct net_device *dev,
				struct iw_request_info *info,
				union iwreq_data *wrqu, char *extra)
{	
	struct r8180_priv *priv =ieee80211_priv(dev);

	down(&priv->wx_sem);
	priv->rf_wakeup(dev);
#ifdef POLLING_METHOD_FOR_RADIO
	priv->iwpriv_radio_on = 1;
#endif
	up(&priv->wx_sem);

	return 0;

}

static int r8180_wx_radio_off(struct net_device *dev,
				struct iw_request_info *info,
				union iwreq_data *wrqu, char *extra)
{	
	struct r8180_priv *priv =ieee80211_priv(dev);

	down(&priv->wx_sem);
	priv->rf_sleep(dev);
#ifdef POLLING_METHOD_FOR_RADIO
	priv->iwpriv_radio_on = 0;
#endif
	up(&priv->wx_sem);

	return 0;

}

static int r8180_wx_set_channelplan(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	int *val = (int *)extra;
	int i;
	
	if(priv->ieee80211->bHwRadioOff)
		return 0;
	
	down(&priv->wx_sem);
	if (DefaultChannelPlan[*val].Len != 0){
		priv ->channel_plan = *val;
		for (i=1;i<=MAX_CHANNEL_NUMBER;i++)
			priv->ieee80211->channel_map[i] = 0;
		for (i=1;i<=DefaultChannelPlan[*val].Len;i++) 
			priv->ieee80211->channel_map[DefaultChannelPlan[*val].Channel[i-1]] = 1;
	}

	up(&priv->wx_sem);

	return 0;
}

static int r8180_wx_get_channelplan(struct net_device *dev, 
			     struct iw_request_info *info, 
			     union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);
	down(&priv->wx_sem);
	*extra = priv->channel_plan;
	up(&priv->wx_sem);

	return 0;
}

static int r8180_wx_get_version(struct net_device *dev, 
			       struct iw_request_info *info, 
			       union iwreq_data *wrqu, char *extra)
{
	struct r8180_priv *priv = ieee80211_priv(dev);

	down(&priv->wx_sem);
	strcpy(extra, "1020.0808");
	up(&priv->wx_sem);

	return 0;
}

static iw_handler r8180_wx_handlers[] =
{
        NULL,                     /* SIOCSIWCOMMIT */
        r8180_wx_get_name,   	  /* SIOCGIWNAME */
        dummy,                    /* SIOCSIWNWID */
        dummy,                    /* SIOCGIWNWID */
        r8180_wx_set_freq,        /* SIOCSIWFREQ */
        r8180_wx_get_freq,        /* SIOCGIWFREQ */
        r8180_wx_set_mode,        /* SIOCSIWMODE */
        r8180_wx_get_mode,        /* SIOCGIWMODE */
        r8180_wx_set_sens,        /* SIOCSIWSENS */
        r8180_wx_get_sens,        /* SIOCGIWSENS */
        NULL,                     /* SIOCSIWRANGE */
        rtl8180_wx_get_range,	  /* SIOCGIWRANGE */
        NULL,                     /* SIOCSIWPRIV */
        NULL,                     /* SIOCGIWPRIV */
        NULL,                     /* SIOCSIWSTATS */
        NULL,                     /* SIOCGIWSTATS */
        dummy,                    /* SIOCSIWSPY */
        dummy,                    /* SIOCGIWSPY */
        NULL,                     /* SIOCGIWTHRSPY */
        NULL,                     /* SIOCWIWTHRSPY */
        r8180_wx_set_wap,      	  /* SIOCSIWAP */
        r8180_wx_get_wap,         /* SIOCGIWAP */
#if (WIRELESS_EXT >= 18)
        r8180_wx_set_mlme, 
#else
  	NULL,
#endif
        dummy,                     /* SIOCGIWAPLIST -- depricated */
        r8180_wx_set_scan,        /* SIOCSIWSCAN */
        r8180_wx_get_scan,        /* SIOCGIWSCAN */
        r8180_wx_set_essid,       /* SIOCSIWESSID */
        r8180_wx_get_essid,       /* SIOCGIWESSID */
        dummy,                    /* SIOCSIWNICKN */
        dummy,                    /* SIOCGIWNICKN */
        NULL,                     /* -- hole -- */
        NULL,                     /* -- hole -- */
        r8180_wx_set_rate,        /* SIOCSIWRATE */
        r8180_wx_get_rate,        /* SIOCGIWRATE */
        r8180_wx_set_rts,                    /* SIOCSIWRTS */
        r8180_wx_get_rts,                    /* SIOCGIWRTS */
        r8180_wx_set_frag,        /* SIOCSIWFRAG */
        r8180_wx_get_frag,        /* SIOCGIWFRAG */
        dummy,                    /* SIOCSIWTXPOW */
        dummy,                    /* SIOCGIWTXPOW */
        r8180_wx_set_retry,       /* SIOCSIWRETRY */
        r8180_wx_get_retry,       /* SIOCGIWRETRY */
        r8180_wx_set_enc,         /* SIOCSIWENCODE */
        r8180_wx_get_enc,         /* SIOCGIWENCODE */
        r8180_wx_set_power,                    /* SIOCSIWPOWER */
        r8180_wx_get_power,                    /* SIOCGIWPOWER */
	NULL,			/*---hole---*/
	NULL, 			/*---hole---*/
	r8180_wx_set_gen_ie,
	NULL, 			/* SIOCSIWGENIE */
#if (WIRELESS_EXT >= 18)
	r8180_wx_set_auth,
	NULL,
	r8180_wx_set_enc_ext, 			/* SIOCSIWENCODEEXT */
#else
	NULL,
	NULL,
	NULL,
#endif
	NULL,
	NULL, 			/* SIOCSIWPMKSA */
	NULL, 			 /*---hole---*/
}; 


static const struct iw_priv_args r8180_private_args[] = { 
	
	{
		SIOCIWFIRSTPRIV + 0x0, 
		IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "badcrc" 
	}, 
	{	SIOCIWFIRSTPRIV + 0x1, 
		0, 0, "dummy" 
		
	},
	{	SIOCIWFIRSTPRIV + 0x2, 
		0, 0, "dummy" 
		
	},
	{	SIOCIWFIRSTPRIV + 0x3, 
		0, 0, "dummy" 
		
	},
	{
		SIOCIWFIRSTPRIV + 0x4,
		IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "activescan"
	
	},
	{	SIOCIWFIRSTPRIV + 0x5, 
		0, 0, "dummy" 
		
	},
	{
		SIOCIWFIRSTPRIV + 0x6, 
		IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "rawtx" 
	},
	{	SIOCIWFIRSTPRIV + 0x7, 
		0, 0, "dummy" 
		
	},
	{
		SIOCIWFIRSTPRIV + 0x8, 
		IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "setiwmode" 
	},
	{
		SIOCIWFIRSTPRIV + 0x9, 
		0, IW_PRIV_TYPE_CHAR | IW_PRIV_SIZE_FIXED | 32, "getiwmode" 
	},
	{
		SIOCIWFIRSTPRIV + 0xA, 
		IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "setpreamble" 
	},
	{
		SIOCIWFIRSTPRIV + 0xB,
		0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "getpreamble"
	},
	{	SIOCIWFIRSTPRIV + 0xC, 
		0, 0, "dummy" 	
	},
	{
		SIOCIWFIRSTPRIV + 0xD, 
		0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "getrssi" 
	},
	{	SIOCIWFIRSTPRIV + 0xE, 
		0, 0, "dummy" 	
	},
	{
		SIOCIWFIRSTPRIV + 0xF, 
		0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "getlinkqual"
 	},
	{
		SIOCIWFIRSTPRIV + 0x10, 
		0, 0, "resetstats"
 	},
	{
		SIOCIWFIRSTPRIV + 0x11, 
		0,0, "dummy"
 	},
	{
		SIOCIWFIRSTPRIV + 0x12, 
		0, 0, "radioon"
 	},
	{
		SIOCIWFIRSTPRIV + 0x13, 
		0, 0, "radiooff"
 	},
	{
		SIOCIWFIRSTPRIV + 0x14, 
		IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, 0, "setchannel"
 	},
	{
		SIOCIWFIRSTPRIV + 0x15, 
		0, IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1, "getchannel"
 	},
	{
		SIOCIWFIRSTPRIV + 0x16, 
		0,0, "dummy"
 	},
	{
		SIOCIWFIRSTPRIV + 0x17, 
		0,IW_PRIV_TYPE_CHAR | IW_PRIV_SIZE_FIXED | 32, "getversion"
 	},
};


static iw_handler r8180_private_handler[] = {
	r8180_wx_set_crcmon,   /*SIOCIWSECONDPRIV*/
	dummy,
	dummy,
	dummy,
	r8180_wx_set_scan_type,
	dummy,
	r8180_wx_set_rawtx,
	dummy,
	r8180_wx_set_iwmode,
	r8180_wx_get_iwmode,
	r8180_wx_set_preamble,
	r8180_wx_get_preamble,
	dummy,
	r8180_wx_get_siglevel,
	dummy,
	r8180_wx_get_sigqual,
	r8180_wx_reset_stats,
	dummy,
	r8180_wx_radio_on,
	r8180_wx_radio_off,
	r8180_wx_set_channelplan,
	r8180_wx_get_channelplan,
	dummy,
	r8180_wx_get_version,
};

#if WIRELESS_EXT >= 17	
static inline int is_same_network(struct ieee80211_network *src,
                                  struct ieee80211_network *dst,
				  struct ieee80211_device *ieee)
{
        return (((src->ssid_len == dst->ssid_len)||(ieee->iw_mode == IW_MODE_INFRA)) &&  
			(src->channel == dst->channel) &&
			!memcmp(src->bssid, dst->bssid, ETH_ALEN) &&
			(!memcmp(src->ssid, dst->ssid, src->ssid_len)||(ieee->iw_mode == IW_MODE_INFRA)) &&  
			((src->capability & WLAN_CAPABILITY_IBSS) ==
			(dst->capability & WLAN_CAPABILITY_IBSS)) &&
			((src->capability & WLAN_CAPABILITY_BSS) ==
			(dst->capability & WLAN_CAPABILITY_BSS)));
}

static struct iw_statistics *r8180_get_wireless_stats(struct net_device *dev)
{
       struct r8180_priv *priv = ieee80211_priv(dev);
	struct ieee80211_device* ieee = priv->ieee80211;
	struct iw_statistics* wstats = &priv->wstats;
	int tmp_level = 0;
	int tmp_qual = 0;
	int tmp_noise = 0;

	if (ieee->state < IEEE80211_LINKED)
	{
		wstats->qual.qual = 0;
		wstats->qual.level = 0;
		wstats->qual.noise = 0;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14))
		wstats->qual.updated = IW_QUAL_ALL_UPDATED | IW_QUAL_DBM;
#else
		wstats->qual.updated = 0x0f;
#endif
		return wstats;
	}	
#if 0
	spin_lock_irqsave(&ieee->lock, flag);
	list_for_each_entry(target, &ieee->network_list, list)
	{
		if (is_same_network(target, &ieee->current_network))
		{
			printk("it's same network:%s\n", target->ssid);
#if 0
			if (!tmp_level)
			{
				tmp_level = target->stats.signalstrength;
				tmp_qual = target->stats.signal;
			}
			else
			{

				tmp_level = (15*tmp_level + target->stats.signalstrength)/16;
				tmp_qual = (15*tmp_qual + target->stats.signal)/16;
			}
#else
			tmp_level = target->stats.signal;
			tmp_qual = target->stats.signalstrength;
			tmp_noise = target->stats.noise;			
			printk("level:%d, qual:%d, noise:%d\n", tmp_level, tmp_qual, tmp_noise);
#endif
			break;
		}
	}
	spin_unlock_irqrestore(&ieee->lock, flag);
#endif
	tmp_level = (&ieee->current_network)->stats.signal;
	tmp_qual = (&ieee->current_network)->stats.signalstrength;
	tmp_noise = (&ieee->current_network)->stats.noise;			
	
	wstats->qual.level = tmp_level;
	wstats->qual.qual = tmp_qual;
	wstats->qual.noise = tmp_noise;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14))
	wstats->qual.updated = IW_QUAL_ALL_UPDATED| IW_QUAL_DBM;
#else
	wstats->qual.updated = 0x0f;
#endif
	return wstats;
}
#endif


struct iw_handler_def  r8180_wx_handlers_def={
	.standard = r8180_wx_handlers,
	.num_standard = sizeof(r8180_wx_handlers) / sizeof(iw_handler),
	.private = r8180_private_handler,
	.num_private = sizeof(r8180_private_handler) / sizeof(iw_handler),
 	.num_private_args = sizeof(r8180_private_args) / sizeof(struct iw_priv_args),
#if WIRELESS_EXT >= 17	
	.get_wireless_stats = r8180_get_wireless_stats,
#endif
	.private_args = (struct iw_priv_args *)r8180_private_args,	
};

