import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/route_provider.dart';
import '../widgets/line_map.dart';

class MapScreen extends StatefulWidget {
  const MapScreen({Key? key}) : super(key: key);

  @override
  _MapScreenState createState() => _MapScreenState();
}

class _MapScreenState extends State<MapScreen> {
  double scale = 1.0;

  @override
  Widget build(BuildContext context) {
    final prov = context.watch<RouteProvider>();

    return Scaffold(
      appBar: AppBar(title: const Text('查询结果'), centerTitle: true),
      body: Center(
        child: InteractiveViewer(
          boundaryMargin: EdgeInsets.all(double.infinity),
          minScale: 0.5,
          maxScale: 3.0,
          onInteractionUpdate: (details) {
            setState(() => scale = details.scale.clamp(0.5, 3.0));
          },
          child: LineMap(
            lines: prov.lines,
            highlightedStations: prov.route.toSet(),
            highlightedLines: prov.routeLines.toSet(),
            scale: scale,
          ),
        ),
      ),
    );
  }
}
