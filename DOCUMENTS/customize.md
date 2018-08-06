# Customizing the Web Page

## Preparation

You can get it for your OS here:

http://freeware.the-meiers.org/

Install adafruit-ampy

- TO DO

## STOP main.py PROCESS

### COOLTERM

Set connection OPTIONS
- Set PORT to /dev/tty.SLAB_USBtoUART (OSX)
- Set BAUD to 115200

Connect

Type ^C (control-C)
(You should see a prompt >>> after doing so)

```
import os
os.remove("main.py")
```

Disconnect

### RESET ESP32 (Feather)

### TERMINAL

```
ampy put your_customized_index.html
ampy put main.py
```
### RESET ESP32 (Feather)
