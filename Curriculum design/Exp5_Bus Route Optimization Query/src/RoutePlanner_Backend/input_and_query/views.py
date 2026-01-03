from django.shortcuts import render
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_http_methods
from .dll_wrapper import RoutePlanner
from .models import BusLine
import json


# Create your views here.
@csrf_exempt
@require_http_methods(["POST"])
def add_line(request):
    try:
        data = json.loads(request.body)
        BusLine.objects.create(
            name=data['name'],
            stations=data['stations'],
            price=data['price'],
            interval=data['interval'],
            speed=data['speed']
        )
        return JsonResponse({'status': 'success'}, status=201)
    except Exception as e:
        return JsonResponse({'status': 'error', 'message': str(e)}, status=400)


@csrf_exempt
@require_http_methods(["POST"])
def query_line(request):
    try:
        # 从 POST 的 JSON 中读取参数
        data = json.loads(request.body)
        mode = data.get('mode', 'cheapest')
        start = data['start']
        end = data['end']

        # 创建规划器并加载所有线路
        planner = RoutePlanner()
        planner.reload_lines()

        # 调用 DLL 查询路径
        result = planner.dll.query_route(
            mode.encode('utf-8'),
            start.encode('utf-8'),
            end.encode('utf-8')
        )

        if not result:
            return JsonResponse({'status': 'error', 'message': 'No route found.'}, status=404)

        return JsonResponse({'status': 'success', 'route': result.decode('utf-8')}, status=200)

    except KeyError:
        return JsonResponse({'status': 'error', 'message': 'Missing required parameters (start/end).'}, status=400)
    except Exception as e:
        return JsonResponse({'status': 'error', 'message': str(e)}, status=500)


@csrf_exempt
@require_http_methods(["GET"])
def get_lines(request):
    try:
        lines = BusLine.objects.all()
        data = []
        for line in lines:
            data.append({
                'name': line.name,
                'price': line.price,
                'interval': line.interval,
                'speed': line.speed,
                'stations': line.stations
            })
        return JsonResponse({'status': 'success', 'lines': data}, status=200)
    except Exception as e:
        return JsonResponse({'status': 'error', 'message': str(e)}, status=500)
