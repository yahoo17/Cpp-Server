#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from flask import Flask, request, render_template

app = Flask(__name__)

@app.route('/test', methods=['GET', 'POST'])
def test():
    
    if request.method=='GET':      
            return 'you send server a GET request'
    elif request.method=='POST':
        return "you send server a POST request"

@app.route('/profile', methods=['GET', 'POST'])
def profile():
    
    # if request.method=='GET':      
    #         return 'you send server a GET request'
    # else request.method=='POST':
    #     return "hello world"
    if request.method=='GET':
        name=request.args.get('name','')
        if name=='yanhao':
            return 'you send server a  request for yanhao'
        elif name=='haoyan':
            return 'you send server a  request for haoyan'
        else:
            return 'you send server a request for other user'
    elif request.method=='POST':
        # 如果要在POST body里面发消息
        ## 需要在POST body里面
        print(request.form)

        name=request.json.get('name')
        if name=='yanhao':
            return dict(name='yanhao',fans=1000000,info='你给服务器我 发了一条POST,里面有 严灏 所以我给你返回')
        elif  name=='haoyan':
            return dict(name='haoyan',fans=10,info='你给服务器我 发了一条POST,里面有 灏严 所以我给你返回')
        else :
            return '你POST的BODY里 的RAW的JSON里面的name 不是我们已知的name'
        

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