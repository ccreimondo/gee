from actor import app
from flask import request, redirect


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
    app.run(host="127.0.0.1", port=8000, debug=True, threaded=True)


if __name__ == "__main__":
    main()
