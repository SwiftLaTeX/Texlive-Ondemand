from flask import Flask, send_file, make_response
from threading import Lock
import time
import os.path
import pykpathsea_xetex
import pykpathsea_pdftex
import pyfontconfig
from flask_cors import cross_origin
import re
import os

app = Flask(__name__)

regex = re.compile(r'[^a-zA-Z0-9 _\-\.]')

def san(name):
    return regex.sub('', name)


fontconfig_lock = Lock()
font_hit_db = {}
@app.route('/fontconfig/<fontvar>/<fontname>')
@cross_origin()
def fetch_font(fontvar, fontname):
    cacheKey = fontvar + fontname

    if not cacheKey in font_hit_db:
        requiredItalic = 0
        if 'I' in fontvar:
            requiredItalic = 1

        requiredBold = 0
        if 'B' in fontvar:
            requiredBold = 1
        
        with fontconfig_lock:
            res = pyfontconfig.find_font(san(fontname), requiredBold, requiredItalic)

        if res is None or not os.path.isfile(res) or not (res.endswith('.ttf') or res.endswith('.otf')):
            return "Font not found", 301
        else:
            font_hit_db[cacheKey] = res
    
    url = font_hit_db[cacheKey]
    response = make_response(send_file(url, mimetype='font/otf'))
    response.headers['fontid'] = os.path.basename(url)
    response.headers['Access-Control-Expose-Headers'] = 'fontid'
    return response


kpathsea_xetex_lock = Lock()
xetex_file_hit_db = {}

@app.route('/xetex/<filename>')
@cross_origin()
def xetex_fetch_file(filename):
    
    if not filename in xetex_file_hit_db:
        with kpathsea_xetex_lock:
            res = pykpathsea_xetex.find_file(san(filename))

        if res is None or not os.path.isfile(res):
            return "File not found", 301
        else:
            xetex_file_hit_db[filename] = res
    
    urls = xetex_file_hit_db[filename]
    return send_file(urls, mimetype="application/javascript")


kpathsea_pdftex_lock = Lock()
pdftex_file_hit_db = {}

@app.route('/pdftex/<filename>')
@cross_origin()
def pdftex_fetch_file(filename):
    
    if not filename in pdftex_file_hit_db:
        with kpathsea_pdftex_lock:
            res = pykpathsea_pdftex.find_file(san(filename))

        if res is None or not os.path.isfile(res):
            return "File not found", 301
        else:
            pdftex_file_hit_db[filename] = res
    
    urls = pdftex_file_hit_db[filename]
    return send_file(urls, mimetype="application/javascript")







