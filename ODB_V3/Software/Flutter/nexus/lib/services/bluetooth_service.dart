import 'dart:async';
import 'package:flutter/foundation.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:nexus/services/data_service.dart';

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

    addLog('Démarrage du scan Bluetooth');

    scanResults.clear();
    isScanning = true;
    notifyListeners();

    FlutterBluePlus.scanResults.listen((results) {
      scanResults = results;
      if (results.isNotEmpty) {
        addLog('${results.length} appareil(s) détecté(s)');
      }
      notifyListeners();
    });

    await FlutterBluePlus.startScan(timeout: timeout);

    FlutterBluePlus.isScanning
        .where((v) => v == false)
        .first
        .then((_) {
      isScanning = false;
      addLog('Scan Bluetooth terminé');
      notifyListeners();
    });
  }

  Future<void> stopScan() async {
    if (!isScanning) return;

    await FlutterBluePlus.stopScan();
    isScanning = false;
    scanResults.clear();
    addLog('Scan Bluetooth arrêté manuellement');
    notifyListeners();
  }

  // ---------- CONNECT ----------
  Future<void> connect(BluetoothDevice device, BluetoothDataService dataService) async {
    final name =
        device.platformName.isNotEmpty ? device.platformName : device.remoteId.str;

    try {
      addLog('Connexion à $name');

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
        addLog('État connexion: $state');

        if (state == BluetoothConnectionState.disconnected) {
          connectedDevice = null;
          addLog('Appareil déconnecté');
          notifyListeners();
        }
      });

      await discoverServices(dataService);

      addLog('Connexion établie avec $name');
    } catch (e) {
      addLog('Erreur de connexion: $e');
      debugPrint('Erreur connexion: $e');
      rethrow;
    }
  }

  Future<void> disconnect() async {
    if (connectedDevice == null) {
      addLog('Aucun appareil à déconnecter');
      return;
    }

    addLog('Déconnexion en cours');

    await connectionSubscription?.cancel();
    connectionSubscription = null;

    for (final sub in notifySubscriptions.values) {
      await sub.cancel();
    }
    notifySubscriptions.clear();

    try {
      await connectedDevice!.disconnect();
      addLog('Déconnexion réussie');
    } catch (e) {
      addLog('Erreur déconnexion: $e');
      debugPrint('Erreur disconnect: $e');
    } finally {
      connectedDevice = null;
      notifyListeners();
    }
  }

  // ---------- SERVICES ----------
  Future<void> discoverServices(BluetoothDataService dataService) async {
    if (connectedDevice == null) return;

    addLog('Découverte des services');

    final services = await connectedDevice!.discoverServices();
    addLog('${services.length} service(s) trouvé(s)');

    for (final service in services) {
      for (final c in service.characteristics) {
        if (c.properties.notify) {
          addLog('Notification activée: ${c.uuid}');
          await enableNotifications(c, dataService);
        }
      }
    }
  }

  Future<void> enableNotifications(
    BluetoothCharacteristic c,
    BluetoothDataService dataService,
  ) async {
    try {
      await c.setNotifyValue(true);

      var sub = c.lastValueStream.listen((data) {
        final text = String.fromCharCodes(data);
        addLog("Message reçu: $text");

        // Appel du parser pour extraire et stocker les données
        dataService.parseMessage(text);
      });

      notifySubscriptions[c.uuid] = sub;
    } catch (e) {
      addLog("Erreur enableNotifications: $e");
    }
  }

  Future<void> sendMessage(
      BluetoothCharacteristic c, String message) async {
    try {
      await c.write(message.codeUnits, withoutResponse: false);
      addLog('TX: $message');
    } catch (e) {
      addLog('Erreur envoi message: $e');
    }
  }

  // ---------- LOG HELPER ----------
  void addLog(String message) {
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
