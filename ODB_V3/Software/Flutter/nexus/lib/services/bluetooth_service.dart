import 'dart:async';
import 'package:flutter/foundation.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

class BluetoothServiceManager with ChangeNotifier {
  // STATE
  List<ScanResult> scanResults = [];
  bool isScanning = false;

  BluetoothDevice? connectedDevice;
  StreamSubscription<BluetoothConnectionState>? connectionSubscription;
  final Map<Guid, StreamSubscription<List<int>>> notifySubscriptions = {};

  // ---------- SCAN ----------
  Future<void> startScan({Duration timeout = const Duration(seconds: 15)}) async {
    if (isScanning) return;

    scanResults.clear();
    isScanning = true;
    notifyListeners();

    // Listen to scan results stream
    FlutterBluePlus.scanResults.listen((results) {
      scanResults = results;
      notifyListeners();
    });

    // Start scan (flutter_blue_plus handles timeouts)
    await FlutterBluePlus.startScan(timeout: timeout);

    // Stop automatically when scanning ends
    FlutterBluePlus.isScanning.where((val) => val == false).first.then((_) {
      isScanning = false;
      notifyListeners();
    });
  }

  Future<void> stopScan() async {
    if (!isScanning) return;

    await FlutterBluePlus.stopScan();
    isScanning = false;
    scanResults.clear();
    notifyListeners();
  }

  // ---------- CONNECT ----------
  Future<void> connect(BluetoothDevice device) async {
    try {
      await connectionSubscription?.cancel();
      await connectedDevice?.disconnect();
      await stopScan();

      await device.connect(
        license: License.free,
        timeout: const Duration(seconds: 10),
        autoConnect: false,
      );

      connectedDevice = device;
      notifyListeners();

      connectionSubscription = device.connectionState.listen((state) {
        if (state == BluetoothConnectionState.disconnected) {
          connectedDevice = null;
          notifyListeners();
        }
      });

      // Discover services automatically
      await discoverServices();
    } catch (e) {
      debugPrint("Erreur connexion: $e");
      rethrow;
    }
  }

  Future<void> disconnect() async {
    await connectionSubscription?.cancel();
    connectionSubscription = null;

    for (var sub in notifySubscriptions.values) {
      await sub.cancel();
    }
    notifySubscriptions.clear();

    try {
      await connectedDevice?.disconnect();
    } catch (e) {
      debugPrint("Erreur during disconnect: $e");
    } finally {
      connectedDevice = null;
      notifyListeners();
    }
  }

  // ---------- SERVICES ----------
  Future<void> discoverServices() async {
    if (connectedDevice == null) return;
    List<BluetoothService> services = await connectedDevice!.discoverServices();
    for (var service in services) {
      for (var c in service.characteristics) {
        if (c.properties.notify) {
          await enableNotifications(c, (data) {
            String text = String.fromCharCodes(data);
            debugPrint("Message reçu: $text");
          });
        }
      }
    }
  }

  Future<void> enableNotifications(
      BluetoothCharacteristic c, void Function(List<int>) onData) async {
    try {
      await c.setNotifyValue(true);
      var sub = c.lastValueStream.listen(onData);
      notifySubscriptions[c.uuid] = sub;
    } catch (e) {
      debugPrint("Erreur enableNotifications: $e");
    }
  }

  Future<void> sendMessage(BluetoothCharacteristic c, String message) async {
    await c.write(message.codeUnits, withoutResponse: false);
  }

  @override
  void dispose() {
    connectionSubscription?.cancel();
    for (var sub in notifySubscriptions.values) {
      sub.cancel();
    }
    notifySubscriptions.clear();
    super.dispose();
  }
}