# coding=utf-8

from actor import app

app.config.from_object(__name__)


@app.route("/")
def hello():
    return "This is actor of gee."
