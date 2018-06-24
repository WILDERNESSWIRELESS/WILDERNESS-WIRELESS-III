import picoweb
 
app = picoweb.WebApp(__name__)

@app.route("/")
def index(req, resp):
    yield from picoweb.start_response(resp, content_type = "text/html")
 
    htmlFile = open('index.html', 'r')
 
    for line in htmlFile:
      yield from resp.awrite(line)
 
#app.run(debug=True, host="192.168.1.33",port=80)
app.run(debug=True, host="127.0.0.1", port=80)
