# Installing the Firmware

If not done already, install python.

If not done already, clone the WWWIII repository here:

https://github.com/WILDERNESSWIRELESS/WILDERNESS-WIRELESS-III

## Installing Micropython Firmware for ESP32

- Navigate to the TOOLS/ESPRESSIF directory
- Issue the following command

```
python esptool.py -p /dev/tty.SLAB_USBtoUART --baud 115200 write_flash --flash_size=detect 0x1000 ~/Documents/WILDERNESS-WIRELESS-III/FIRMWARE/esp32-20180511-v1.9.4.bin
```

## Installing Web and DNS Servers

## Installing Content Files
