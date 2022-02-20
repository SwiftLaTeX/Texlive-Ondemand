from gevent.pywsgi import WSGIServer
from app import app

http_server = WSGIServer(('0.0.0.0', 5001), app)
http_server.serve_forever()
