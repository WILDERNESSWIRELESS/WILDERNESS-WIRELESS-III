# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
#import webrepl
#webrepl.start()

import network

from machine import Pin

# This is the signal to the AVR

p23 = Pin(23, Pin.OUT)
p23.value(1)

# RED Status LED shows if boot is ok (flashes during brownout)

#p11 = Pin(11, Pin.OUT)
#p11.value(1)

# Join a network

sta=network.WLAN(network.STA_IF)
sta.active(True)
sta.connect("NETGEAR22","ancientstreet088")

# Create a network

ap=network.WLAN(network.AP_IF);
ap.active(True)
ap.config(essid=">>>><<<<", authmode=0)
ap.ifconfig(('10.0.0.1','255.255.255.0','10.0.0.1','10.0.0.1'))
