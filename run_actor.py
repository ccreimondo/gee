#! .pyenv/bin/python2
from actor import app
from flask import request, redirect

# for debug
@app.before_request
def add_static():
    if not app.debug:
        return
    path = request.path
    if path == "/":
        return redirect("/static/index.html")


# allow cors for quick development
@app.after_request
def add_cors(res):
    if not app.debug:
        return res
    res.headers["Access-Control-Allow-Origin"] = request.headers.get("Origin", '*')
    res.headers["Access-Control-Allow-Credentials"] = "true"
    res.headers["Access-Control-Allow-Methods"] = "POST, GET"
    res.headers["Access-Control-Max-Age"] = '1'
    res.headers["Server"] = app.config["APP_NAME"]
    return res


def main():
    app.run(host="0.0.0.0", port=5000, debug=True, threaded=True)


if __name__ == "__main__":
    main()
