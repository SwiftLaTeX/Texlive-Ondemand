from flask import Flask, send_file, make_response, send_from_directory
from threading import Lock
import time
import os.path
import pykpathsea_xetex
import pykpathsea_pdftex
from flask_cors import cross_origin
import re
import os

app = Flask(__name__)

regex = re.compile(r'[^a-zA-Z0-9 _\-\.]')

def san(name):
    return regex.sub('', name)


@app.route('/xetex/<int:fileformat>/<filename>')
@cross_origin()
def xetex_fetch_file(fileformat, filename):
    filename = san(filename)
    url = None
    if filename == "swiftlatexxetex.fmt" or filename == "xetexfontlist.txt":
        url = filename
    else:
        url = pykpathsea_xetex.find_file(filename, fileformat)

    if url is None or not os.path.isfile(url):
        return "File not found", 301
    else:
        response = make_response(send_file(url, mimetype='application/octet-stream'))
        response.headers['fileid'] = os.path.basename(url)
        response.headers['Access-Control-Expose-Headers'] = 'fileid'
        return response



@app.route('/pdftex/<int:fileformat>/<filename>')
@cross_origin()
def pdftex_fetch_file(fileformat, filename):
    filename = san(filename)
    url = None
    if filename == "swiftlatexpdftex.fmt":
        url = filename
    else:
        url = pykpathsea_pdftex.find_file(filename, fileformat)

    if url is None or not os.path.isfile(url):
        return "File not found", 301
    else:
        response = make_response(send_file(url, mimetype='application/octet-stream'))
        response.headers['fileid'] = os.path.basename(url)
        response.headers['Access-Control-Expose-Headers'] = 'fileid'
        return response
            


@app.route('/pdftex/pk/<int:dpi>/<filename>')
@cross_origin()
def pdftex_fetch_pk(dpi, filename):
    filename = san(filename)
    
    url = pykpathsea_pdftex.find_pk(filename, dpi)

    if url is None or not os.path.isfile(url):
        return "File not found", 301
    else:
        response = make_response(send_file(url, mimetype='application/octet-stream'))
        response.headers['pkid'] = os.path.basename(url)
        response.headers['Access-Control-Expose-Headers'] = 'pkid'
        return response




