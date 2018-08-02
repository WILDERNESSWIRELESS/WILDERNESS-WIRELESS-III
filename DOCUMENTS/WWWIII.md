# this workshop is mainly about energy and community

We have to work within constraints because the future may not be plentiful

Not everything is possible. How do we work with what we have?

Un-tether

Field Guide

# Wilderness Wireless Workshop III

## Day 1 (8hrs: 10:00-18:00)

- Radio as natural phenomenon
  - Origin of Universe
  - Cosmic Microwave Background: Alpher & Herman
  - Expanding Universe: Hubble & LeMaitre
  - Electromagnetic Spectrum
  - Radio and light comparison
  - Natural Radio
    - McGreevy Archive.org
- SDR
- Materials in electronics
  - Silicon
- Nature of energy, electronics crash course
- Radio TX build
- Transmission Arts
  - Sally Ann McIntyre
  - http://www.haineshinterding.net/category/hinterding/
  - Other artists...
- Radio vs Internet

- Transmissions in Hasenheide!
- After hours Pub Time?

## Day 2 (8hrs: 10:00-18:00)

- analog vs digital
- design constraints
  - not always on
  - charge/discharge cycle
  - behaviors
    - nocturnal, intermittent, shy
- hardware components
  - Solar Panel
    - How are they made?
    - Materials
    - Environmental Concerns
  - LiPo Battery
  - Battery Babysitter
    - PMIC
    - BQ27441 Fuel Gauge
  - ATMEGA328P
    - Supervisory Functions
    - Analogs
      - Circadian Rhythms
      - Sleep disruption
      - Nocturnal
  - Microcontroller ESP32
    - Python

- After hours Pub Time!

## Day 3 (8hrs: 10:00-18:00)

- construction of websites
  - HTML
  - CSS
  - Web Frameworks
- Wifi Walk
- Map of the wilderness wireless
  - QGis

- After hours Pub Time!

# Hardware Tiers

##1

↟↟↟WW↟↟↟

The Wilderness Wireless Workshop aims to break participants of their always-on mentality.

Wilderness wireless takes a free-radio approach to wireless networking

Wilderness wireless is a

Wilderness Wireless engages participants in the creation

Speculative design for a not always-on future

narrowcasting vs broadcasting

Science!
Electronics !
Soldering !
Python !
Microcontrollers!
Alternative Energy !
Radio !
Internet!
Web programming!
Transmission Art!
Command Line!

## AMPY

https://github.com/adafruit/ampy`

### to set a shell variable:

set AMPY_PORT /dev/tty.usbserialAL00FPKS

export AMPY_PORT

##REPL:
### Read, Evaluate, Print, Loop

## Installing PicoWeb
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


## AP Authmodes

0 – open
1 – WEP
2 – WPA-PSK
3 – WPA2-PSK
4 – WPA/WPA2-PSK

##ESP32S stuff


python esptool.py -p /dev/tty.SLAB_USBtoUART --baud 115200 write_flash --flash_size=detect 0x1000 ~/Desktop/esp32-20180511-v1.9.4.bin

#IoT
#History of Internet


# SCENARIOS / BEHAVIORS

SHYNESS

A WifiNetwork that only broadcasts when no one is around.

NOCTOURNALISM

A wifi network that only broadcasts at night

INTROVERT

EXTROVERT

What are the possibilities when you can hold a web server in your hand?

How can a webserver be embodied?

What is the interaction of the webserver and space?

What is a webserver?

HIBERNATION

# LED Interface

LED 0 picoweb activity

LED 1 ESP Main

LED 2 ESP Boot.py

LED 3 Blink if PWR > BATHI

LED 4 Blink if UP and BATLO < PWR < BATHI

LED 5 Blink if UP and PWR < BATLO



# LINKS

## Sparkfun Battery Babysitter Hookup Guide

https://learn.sparkfun.com/tutorials/battery-babysitter-hookup-guide

## Adafruit Solar Charger Design Notes

https://learn.adafruit.com/usb-dc-and-solar-lipoly-charger/design-notes

## National Renewable Energy Laboratory

https://www.nrel.gov/workingwithus/re-solar.html

## Wisconsin vs Foxconn

https://www.wiscontext.org/four-pollutants-illustrate-hazards-of-electronics-manufacturing

## Have We Entered the Anthropocene (2010)

http://www.igbp.net/news/opinion/opinion/haveweenteredtheanthropocene.5.d8b4c3c12bf3be638a8000578.html
