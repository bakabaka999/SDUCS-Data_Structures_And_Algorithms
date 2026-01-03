import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/route_provider.dart';
import '../widgets/line_map.dart';
import '../widgets/station_input.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({Key? key}) : super(key: key);

  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  double scale = 1.0;

  @override
  Widget build(BuildContext context) {
    final prov = context.watch<RouteProvider>();
    final allStations = prov.lines
        .expand((l) => l.stations.map((s) => s.name))
        .toSet()
        .toList();

    return Scaffold(
      appBar: AppBar(title: const Text('公交线路可视化'), centerTitle: true),
      body: Column(
        children: [
          Expanded(
            flex: 3,
            child: Padding(
              padding: const EdgeInsets.all(16),
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
          ),
          Expanded(
            flex: 2,
            child: Padding(
              padding: const EdgeInsets.symmetric(horizontal: 24),
              child: Column(
                children: [
                  StationInput(
                    stations: allStations,
                    onSelected: prov.setStart,
                  ),
                  const SizedBox(height: 12),
                  StationInput(
                    stations: allStations,
                    onSelected: prov.setEnd,
                  ),
                  const SizedBox(height: 12),
                  ToggleButtons(
                    isSelected: [
                      prov.mode == 'cheapest',
                      prov.mode == 'fast_with_wait',
                      prov.mode == 'fast_no_wait',
                    ],
                    borderRadius: BorderRadius.circular(8),
                    selectedColor: Colors.white,
                    fillColor: Theme.of(context).primaryColor,
                    onPressed: (i) {
                      const modes = [
                        'cheapest',
                        'fast_with_wait',
                        'fast_no_wait'
                      ];
                      prov.setMode(modes[i]);
                    },
                    children: const [
                      Padding(
                          padding: EdgeInsets.symmetric(horizontal: 8),
                          child: Text('最便宜')),
                      Padding(
                          padding: EdgeInsets.symmetric(horizontal: 8),
                          child: Text('含等车快')),
                      Padding(
                          padding: EdgeInsets.symmetric(horizontal: 8),
                          child: Text('无等车快')),
                    ],
                  ),
                  const Spacer(),
                  SizedBox(
                    width: double.infinity,
                    height: 48,
                    child: ElevatedButton(
                      onPressed: prov.fetchRoute,
                      child: const Text('查询并高亮'),
                    ),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }
}
