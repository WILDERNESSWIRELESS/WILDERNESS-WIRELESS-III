from machine import UART

uart.init(9600, bits=8, parity=None, stop=1) # init with given parameters

while uart.any > 0:
    uart.write(uart.read())
