NIC_SELECT = RTL8187L

CC = gcc
KVER  := $(shell uname -r)
MODDESTDIR := /lib/modules/$(KVER)/kernel/drivers/net/wireless/$(NIC_SELECT)
KSRC := /lib/modules/$(KVER)/build
PWD = $(shell pwd)
INSTALL_PREFIX :=

ifneq ($(shell uname -r|cut -d. -f1,2), 2.4)
EXTRA_CFLAGS += -I$(TOPDIR)/drivers/net/wireless
EXTRA_CFLAGS += -std=gnu89
EXTRA_CFLAGS += -O2

EXTRA_CFLAGS += -DPOLLING_METHOD_FOR_RADIO
EXTRA_CFLAGS += -DLED
#EXTRA_CFLAGS += -DLED_SHIN
#EXTRA_CFLAGS += -DSW_ANTE_DIVERSITY

ieee80211-rtl-objs := ieee80211_softmac.o \
	ieee80211_rx.o \
	ieee80211_tx.o \
	ieee80211_wx.o \
	ieee80211_module.o \
	ieee80211_softmac_wx.o

ieee80211_crypt-rtl-objs := ieee80211_crypt.o
ieee80211_crypt_tkip-rtl-objs := ieee80211_crypt_tkip.o
ieee80211_crypt_ccmp-rtl-objs := ieee80211_crypt_ccmp.o
ieee80211_crypt_wep-rtl-objs := ieee80211_crypt_wep.o

obj-m +=ieee80211-rtl.o
obj-m +=ieee80211_crypt-rtl.o
obj-m +=ieee80211_crypt_wep-rtl.o
obj-m +=ieee80211_crypt_tkip-rtl.o
obj-m +=ieee80211_crypt_ccmp-rtl.o


all: modules

modules:
	$(MAKE) -C $(KSRC) M=$(PWD) CC=$(CC) modules

clean:
	rm -f *.mod.c *.mod *.o .*.cmd *.ko *~
	rm -rf $(PWD)/tmp
	rm -fr Module.symvers
	rm -fr modules.order
	rm -fr Module.markers

install: modules
	rm -fr $(MODDESTDIR)
	mkdir -p $(MODDESTDIR)
	@install -p -m 644 ieee80211_crypt-rtl.ko $(MODDESTDIR)
	@install -p -m 644 ieee80211_crypt_wep-rtl.ko $(MODDESTDIR)
	@install -p -m 644 ieee80211_crypt_tkip-rtl.ko $(MODDESTDIR)
	@install -p -m 644 ieee80211_crypt_ccmp-rtl.ko $(MODDESTDIR)
	@install -p -m 644 ieee80211-rtl.ko $(MODDESTDIR)
	depmod -a
uninstall:
	rm -fr $(MODDESTDIR)
	depmod -a 

else #2.4 kernel

LD := ld
CONFIG_FILE := $(KSRC)/include/linux/autoconf.h

CFLAGS += -DLINUX -D__KERNEL__ -DMODULE -O2 -pipe -Wall
CFLAGS += -I$(KSRC)/include -I.
#Kernel 2.4.31
CFLAGS += -DMODVERSIONS -DEXPORT_SYMTAB -include $(KSRC)/include/linux/modversions.h
#Kernel 2.4.20
#CFLAGS += -D__NO_VERSION__ -DEXPORT_SYMTAB 

CFLAGS += -DPOLLING_METHOD_FOR_RADIO
CFLAGS += -DLED
#CFLAGS += -DLED_SHIN
#CFLAGS += -DSW_ANTE_DIVERSITY

SMP := $(shell $(CC) $(MODCFLAGS) -E -dM $(CONFIG_FILE) | \
   grep CONFIG_SMP | awk '{print $$3}')
ifneq ($(SMP),1)
   SMP := 0
endif
ifeq ($(SMP),1)
   CFLAGS    += -D__SMP__
endif

OBJS := ${patsubst %.c, %.o, ${wildcard *.c}}
all:${OBJS} ieee80211_crypt-rsl.o \
	michael_mic-rsl.o \
	aes-rsl.o \
	ieee80211_crypt_wep-rsl.o \
	ieee80211_crypt_tkip-rsl.o \
	ieee80211_crypt_ccmp-rsl.o \
	crypto-rsl.o ieee80211-rsl.o

ieee80211_crypt-rsl.o: ieee80211_crypt.o
	mv $^ $@	

michael_mic-rsl.o: michael_mic.o
	mv $^ $@

aes-rsl.o: aes.o
	mv $^ $@

ieee80211_crypt_wep-rsl.o: ieee80211_crypt_wep.o
	mv $^ $@

ieee80211_crypt_tkip-rsl.o: ieee80211_crypt_tkip.o
	mv $^ $@

ieee80211_crypt_ccmp-rsl.o: ieee80211_crypt_ccmp.o
	mv $^ $@

crypto-rsl.o: arc4.o \
	api.o \
	autoload.o \
	cipher.o \
	compress.o \
	digest.o \
	scatterwalk.o \
	proc.o
	$(LD) -r $^ -o $@

ieee80211-rsl.o: ieee80211_rx.o \
	ieee80211_tx.o \
	ieee80211_wx.o \
	ieee80211_module.o \
	ieee80211_softmac_wx.o \
	ieee80211_softmac.o 
	$(LD) -r $^ -o $@
install:
	rm -fr $(MODDESTDIR)
	mkdir -p $(MODDESTDIR)
	@install -p -m 644 ieee80211_crypt-rsl.o $(MODDESTDIR)
	@install -p -m 644 crypto-rsl.o $(MODDESTDIR)
	@install -p -m 644 michael_mic-rsl.o $(MODDESTDIR)
	@install -p -m 644 aes-rsl.o $(MODDESTDIR)
	@install -p -m 644 ieee80211_crypt_wep-rsl.o $(MODDESTDIR)
	@install -p -m 644 ieee80211_crypt_tkip-rsl.o $(MODDESTDIR)
	@install -p -m 644 ieee80211_crypt_ccmp-rsl.o $(MODDESTDIR)
	@install -p -m 644 ieee80211-rsl.o $(MODDESTDIR)
	/sbin/depmod -a ${shell uname -r}

uninstall:
	rm -fr $(MODDESTDIR)
	/sbin/depmod -a ${shell uname -r}

clean:
	rm -f *.mod.c *.mod *.o .*.cmd *.ko *~
	rm -rf $(PWD)/tmp
	rm -rf *~
	rm -fr Module.symvers
	rm -fr modules.order
	rm -fr Module.markers
	rm -rf tags

endif
