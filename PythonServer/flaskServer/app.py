from flask import Flask
from flask import request

app=Flask(__name__)

@app.route('/',methods=['GET','POST'])
def home():
    return '<h1>Home Page</h1>'

@app.route('/signin',methods=['GET'])
def signin_form():
    return '''<form action="/signin" method ="POST">
                <p><input name="username"></p>
                <p><input name="password"></p>
                <p><button type="登陆">Sign In</button></p>
                </form>
                <html>
    <head>
        <title>
            HELLO
        </title>
        
        <style>
     
            h1{
                color:#333333;
                font-size: 48;
                text-shadow: 3px 3px 3px #666666;
                
            }
        </style>
        <script>
            function change(){
                var a=true;
                if(a)
                    {
                        document.getElementsByTagName('h1')[0].style.color='#ff0000';
                        a=false;

                    }
                    
                else
                    document.getElementsByTagName('h1')[0].style.color='#666666';
            }
        </script>
    </head>    
    <body>
      
       <h1 onclick="change()">hello world</h1>
    </body>
    
</html>
                '''

@app.route('/signin',methods=['POST'])
def signin():
    # 需要从request对象读取表单内容
    if request.form['username']=='admin' and request.form['password']=='password':
        return '<h3>HELLO ,admin</h3>'
    return '<h3>Bad userName or Password.</h3>'

if __name__=='__main__':
    app.run()