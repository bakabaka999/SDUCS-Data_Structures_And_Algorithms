from django.urls import path
from . import views

urlpatterns = [
    path('line/add/', views.add_line, name='add_line'),
    path('line/list/', views.get_lines, name='get_lines'),
    path('query/', views.query_line, name='query_line')
]