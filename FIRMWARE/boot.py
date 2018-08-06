# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
#import webrepl
#webrepl.start()

import network
import os
import utime
import random

from machine import Pin

# Join a network: Only needed for upip installs

#sta=network.WLAN(network.STA_IF)
#sta.active(True)
#sta.connect("WildernessWireless","WWWIII2018")

# Create a network:

essids=["a________", "l________", "z________"]
ap=network.WLAN(network.AP_IF)

while True:
    ap.active(True)
    ap.ifconfig(('10.0.0.1','255.255.255.0','10.0.0.1','10.0.0.1'))
    randint = random.randint(0,2)
    ap.config(essid=essids[randint], authmode=0)
    utime.sleep(5)
    ap.active(False)
    utime.sleep(5)

# This is the signal to the AVR

p13 = Pin(13, Pin.OUT)
p13.value(1)

# RED Status LED shows if boot is ok (flashes during brownout)

p12 = Pin(12, Pin.OUT)
p12.value(1)
