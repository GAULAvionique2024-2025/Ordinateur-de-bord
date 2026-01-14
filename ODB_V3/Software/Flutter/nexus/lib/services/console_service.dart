import 'package:flutter/foundation.dart';

class ConsoleService with ChangeNotifier {
  ConsoleService._internal();
  static final ConsoleService _instance = ConsoleService._internal();
  factory ConsoleService() => _instance;

  final List<String> _logs = [];
  List<String> get logs => List.unmodifiable(_logs);

  // ---------- LOG ----------
  void log(String message) {
    final time = DateTime.now().toIso8601String().substring(11, 19);
    _logs.insert(0, '[$time] $message');
    notifyListeners();
  }

  void clear() {
    _logs.clear();
    notifyListeners();
  }
}