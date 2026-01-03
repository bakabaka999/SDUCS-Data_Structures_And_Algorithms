import ctypes
import os
from django.conf import settings


# C接口结构体
class CStation(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char_p),
        ("x", ctypes.c_int),
        ("y", ctypes.c_int)
    ]


class RoutePlanner:
    def __init__(self):
        self.ddl_path = os.path.join(settings.BASE_DIR, './lib/RoutePlanner.dll')
        self._load_dll()

    def _load_dll(self):
        try:
            self.dll = ctypes.CDLL(self.ddl_path)

            self.dll.add_line.argtypes = [
                ctypes.c_char_p,
                ctypes.POINTER(CStation),
                ctypes.c_int,
                ctypes.c_int,
                ctypes.c_int,
                ctypes.c_double
            ]
            self.dll.add_line.restype = None

            self.dll.query_route.argtypes = [
                ctypes.c_char_p,  # mode
                ctypes.c_char_p,  # start
                ctypes.c_char_p  # end
            ]
            self.dll.query_route.restype = ctypes.c_char_p  # 返回 JSON 字符串

            self.dll.reset_planner.restype = None

        except Exception as e:
            raise RuntimeError(f"DLL加载失败: {e}")

    def reload_lines(self):
        # 从数据库重新加载所有线路
        from .models import BusLine

        self.dll.reset_planner()

        # 遍历数据库所有交通线路
        for line in BusLine.objects.all():
            # 遍历线路所有站点
            stations = [
                (s['name'].encode(), s['x'], s['y'])
                for s in line.stations
            ]

            # 创建C结构体数组
            c_stations = (CStation * len(stations))()
            for i, (name, x, y) in enumerate(stations):
                c_stations[i].name = name
                c_stations[i].x = x
                c_stations[i].y = y

            # 调用DLL接口
            self.dll.add_line(
                line.name.encode(),
                c_stations,
                len(stations),
                line.price,
                line.interval,
                ctypes.c_double(line.speed)
            )
