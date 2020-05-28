## Django

你只需要记住，改变模型需要这三步：

- 编辑 `models.py` 文件，改变模型。
- 运行 [`python manage.py makemigrations`](https://docs.djangoproject.com/zh-hans/3.0/ref/django-admin/#django-admin-makemigrations)   为模型的改变生成迁移文件。
- 运行  [`python manage.py migrate`](https://docs.djangoproject.com/zh-hans/3.0/ref/django-admin/#django-admin-migrate)  来应用数据库迁移。



## 模板标签

**普通变量**

templates 中的 runoob.html ：

```
<p>{{ name }}</p>
```

```python
from django.shortcuts import render

def runoob(request):
  views_name = "菜鸟教程"
  return  render(request,"runoob.html", {"name":views_name})
```

templates 中的 runoob.html ：

```
<p>{{ name }}</p>
```



总结一下

```
view中返回：｛"HTML变量名" : "views变量名"｝
HTML文件中用两个括号来表示变量：｛｛变量名｝｝
```

### 列表

templates 中的 runoob.html中，可以用 . 索引下标取出对应的元素。

HelloWorld/HelloWorld/views.py 文件代码：

```
from django.shortcuts import render

def runoob(request):
    views_list = ["菜鸟教程1","菜鸟教程2","菜鸟教程3"]
    return render(request, "runoob.html", {"views_list": views_list})
```

HelloWorld/templates/runoob.html 文件代码：

```

<p>{{ views_list }}</p>   # 取出整个列表
<p>{{ views_list.0 }}</p> # 取出列表的第一个元素
```



### 字典

templates 中的 runoob.html中，可以用 .键 取出对应的值。

HelloWorld/HelloWorld/views.py 文件代码：

```
from django.shortcuts import render

def runoob(request):
    views_dict = {"name":"菜鸟教程"}
    return render(request, "runoob.html", {"views_dict": views_dict})
```

HelloWorld/templates/runoob.html 文件代码：

```

<p>{{ views_dict }}</p>
<p>{{ views_dict.name }}</p>
```



再次访问 http://127.0.0.1:8000/runoob，可以看到页面：

![img](https://www.runoob.com/wp-content/uploads/2015/01/Django_4.png)