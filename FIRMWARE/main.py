from microDNSSrv import MicroDNSSrv
import logging
import picoweb
import ure as re
from machine import Pin

# Resolve all DNS queries to local host
MicroDNSSrv.Create({'*':'10.0.0.1'})
# Start logging
log=logging.getLogger("BIB")

# Orange LED on Pin 10 will indicate this script is running
p10 = Pin(10, Pin.OUT)
p10.value(1)

# Yellow LED on Pin 09 will blink when HTTP request is received
p09 = Pin(9, Pin.OUT)
p09.value(0)

app = picoweb.WebApp(__name__)

#@app.route("/", index)
#def index(req, resp):
#    log.info("Request: "+str(req.headers))
#    yield from picoweb.start_response(resp, status="302",headers={'Location':'/home'})
#    #yield from picoweb.start_response(resp, status="302")
#    #yield from picoweb.start_response(resp, status="200")
#    yield from resp.awrite("Hello world from picoweb running on the ESP32")

# TO DO: fix redirect loop. Does pop up CP on ios
@app.route(re.compile('.'))
def home(req, resp):
    p09.value(1)
    yield from picoweb.start_response(resp,status="302",headers={'Location':'home'})
    yield from resp.awrite("<h1>HOME</h1>")
    p09.value(0)

app.run(debug=2, host = "10.0.0.1", port = 80)
