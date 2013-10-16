#! /bin/sh

if [ "$1" = "RFON" ]; then
	dbus-send --system --type=method_call --dest=org.freedesktop.NetworkManager /org/freedesktop/NetworkManager org.freedesktop.NetworkManager.setWirelessEnabled boolean:true
	echo "Now Polling Method Turn  RF ON !" >> /etc/acpi/events/RadioPowerTest
else
	dbus-send --system --type=method_call --dest=org.freedesktop.NetworkManager /org/freedesktop/NetworkManager org.freedesktop.NetworkManager.setWirelessEnabled boolean:false
	echo "Now Polling Method Turn RF OFF !" >> /etc/acpi/events/RadioPowerTest
fi

