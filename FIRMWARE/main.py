from microDNSSrv import MicroDNSSrv
import logging
import picoweb
import ure as re
from machine import Pin
#import os

# Resolve all DNS queries to local host
MicroDNSSrv.Create({'*':'10.0.0.1'})
# Start logging
log=logging.getLogger("BIB")

# Orange LED on Pin 10 will indicate this script is running
p27 = Pin(27, Pin.OUT)
p27.value(1)

# Yellow LED on Pin 09 will blink when HTTP request is received
p33 = Pin(33, Pin.OUT)
p33.value(0)

# Need some way of interrupting this app. Use GPIO 21
#p21 = Pin(21, Pin.IN, Pin.PULL_UP)
#p21.irq(trigger=Pin.IRQ_FALLING, handler=callback)

app = picoweb.WebApp(__name__)

@app.route(re.compile('.'))
def home(req, resp):
    p33.value(1)
    if req.path != "/home":
        yield from picoweb.start_response(resp,status="302",headers={'Location':'home'})
    yield from picoweb.start_response(resp,status="200", content_type="text/html")
    htmlFile = open("index.html", "r")
    for line in htmlFile:
        yield from resp.awrite(line)
    p33.value(0)

#def callback(p):
#    print("Received an interrupt")


app.run(debug=1, host = "10.0.0.1", port = 80)
