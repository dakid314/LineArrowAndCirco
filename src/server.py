from flask import  jsonify, Flask, request
from flask import make_response, send_from_directory
from flask_cors import CORS
import hashlib
import os
import time
import base64
import subprocess
import argparse

Port = 5000
path_to_exe = ""

if os.path.exists("./out/") == False:
    os.mkdir("./out/")
if os.path.exists("./out/img/") == False:
    os.mkdir("./out/img/")
if os.path.exists("./tmp/") == False:
    os.mkdir("./tmp/")

app = Flask(__name__)
CORS(app, resources=r'/*')

libpath = os.path.join(os.path.abspath('.'), 'lib')


@app.route("/")
def index_page():
    return send_from_directory(libpath, 'index.html', as_attachment=False)

@app.route("/LineArrowAndCirco/")
def index():
    return send_from_directory(libpath, 'index.html', as_attachment=False)

@app.route("/LineArrowAndCirco/<path:filename>")
def index_file(filename):
    return send_from_directory(libpath, filename, as_attachment=False)
@app.route("/LineArrowAndCirco/LineArrowAndCirco.zip")
def zip():
    return send_from_directory(libpath, 'LineArrowAndCirco.zip', as_attachment=True)
@app.route("/<path:filename>")
def index_file_bendi(filename):
    return send_from_directory(libpath, filename, as_attachment=False)

@app.route('/LineArrowAndCirco/LineArrowAndCirco')
def LineArrowAndCirco():
    try:
        data = base64.b64decode(request.args['Data']).decode("utf-8")
        if not data:
            return data
        t = hashlib.sha256((data + str(time.time() * 1000000)).encode('utf-8'))
        filename = t.hexdigest()
        with open("./tmp/{}.data".format(filename), "w+", encoding='UTF-8') as f:
            f.write(data)
        cmdstdout = subprocess.Popen(
            f"{path_to_exe} --datapath ./ --tmppath ./ -i ./tmp/{filename}.data -o ./out/img/{filename} -w {request.args['width']} -f { '30' if int(request.args['width']) > 30 else '20'}", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        cstderr = cmdstdout.communicate()[1].decode('UTF-8')
        cmdstdout.wait()
        svgdata = str()
        pngdata = str()
        pdfdata = str()
        circo_pdfdata = str()
        with open("./out/img/{}.svg".format(filename)) as f:
            svgdata = f.read()
        with open("./out/img/{}.png".format(filename), 'rb') as f:
            pngdata = f.read()
        with open("./out/img/{}.pdf".format(filename), 'rb') as f:
            pdfdata = f.read()
        with open("./out/img/{}_circo.pdf".format(filename), 'rb') as f:
            circo_pdfdata = f.read()

        os.remove("./out/img/{}.svg".format(filename))
        os.remove("./out/img/{}.pdf".format(filename))
        os.remove("./out/img/{}_circo.pdf".format(filename))
        os.remove("./out/img/{}.png".format(filename))
        os.remove("./tmp/{}.data".format(filename))
        if request.args['D'] == '1':
            resp = make_response(svgdata)
            resp.headers["Content-Disposition"] = "attachment; filename={}.svg".format(
                filename)
            return resp
        if request.args['D'] == '2':
            resp = make_response(pdfdata)
            resp.headers["Content-Disposition"] = "attachment; filename={}.pdf".format(
                filename)
            return resp
        if request.args['D'] == '3':
            resp = make_response(circo_pdfdata)
            resp.headers["Content-Disposition"] = "attachment; filename={}_circo.pdf".format(
                filename)
            return resp
        else:
            return "<img src=\"data:image/png;base64,{}\" style=\"width: 100%; height: 100%;\"/>".format(base64.b64encode(pngdata).decode('utf-8'))
    except BaseException as e:
        print(e)
        return "<h1>Sorry, Error. May Check Your Input.</h1><h1>Serverd Error Code</h1><pre>" + str(e) + "</pre><h1>Cored Program Return</h1><p><pre>" + cstderr + "</pre></p>"
@app.route('/LineArrowAndCirco')
def LineArrowAndCirco_bendi():
    try:
        data = base64.b64decode(request.args['Data']).decode("utf-8")
        if not data:
            return data
        t = hashlib.sha256((data + str(time.time() * 1000000)).encode('utf-8'))
        filename = t.hexdigest()
        with open("./tmp/{}.data".format(filename), "w+", encoding='UTF-8') as f:
            f.write(data)
        cmdstdout = subprocess.Popen(
            f"{path_to_exe} --datapath ./ --tmppath ./ -i ./tmp/{filename}.data -o ./out/img/{filename} -w {request.args['width']} -f { '30' if int(request.args['width']) > 30 else '20'}", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        cstderr = cmdstdout.communicate()[1].decode('UTF-8')
        cmdstdout.wait()
        svgdata = str()
        pngdata = str()
        pdfdata = str()
        circo_pdfdata = str()
        with open("./out/img/{}.svg".format(filename)) as f:
            svgdata = f.read()
        with open("./out/img/{}.png".format(filename), 'rb') as f:
            pngdata = f.read()
        with open("./out/img/{}.pdf".format(filename), 'rb') as f:
            pdfdata = f.read()
        with open("./out/img/{}_circo.pdf".format(filename), 'rb') as f:
            circo_pdfdata = f.read()

        os.remove("./out/img/{}.svg".format(filename))
        os.remove("./out/img/{}.pdf".format(filename))
        os.remove("./out/img/{}_circo.pdf".format(filename))
        os.remove("./out/img/{}.png".format(filename))
        os.remove("./tmp/{}.data".format(filename))
        if request.args['D'] == '1':
            resp = make_response(svgdata)
            resp.headers["Content-Disposition"] = "attachment; filename={}.svg".format(
                filename)
            return resp
        if request.args['D'] == '2':
            resp = make_response(pdfdata)
            resp.headers["Content-Disposition"] = "attachment; filename={}.pdf".format(
                filename)
            return resp
        if request.args['D'] == '3':
            resp = make_response(circo_pdfdata)
            resp.headers["Content-Disposition"] = "attachment; filename={}_circo.pdf".format(
                filename)
            return resp
        else:
            return "<img src=\"data:image/png;base64,{}\" style=\"width: 100%; height: 100%;\"/>".format(base64.b64encode(pngdata).decode('utf-8'))
    except BaseException as e:
        print(e)
        return "<h1>Sorry, Error. May Check Your Input.</h1><h1>Serverd Error Code</h1><pre>" + str(e) + "</pre><h1>Cored Program Return</h1><p><pre>" + cstderr + "</pre></p>"

def cors_response(res):
    response = make_response(jsonify(res))
    response.headers['Access-Control-Allow-Origin'] = '*'
    response.headers['Access-Control-Allow-Methods'] = 'PUT,GET,POST,DELETE,OPTIONS'
    response.headers['Access-Control-Allow-Headers'] = 'Content-Type, X-Requested-With'
    return response


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-e', type=str, help='Path to LineArrowAndCirco.')
    parser.add_argument('--port', type=int, help='Port', default=Port)
    args = parser.parse_args()
    path_to_exe = args.e
    Port = args.port
    app.config['JSON_AS_ASCII'] = False
    app.run(host='0.0.0.0', port=Port, debug=True)
