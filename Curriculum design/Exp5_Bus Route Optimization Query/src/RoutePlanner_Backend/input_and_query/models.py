from django.db import models
import json


# Create your models here.
class BusLine(models.Model):
    name = models.CharField(max_length=200, unique=True)
    stations = models.JSONField()
    price = models.PositiveIntegerField()
    interval = models.PositiveIntegerField(help_text="发车间隔(分钟)")
    speed = models.FloatField(help_text="运行速度(公里/小时)")

    def __str__(self):
        return f"{self.name} (票价： {self.price}元"
