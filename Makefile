LINUX_KSRC_MODULE = /lib/modules/$(shell uname -r)/kernel/drivers/net/wireless
RTL8187L_DIR = $(shell pwd)
KVER  = $(shell uname -r)
KSRC = /lib/modules/$(KVER)/build
HAL_SUB_DIR = rtl8187


all: 
ifeq ($(shell uname -r|cut -d. -f1,2), 2.4)
	@make -C $(RTL8187L_DIR)/ieee80211
	@make -C $(RTL8187L_DIR)/rtl8187 
else	
#	@make -C $(KSRC) SUBDIRS=$(RTL8187L_DIR)/ieee80211 modules 
#	@cp $(RTL8187L_DIR)/ieee80211/Module.symvers $(RTL8187L_DIR)/rtl8187 
	@make -C $(KSRC) SUBDIRS=$(RTL8187L_DIR)/rtl8187 modules 
endif
ifeq ($(shell uname -r|cut -d. -f1,2), 2.4)
install:
	@make -C ieee80211/ install
	@make -C rtl8187/ install
else
install:
	@grep rtl8187.ko /lib/modules/$(shell uname -r)/modules.dep && rm -fr $(LINUX_KSRC_MODULE)/rtl8187.ko || echo No system rtl8187.ko file found, Now we will install the new driver rtl8187.ko into system
	@grep rtl8187.ko /lib/modules/$(shell uname -r)/modules.dep && rm -fr $(LINUX_KSRC_MODULE)/rtl818x/rtl8187.ko || echo No system rtl8187.ko file found, Now we will install the new driver rtl8187.ko into system
	@cp $(RTL8187L_DIR)/RadioPower.sh /etc/acpi/events/ 
#	@make -C ieee80211/ install
	@make -C rtl8187/ install
endif

uninstall:
	@make -C ieee80211/ uninstall
	@make -C rtl8187/ uninstall
clean:
	@make -C rtl8187/ clean
	@make -C ieee80211/ clean
	@rm -rf *~
	@rm -rf Module.markers
	@rm -rf modules.order
	@rm -rf tags
