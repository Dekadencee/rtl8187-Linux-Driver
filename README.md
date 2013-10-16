Realtek 8187L Linux Driver
==========================

This is the oficial driver of Realtek 8187L chipset, with some modifications and upgrades to work fine in > 3.80 Linux kernel.

Tested on Ubuntu 13.04 (Linux kernel 3.8.0-31) with usb card ALFA Networks AWUS036H, used for daily use (navigation, downloads...) and test security of wireless networks with some utils (e.g aircrack-ng)


### Installing driver

1) Download the updated driver from GitHub:

    wget https://github.com/Dekadencee/rtl8187-Linux-Driver/archive/master.zip

2) Unzip file and remove zip:

    unzip master.zip
    rm master.zip

3) Compile driver:

    cd master
    sudo make
    sudo make install

4) Backlist the native drivers to aboid conflict:

    echo "blacklist rtl8187" | sudo tee -a /etc/modprobe.d/blacklist.conf

5) Reboot or reload drivers manually removing the native one:

    sudo modprobe -rfv rtl8187
    sudo modprobe -v r8187l


### Thanks

Special thanks to varunendra (http://ubuntuforums.org/member.php?u=1043629) for the guides and tips on Ubuntu forums
