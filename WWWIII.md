# Wilderness Wireless Workshop III

## Day 1 (8hrs: 10:00-18:00)

- Radio as natural phenomenon
  - Origin of Universe
  - Electromagnetic Spectrum
  - Radio and light comparison
  - Natural Radio
    - McGreevy Archive.org
- SDR
- Materials in electronics
  - Silicon
- Nature of energy
- Radio TX build
- Transmission Arts
  - Sally Ann McIntyre
  - Other artists...
- Radio vs Internet

- After hours Pub Time!

## Day 2 (8hrs: 10:00-18:00)

- analog vs digital
- design constraints
  - not always on
  - charge/discharge cycle
  - behaviors
    - nocturnal, intermittent
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

Kogawa TX
WiFi AP in a Jam Jar
Full WWIII Build

https://www.adafruit.com/product/500 $39

https://www.sparkfun.com/products/13782 $39

POST-KOGAWA PRACTICE

↟↟↟↟↟↟↟↟

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

### to set a shell variable:

set AMPY_PORT /dev/tty.usbserialAL00FPKS

export AMPY_PORT

##REPL:
### Read, Evaluate, Print, Loop

## Installing PicoWeb
install picoweb manually from github, upload via ampy
* upip install("micropython-logging")
* upip install("micropython-pkg_resources")

## AP Authmodes

0 – open
1 – WEP
2 – WPA-PSK
3 – WPA2-PSK
4 – WPA/WPA2-PSK

##ESP32S stuff


python esptool.py -p /dev/tty.SLAB_USBtoUART --baud 115200 write_flash --flash_size=detect 0x1000 ~/Desktop/esp32-20180511-v1.9.4.bin
