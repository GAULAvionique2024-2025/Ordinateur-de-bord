import 'dart:async';
import 'package:flutter/foundation.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

class BluetoothServiceManager with ChangeNotifier {
  // ---------- STATE ----------
  List<ScanResult> scanResults = [];
  bool isScanning = false;

  BluetoothDevice? connectedDevice;
  StreamSubscription<BluetoothConnectionState>? connectionSubscription;
  final Map<Guid, StreamSubscription<List<int>>> notifySubscriptions = {};

  // ---------- LOGS ----------
  final List<String> _logs = [];
  List<String> get logs => List.unmodifiable(_logs);

  // ---------- SCAN ----------
  Future<void> startScan({Duration timeout = const Duration(seconds: 15)}) async {
    if (isScanning) return;

    _addLog('Démarrage du scan Bluetooth');

    scanResults.clear();
    isScanning = true;
    notifyListeners();

    FlutterBluePlus.scanResults.listen((results) {
      scanResults = results;
      if (results.isNotEmpty) {
        _addLog('${results.length} appareil(s) détecté(s)');
      }
      notifyListeners();
    });

    await FlutterBluePlus.startScan(timeout: timeout);

    FlutterBluePlus.isScanning
        .where((v) => v == false)
        .first
        .then((_) {
      isScanning = false;
      _addLog('Scan Bluetooth terminé');
      notifyListeners();
    });
  }

  Future<void> stopScan() async {
    if (!isScanning) return;

    await FlutterBluePlus.stopScan();
    isScanning = false;
    scanResults.clear();
    _addLog('Scan Bluetooth arrêté manuellement');
    notifyListeners();
  }

  // ---------- CONNECT ----------
  Future<void> connect(BluetoothDevice device) async {
    final name =
        device.platformName.isNotEmpty ? device.platformName : device.remoteId.str;

    try {
      _addLog('Connexion à $name');

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
        _addLog('État connexion: $state');

        if (state == BluetoothConnectionState.disconnected) {
          connectedDevice = null;
          _addLog('Appareil déconnecté');
          notifyListeners();
        }
      });

      await discoverServices();

      _addLog('Connexion établie avec $name');
    } catch (e) {
      _addLog('Erreur de connexion: $e');
      debugPrint('Erreur connexion: $e');
      rethrow;
    }
  }

  Future<void> disconnect() async {
    if (connectedDevice == null) {
      _addLog('Aucun appareil à déconnecter');
      return;
    }

    _addLog('Déconnexion en cours');

    await connectionSubscription?.cancel();
    connectionSubscription = null;

    for (final sub in notifySubscriptions.values) {
      await sub.cancel();
    }
    notifySubscriptions.clear();

    try {
      await connectedDevice!.disconnect();
      _addLog('Déconnexion réussie');
    } catch (e) {
      _addLog('Erreur déconnexion: $e');
      debugPrint('Erreur disconnect: $e');
    } finally {
      connectedDevice = null;
      notifyListeners();
    }
  }

  // ---------- SERVICES ----------
  Future<void> discoverServices() async {
    if (connectedDevice == null) return;

    _addLog('Découverte des services');

    final services = await connectedDevice!.discoverServices();
    _addLog('${services.length} service(s) trouvé(s)');

    for (final service in services) {
      for (final c in service.characteristics) {
        if (c.properties.notify) {
          _addLog('Notification activée: ${c.uuid}');
          await enableNotifications(c, (data) {
            final text = String.fromCharCodes(data);
            _addLog('RX: $text');
          });
        }
      }
    }
  }

  Future<void> enableNotifications(
      BluetoothCharacteristic c, void Function(List<int>) onData) async {
    try {
      await c.setNotifyValue(true);
      notifySubscriptions[c.uuid] = c.lastValueStream.listen(onData);
    } catch (e) {
      _addLog('Erreur notification ${c.uuid}: $e');
    }
  }

  Future<void> sendMessage(
      BluetoothCharacteristic c, String message) async {
    try {
      await c.write(message.codeUnits, withoutResponse: false);
      _addLog('TX: $message');
    } catch (e) {
      _addLog('Erreur envoi message: $e');
    }
  }

  // ---------- LOG HELPER ----------
  void _addLog(String message) {
    final time = DateTime.now().toIso8601String().substring(11, 19);
    _logs.insert(0, '[$time] $message');
    notifyListeners();
  }

  @override
  void dispose() {
    connectionSubscription?.cancel();
    for (final sub in notifySubscriptions.values) {
      sub.cancel();
    }
    notifySubscriptions.clear();
    super.dispose();
  }
}
