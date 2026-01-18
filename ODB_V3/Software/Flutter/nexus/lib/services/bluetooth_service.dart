import 'dart:async';
import 'package:flutter/foundation.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:nexus/services/data_service.dart';
import 'package:nexus/services/console_service.dart';

class BluetoothServiceManager with ChangeNotifier {
  BluetoothServiceManager() {
    ConsoleService().addListener(_onConsoleChanged);
  }
  // ---------- STATE ----------
  List<ScanResult> scanResults = [];
  bool isScanning = false;

  BluetoothDevice? connectedDevice;
  StreamSubscription<BluetoothConnectionState>? connectionSubscription;
  final Map<Guid, StreamSubscription<List<int>>> notifySubscriptions = {};
  BluetoothCharacteristic? _writeCharacteristic;

  // ---------- RSSI ----------
  /// stocke le dernier RSSI connu (-999 = inconnu)
  int rssi = -999;
  String get rssiDisplay => rssi == -999 ? '—' : '$rssi dBm';

  /// relance un scan court pour get le rssi
  Future<void> refreshScan({Duration timeout = const Duration(seconds: 5)}) async {
    if (isScanning) await stopScan();
    await startScan(timeout: timeout);
  }

  // ---------- LOGS ----------
  void _onConsoleChanged() => notifyListeners();
  List<String> get logs => ConsoleService().logs;

  // ---------- SCAN ----------
  Future<void> startScan({Duration timeout = const Duration(seconds: 15)}) async {
    if (isScanning) return;

    ConsoleService().clear();
    ConsoleService().log('Démarrage du scan Bluetooth');

    scanResults.clear();
    isScanning = true;
    notifyListeners();

    FlutterBluePlus.scanResults.listen((results) {
      scanResults = results;
      // Met à jour le RSSI en prenant la valeur du périphérique connecté (si présent)
      if (results.isNotEmpty) {
        ConsoleService().log('${results.length} appareil(s) détecté(s)');
        final match = connectedDevice != null
            ? results.firstWhere((r) => r.device.remoteId == connectedDevice!.remoteId, orElse: () => results.first)
            : results.first;
        rssi = match.rssi;
      } else {
        rssi = -999;
      }
      notifyListeners();
    });

    await FlutterBluePlus.startScan(timeout: timeout);

    FlutterBluePlus.isScanning
        .where((v) => v == false)
        .first
        .then((_) {
      isScanning = false;
      ConsoleService().log('Scan Bluetooth terminé');
      notifyListeners();
    });
  }

  Future<void> stopScan() async {
    if (!isScanning) return;

    await FlutterBluePlus.stopScan();
    isScanning = false;
    ConsoleService().log('Scan Bluetooth arrêté manuellement');
    notifyListeners();
  }

  // ---------- CONNECT ----------
  Future<void> connect(BluetoothDevice device, DataServiceManager dataService) async {
    final name = device.platformName.isNotEmpty ? device.platformName : device.remoteId.str;

    try {
      ConsoleService().log('Connexion à $name');

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
        ConsoleService().log('État connexion: $state');

        if (state == BluetoothConnectionState.disconnected) {
          connectedDevice = null;
          rssi = -999;
          ConsoleService().log('Appareil déconnecté');
          notifyListeners();
        }
      });

      await discoverServices(dataService);

      ConsoleService().log('Connexion établie avec $name');
    } catch (e) {
      ConsoleService().log('Erreur de connexion: $e');
      debugPrint('Erreur connexion: $e');
      rethrow;
    }
  }

  Future<void> disconnect() async {
    if (connectedDevice == null) {
      ConsoleService().log('Aucun appareil à déconnecter');
      return;
    }

    ConsoleService().log('Déconnexion en cours');

    await connectionSubscription?.cancel();
    connectionSubscription = null;

    for (final sub in notifySubscriptions.values) {
      await sub.cancel();
    }
    notifySubscriptions.clear();

    try {
      await connectedDevice!.disconnect();
      ConsoleService().log('Déconnexion réussie');
    } catch (e) {
      ConsoleService().log('Erreur déconnexion: $e');
      debugPrint('Erreur disconnect: $e');
    } finally {
      connectedDevice = null;
      notifyListeners();
    }
  }

  // ---------- SERVICES ----------
  Future<void> discoverServices(DataServiceManager dataService) async {
    if (connectedDevice == null) return;

    ConsoleService().log('Découverte des services');

    final services = await connectedDevice!.discoverServices();
    ConsoleService().log('${services.length} service(s) trouvé(s)');

    for (final service in services) {
      for (final c in service.characteristics) {
        if (c.properties.notify) {
          ConsoleService().log('Notification activée: ${c.uuid}');
          await enableNotifications(c, dataService);
        }
      
        // Enregistrer la première caractéristique d'écriture disponible
        if (_writeCharacteristic == null && (c.properties.write || c.properties.writeWithoutResponse)) {
          _writeCharacteristic = c;
          ConsoleService().log('Caractéristique écriture sélectionnée: ${c.uuid}');
        }
      }
    }
  }

  Future<void> enableNotifications(BluetoothCharacteristic c, DataServiceManager dataService,
  ) async {
    try {
      await c.setNotifyValue(true);

      var sub = c.lastValueStream.listen((data) {
        final text = String.fromCharCodes(data);
        ConsoleService().log("Message reçu: $text");

        // Appel du parser pour extraire et stocker les données
        dataService.parseMessage(text);
      });

      notifySubscriptions[c.uuid] = sub;
    } catch (e) {
      ConsoleService().log("Erreur enableNotifications: $e");
    }
  }

  Future<void> send(String message, {BluetoothCharacteristic? characteristic}) async {
    if (connectedDevice == null) {
      ConsoleService().log('Aucun appareil connecté');
      return;
    }
    final c = characteristic ?? _writeCharacteristic;
    if (c == null) {
      ConsoleService().log('Aucune caractéristique d\'écriture disponible');
      return;
    }
    try {
      await c.write(message.codeUnits, withoutResponse: false);
      ConsoleService().log('TX: $message');
    } catch (e) {
      ConsoleService().log('Erreur envoi message: $e');
    }
  }

  @override
  void dispose() {
    try {
      ConsoleService().removeListener(_onConsoleChanged);
    } catch (_) {}

    connectionSubscription?.cancel();
    for (final sub in notifySubscriptions.values) {
      sub.cancel();
    }
    notifySubscriptions.clear();
    super.dispose();
  }
}
