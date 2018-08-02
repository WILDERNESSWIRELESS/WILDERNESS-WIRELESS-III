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
* from terminal:
* ampy put boot.py
* connect to esp32 with coolterm
* import upip
* upip.install("micropython-logging")
* upip.install("micropython-pkg_resources")
* upip.install("micropython-uasyncio")
* upip.install("utemplate")
* import os
* os.mkdir("picoweb")
* disconnect coolterm
* go back to terminal
* ampy put __init__.py picoweb/__init__.py
* ampy put utils.py picoweb/utils.py
* ampy put microDNSSrv.py
* ampy put index.html
* ampy put style.css
* ampy put main.py
* RESET
## Installing Web and DNS Servers

## Installing Content Files
