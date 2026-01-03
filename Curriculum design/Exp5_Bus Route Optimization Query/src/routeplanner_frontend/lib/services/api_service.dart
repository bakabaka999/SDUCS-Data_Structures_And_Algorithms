import 'dart:async';
import 'dart:convert';
import 'package:http/http.dart' as http;
import '../models/bus_line.dart';

class ApiService {
  static const _baseUrl = 'http://8.148.65.255:8000/input_and_query';

  static Future<List<BusLine>> fetchLines() async {
    final resp = await http.get(Uri.parse('$_baseUrl/line/list/'));
    if (resp.statusCode == 200) {
      final Map<String, dynamic> body = json.decode(resp.body);
      final List<dynamic> lines = body['lines'];
      return lines
          .asMap()
          .entries
          .map((e) => BusLine.fromJson(e.value, e.key))
          .toList();
    }
    throw Exception('加载线路失败：${resp.statusCode}');
  }

  static Future<void> addLine(Map<String, dynamic> lineJson) async {
    final resp = await http.post(
      Uri.parse('$_baseUrl/line/add/'),
      headers: {'Content-Type': 'application/json'},
      body: json.encode(lineJson),
    );
    if (resp.statusCode != 201) {
      throw Exception('添加线路失败：${resp.body}');
    }
  }

  static Future<List<String>> queryRoute({
    required String start,
    required String end,
    required String mode,
  }) async {
    try {
      print('开始查询路线：起点=$start, 终点=$end, 方式=$mode');

      final resp = await http.post(
        Uri.parse('$_baseUrl/query/'),
        headers: {'Content-Type': 'application/json'},
        body: json.encode({
          'start': start,
          'end': end,
          'mode': mode,
        }),
      );

      // 打印响应的状态码和返回内容
      print('响应状态码：${resp.statusCode}');
      print('响应内容：${resp.body}');

      if (resp.statusCode == 200) {
        final data = json.decode(resp.body);

        // 解析 'route' 字段中的字符串，转成 JSON 对象
        final routeJson = json.decode(data['route']); // 解析 'route' 字段

        // 输出获取到的数据
        print('查询到的路线数据：$routeJson');

        // 返回路径列表
        return List<String>.from(routeJson['path']);
      }
      throw Exception('查询失败：${resp.body}');
    } catch (e) {
      // 捕获并打印异常信息
      print('异常发生: $e');
      rethrow; // 重新抛出异常
    }
  }

}
