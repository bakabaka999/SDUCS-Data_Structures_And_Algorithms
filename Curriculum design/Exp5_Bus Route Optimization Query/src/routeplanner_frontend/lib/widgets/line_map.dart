import 'dart:math';
import 'package:flutter/material.dart';
import '../models/bus_line.dart';

/// 根据线路名称生成柔和且唯一的颜色
Color generateUniqueColor(String lineName) {
  final hash = lineName.hashCode;
  return Color.fromARGB(
    200,
    100 + ((hash & 0xFF0000) >> 16) % 156,
    100 + ((hash & 0x00FF00) >> 8) % 156,
    100 + (hash & 0x0000FF) % 156,
  );
}

/// 可缩放、可高亮的线路图组件
class LineMap extends StatelessWidget {
  final List<BusLine> lines;
  final Set<String> highlightedStations;
  final Set<String> highlightedLines;
  final double scale;

  const LineMap({
    Key? key,
    required this.lines,
    required this.highlightedStations,
    required this.highlightedLines,
    required this.scale,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return CustomPaint(
      size: Size.infinite,
      painter: _LinePainter(
        lines: lines,
        highlightedStations: highlightedStations,
        highlightedLines: highlightedLines,
        scale: scale,
      ),
    );
  }
}

class _LinePainter extends CustomPainter {
  final List<BusLine> lines;
  final Set<String> highlightedStations;
  final Set<String> highlightedLines;
  final double scale;

  _LinePainter({
    required this.lines,
    required this.highlightedStations,
    required this.highlightedLines,
    required this.scale,
  });

  @override
  void paint(Canvas canvas, Size size) {
    // 画笔定义
    final defaultLinePaint = Paint()..strokeWidth = 4.0 * scale;
    final highlightLinePaint = Paint()
      ..strokeWidth = 6.0 * scale
      ..color = Colors.yellow;
    final stationFill = Paint()..style = PaintingStyle.fill;
    final stationStroke = Paint()
      ..style = PaintingStyle.stroke
      ..strokeWidth = 2.0 * scale
      ..color = Colors.black;

    final textPainter = TextPainter(
      textAlign: TextAlign.center,
      textDirection: TextDirection.ltr,
    );

    // 1. 坐标归一化
    final allXs = lines.expand((l) => l.stations).map((s) => s.pos.dx);
    final allYs = lines.expand((l) => l.stations).map((s) => s.pos.dy);
    final minX = allXs.reduce(min), maxX = allXs.reduce(max);
    final minY = allYs.reduce(min), maxY = allYs.reduce(max);
    final dx = maxX - minX, dy = maxY - minY;

    Offset toCanvas(Offset p) {
      final x = (p.dx - minX) / dx * size.width;
      final y = (p.dy - minY) / dy * size.height;
      return Offset(x, y);
    }

    // 2. 绘制线路与站点
    for (var line in lines) {
      final linePaint = highlightedLines.contains(line.name)
          ? highlightLinePaint
          : defaultLinePaint
        ..color = generateUniqueColor(line.name);

      // 画线
      for (int i = 0; i < line.stations.length - 1; i++) {
        final a = toCanvas(line.stations[i].pos);
        final b = toCanvas(line.stations[i + 1].pos);
        canvas.drawLine(a, b, linePaint);
      }

      // 画站点
      for (var station in line.stations) {
        final pos = toCanvas(station.pos);
        final isH = highlightedStations.contains(station.name);
        stationFill.color = isH ? Colors.red : Colors.white;
        canvas.drawCircle(pos, 8.0 * scale, stationFill);
        canvas.drawCircle(pos, 8.0 * scale, stationStroke);

        // 绘制站点名称
        textPainter.text = TextSpan(
          text: station.name,
          style: TextStyle(
            fontSize: 14.0 * scale,
            color: Colors.black,
            shadows: [Shadow(blurRadius: 2, color: Colors.white)],
          ),
        );
        textPainter.layout();
        textPainter.paint(
          canvas,
          pos + Offset(-textPainter.width / 2, 10.0 * scale),
        );
      }
    }
  }


  @override
  bool shouldRepaint(covariant CustomPainter old) => true;
}
