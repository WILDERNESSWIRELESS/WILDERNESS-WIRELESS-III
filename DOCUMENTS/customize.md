# Customizing the Web Page

## Preparation

You can get it for your OS here:

http://freeware.the-meiers.org/

Install adafruit-ampy

- TO DO

## Stopping the main.py process

### COOLTERM

Set connection OPTIONS
- Set PORT to /dev/tty.SLAB_USBtoUART (OSX)
- Set BAUD to 115200

Connect

Type ^C (control-C)
(You should see a prompt >>> after doing so)

```
import os
os.listdir()
os.remove("main.py")
os.listdir()
```

Disconnect

### RESET ESP32 (Feather)

### TERMINAL

```
ampy put your_customized_index.html
ampy put main.py
```
### RESET ESP32 (Feather)
