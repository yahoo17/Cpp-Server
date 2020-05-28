from django.db import models
import datetime
from django.utils import timezone
#新加入的 import datetime 和 from django.utils import timezone 分别导入了 Python 的标准 datetime 模块和 Django 中和时区相关的 django.utils.timezone 工具模块。

class Question(models.Model):
    question_text=models.CharField(max_length=200)

    pub_date=models.DateTimeField('date published created by myself')

    def __str__(self):
        return self.question_text
    def was_published_recently(self):
        return self.pub_date>=timezone.now()-datetime.timedelta(days=1)

class Choice(models.Model):
    question =models.ForeignKey(Question,on_delete=models.CASCADE)

    choice_text=models.CharField(max_length=200)
    votes=models.IntegerField(default=0)

    def __str__(self):
        return self.choice_text

