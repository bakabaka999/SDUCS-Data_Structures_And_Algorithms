import 'dart:ui';
import 'package:flutter/material.dart';

class Station {
  final String name;
  final Offset pos;
  final bool isTransfer;

  Station({
    required this.name,
    required this.pos,
    this.isTransfer = false,
  });
}

class BusLine {
  final String name;
  final List<Station> stations;
  final Color color;
  final int price;
  final int interval;
  final double speed;

  BusLine({
    required this.name,
    required this.stations,
    required this.color,
    required this.price,
    required this.interval,
    required this.speed,
  });

  factory BusLine.fromJson(Map<String, dynamic> json, int order) {
    final stations = (json['stations'] as List).map((s) {
      return Station(
        name: s['name'],
        pos: Offset(
          (s['x'] as num).toDouble(),
          (s['y'] as num).toDouble(),
        ),
        isTransfer: false,
      );
    }).toList();

    return BusLine(
      name: json['name'],
      stations: stations,
      color: Colors.primaries[order % Colors.primaries.length],
      price: json['price'],
      interval: json['interval'],
      speed: (json['speed'] as num).toDouble(),
    );
  }



  Map<String, dynamic> toJson() => {
        'name': name,
        'stations': stations.map((s) => s.name).toList(),
        'price': price,
        'interval': interval,
        'speed': speed,
      };
}
