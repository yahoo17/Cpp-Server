#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from flask import Flask, request, render_template

app = Flask(__name__)

@app.route('/test', methods=['GET', 'POST'])
def test():
    if request.method=='GET':
        if request.
        return 'you send server a GET request'
    elif request.method=='POST':
        return 'you send server a POST request'
    return "hello world"

@app.route('/', methods=['GET', 'POST'])
def home():
    return "hello world"

@app.route('/signin', methods=['GET'])
def signin_form():
    return render_template('form.html')

@app.route('/signin', methods=['POST'])
def signin():
    username = request.form['username']
    password = request.form['password']
    if username=='admin' and password=='password':
        return render_template('signin-ok.html', username=username)
    return render_template('form.html', message='Bad username or password', username=username)

if __name__ == '__main__':
    app.run()