# name=input("please input your name\n")
# print("myname is",name)

# a=188888888888888888888888888888
# if a>=0:
#     print(a*a)
# else:
#     print("xiao yu 0")

# a=True
# b=False
# print(a and not b)
# def hhhh() : 
#     print(10/3,10//3)
    
# hhhh()

# def application(environ,start_response):
#     #environ:一个包含所有HTTP请求的dict对象
#     #start_reponse :一个发送HTTP请求响应的函数
#     start_response('200 OK',[('Content-Type','text/html')])
#     body='<h1>Hello,%s!</h1>' %(environ['PATH_INFO'][0:]or 'web')
#     ##调用： start_response 就发送了HTTP响应的Header，注意Header只能发送一次，
#     # 也就是只能调用一次start_response()函数   
#     return [body.encode('utf-8')]

def application(environ,start_response):
    method=environ['REQUEST_METHOD']
    path=environ['PATH_INFO']
    if method=='GET'and path=='/':
        return handle_home(environ,start_response)
    if method=='POST'and path=='/signin':
        return handle_signin(environ,start_response)
