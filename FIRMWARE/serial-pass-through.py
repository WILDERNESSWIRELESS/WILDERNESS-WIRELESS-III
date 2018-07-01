from machine import UART

UART.init(9600, bits=8, parity=None, stop=1) # init with given parameters

while True:
    if UART.any()>0:
        print(UART.read())
