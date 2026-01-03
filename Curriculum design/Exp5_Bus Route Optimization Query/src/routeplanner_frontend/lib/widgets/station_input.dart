import 'package:flutter/material.dart';

class StationInput extends StatelessWidget {
  final List<String> stations;
  final ValueChanged<String> onSelected;

  const StationInput({
    super.key,
    required this.stations,
    required this.onSelected,
  });

  @override
  Widget build(BuildContext context) {
    return Autocomplete<String>(
      optionsBuilder: (text) {
        return stations.where((s) => s.contains(text.text)).toList();
      },
      onSelected: onSelected,
      fieldViewBuilder: (ctx, ctrl, fn, onEd) {
        return TextField(
          controller: ctrl,
          focusNode: fn,
          onSubmitted: (_) => onEd(),
          decoration: InputDecoration(
            filled: true,
            fillColor: Colors.white,
            contentPadding: const EdgeInsets.symmetric(horizontal: 12),
            border: OutlineInputBorder(
              borderRadius: BorderRadius.circular(8),
              borderSide: BorderSide.none,
            ),
          ),
        );
      },
    );
  }
}
