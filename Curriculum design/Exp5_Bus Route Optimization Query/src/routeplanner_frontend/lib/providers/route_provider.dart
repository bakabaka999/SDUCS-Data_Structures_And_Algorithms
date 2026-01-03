// providers/route_provider.dart
import 'package:flutter/material.dart';
import '../models/bus_line.dart';
import '../services/api_service.dart';

class RouteProvider extends ChangeNotifier {
  List<BusLine> lines = [];
  List<String> route = [];
  List<String> routeLines = []; // 新增：存放被经过的线路名称

  String start = '';
  String end = '';
  String mode = 'cheapest';

  Future<void> loadLines() async {
    lines = await ApiService.fetchLines();
    notifyListeners();
  }

  void setStart(String s) {
    start = s;
    notifyListeners();
  }

  void setEnd(String e) {
    end = e;
    notifyListeners();
  }

  void setMode(String m) {
    mode = m;
    notifyListeners();
  }

  Future<void> fetchRoute() async {
    // 1. 调用后端拿到经过的站点顺序
    route = await ApiService.queryRoute(
      start: start,
      end: end,
      mode: mode,
    );

    // 2. 计算哪些线路被经过
    routeLines = _findLinesForRoute(route);

    notifyListeners();
  }

  /// 根据站点列表，找出经过了哪些线路（简单匹配相邻两站都在某条线路上）
  List<String> _findLinesForRoute(List<String> path) {
    final used = <String>{};
    for (var line in lines) {
      // 该线路所有站点名称
      final names = line.stations.map((s) => s.name).toList();
      for (int i = 0; i < path.length - 1; i++) {
        if (names.contains(path[i]) && names.contains(path[i + 1])) {
          used.add(line.name);
        }
      }
    }
    return used.toList();
  }
}
