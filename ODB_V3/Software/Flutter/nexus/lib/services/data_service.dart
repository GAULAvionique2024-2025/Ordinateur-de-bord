import 'package:flutter/foundation.dart';
import 'dart:math';
import 'package:nexus/services/bluetooth_service.dart';
import 'package:nexus/services/console_service.dart';

enum SensorState { unknown, ok, error }
enum RadioState { disconnected, connecting, connected }

class DataServiceManager with ChangeNotifier {
  final BluetoothServiceManager btService;
  DataServiceManager(this.btService);

  // ---------- Variables extraites ----------
  String odbState = '';
  double batteryVoltage = 0.0;
  double batteryVoltageMax = 0.0;
  bool goodPowerState = false;
  double temperature = 0.0;
  List<bool> pyros = List.filled(4, false);
  bool pyrosArmed = false;
  double imuAccX = 0.0, imuAccY = 0.0, imuAccZ = 0.0;
  double imuGyroX = 0.0, imuGyroY = 0.0, imuGyroZ = 0.0;
  double imuMagX = 0.0, imuMagY = 0.0, imuMagZ = 0.0;
  double accHighGX = 0.0, accHighGY = 0.0, accHighGZ = 0.0;
  double sdUsed = 0.0, sdMax = 0.0;
  double gpsLat = 0.0, gpsLon = 0.0, gpsAlt = 0.0;
  int gpsSatellites = 0;
  bool gpsFix = false;
  double barometerPressure = 0.0;

  // ---------- États des capteurs ----------
  SensorState batterySensorState = SensorState.unknown;
  SensorState temperatureSensorState = SensorState.unknown;
  SensorState imuSensorState = SensorState.unknown;
  SensorState accHighGSensorState = SensorState.unknown;
  SensorState sdSensorState = SensorState.unknown;
  SensorState gpsSensorState = SensorState.unknown;
  SensorState barometerSensorState = SensorState.unknown;
  RadioState radioState = RadioState.disconnected;

  // ---------- Getter / valeurs derivées ----------
  double get batteryPercent => batteryVoltageMax > 0
      ? (batteryVoltage / batteryVoltageMax * 100).clamp(0, 100)
      : 0.0;
  String get temperatureDisplay => temperature != 0.0 ? '${temperature.toStringAsFixed(1)}°C' : '—';
  double get sdUsagePercent =>
      sdMax > 0 ? (sdUsed / sdMax * 100).clamp(0, 100) : 0.0;
  double pressureToAltitude(double pressureHpa, [double seaLevelHpa = 1013.25]) {
    if (pressureHpa <= 0 || seaLevelHpa <= 0) return 0.0;
    final ratio = pressureHpa / seaLevelHpa;
    final alt = 44330.0 * (1 - pow(ratio, 1 / 5.255));
    return alt.isFinite ? alt.toDouble() : 0.0;
  }
  double get barometerAlt => pressureToAltitude(barometerPressure);

  String get altitudeDisplay {
    if (!hasConnection) return '—';
    if (barometerSensorState == SensorState.ok) {
      return '${barometerAlt.toStringAsFixed(0)} m';
    }
    if (gpsSensorState == SensorState.ok) {
      return '${gpsAlt.toStringAsFixed(0)} m';
    }
    return '—';
  }
  int get pyrosActiveCount => pyros.where((p) => p).length;
  String get pyrosSummary => pyros.map((p) => p ? '1' : '0').join(',');
  String get missionStatus => odbState.isNotEmpty ? odbState : '—';
  
  bool get odbSensorState => (
      temperatureSensorState == SensorState.ok &&
      imuSensorState == SensorState.ok &&
      accHighGSensorState == SensorState.ok &&
      sdSensorState == SensorState.ok &&
      gpsSensorState == SensorState.ok &&
      gpsFix == true &&
      barometerSensorState == SensorState.ok &&
      goodPowerState == true &&
      pyrosActiveCount >= 0
    );
  bool get missionReady => odbSensorState && radioState == RadioState.connected;
  bool get hasConnection => btService.connectedDevice != null;

  // ---------- PARSER ----------
  void parseMessage(String message) {
    try {
      final entries = message.split(';');
      for (final entry in entries) {
        if (entry.isEmpty) continue;
        final parts = entry.split(':');
        if (parts.length != 2) continue;

        final key = parts[0].trim().toLowerCase();
        final value = parts[1].trim();

        switch (key) {
          case 'temp':
            temperature = double.tryParse(value) ?? temperature;
            temperatureSensorState = temperature >= -30 && temperature <= 80 ? SensorState.ok : SensorState.error;
            break;
          case 'odb':
            odbState = value;
            break;

          case 'bat':
            batteryVoltage = double.tryParse(value) ?? batteryVoltage;
            batterySensorState = batteryVoltage > 0 ? SensorState.ok : SensorState.error;
            goodPowerState = batteryVoltage >= 5.06 ? true : false;
            break;

          case 'bat_max':
            batteryVoltageMax = double.tryParse(value) ?? batteryVoltageMax;
            break;

          case 'pyro1':
            pyros[0] = value == '1';
            break;
          case 'pyro2':
            pyros[1] = value == '1';
            break;
          case 'pyro3':
            pyros[2] = value == '1';
            break;
          case 'pyro4':
            pyros[3] = value == '1';
            break;
          case 'armed':
            pyrosArmed = value == '1';
            break;

          case 'accx':
            imuAccX = double.tryParse(value) ?? imuAccX;
            imuSensorState = SensorState.ok;
            break;
          case 'accy':
            imuAccY = double.tryParse(value) ?? imuAccY;
            break;
          case 'accz':
            imuAccZ = double.tryParse(value) ?? imuAccZ;
            break;
          case 'gyrox':
            imuGyroX = double.tryParse(value) ?? imuGyroX;
            break;
          case 'gyroy':
            imuGyroY = double.tryParse(value) ?? imuGyroY;
            break;
          case 'gyroz':
            imuGyroZ = double.tryParse(value) ?? imuGyroZ;
            break;
          case 'magx':
            imuMagX = double.tryParse(value) ?? imuMagX;
            break;
          case 'magy':
            imuMagY = double.tryParse(value) ?? imuMagY;
            break;
          case 'magz':
            imuMagZ = double.tryParse(value) ?? imuMagZ;
            break;

          case 'acc_hg_x':
            accHighGX = double.tryParse(value) ?? accHighGX;
            accHighGSensorState = SensorState.ok;
            break;
          case 'acc_hg_y':
            accHighGY = double.tryParse(value) ?? accHighGY;
            break;
          case 'acc_hg_z':
            accHighGZ = double.tryParse(value) ?? accHighGZ;
            break;

          case 'sd_used':
            sdUsed = double.tryParse(value) ?? sdUsed;
            sdSensorState = SensorState.ok;
            break;
          case 'sd_max':
            sdMax = double.tryParse(value) ?? sdMax;
            break;

          case 'gps_lat':
            gpsLat = double.tryParse(value) ?? gpsLat;
            gpsSensorState = SensorState.ok;
            break;
          case 'gps_lon':
            gpsLon = double.tryParse(value) ?? gpsLon;
            break;
          case 'gps_alt':
            gpsAlt = double.tryParse(value) ?? gpsAlt;
            break;
          case 'gps_sats':
            gpsSatellites = int.tryParse(value) ?? gpsSatellites;
            break;
          case 'fix':
            gpsFix = value == '1' ? true : false;
            break;

          case 'baro_pressure':
            barometerPressure = double.tryParse(value) ?? barometerPressure;
            barometerSensorState = SensorState.ok;
            break;

          case 'radio':
            radioState = value.toLowerCase() == 'connected'
                ? RadioState.connected
                : value.toLowerCase() == 'connecting'
                    ? RadioState.connecting
                    : RadioState.disconnected;
            break;

          default:
            ConsoleService().log('Clé inconnue: $key -> $value');
        }
      }

      notifyListeners();
    } catch (e) {
      ConsoleService().log('Erreur parseMessage: $e');
    }
  }
}
