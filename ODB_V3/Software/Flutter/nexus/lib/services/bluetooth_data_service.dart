import 'package:flutter/foundation.dart';
import 'package:nexus/services/bluetooth_service.dart';

enum SensorState { unknown, ok, error }
enum RadioState { disconnected, connecting, connected }

class BluetoothDataService with ChangeNotifier {
  final BluetoothServiceManager btService;
  BluetoothDataService(this.btService);

  // ---------- Variables extraites ----------
  String odbState = '';
  double batteryVoltage = 0.0;
  double batteryVoltageMax = 0.0;
  List<bool> pyros = List.filled(4, false);
  double imuAccX = 0.0, imuAccY = 0.0, imuAccZ = 0.0;
  double imuGyroX = 0.0, imuGyroY = 0.0, imuGyroZ = 0.0;
  double imuMagX = 0.0, imuMagY = 0.0, imuMagZ = 0.0;
  double accHighGX = 0.0, accHighGY = 0.0, accHighGZ = 0.0;
  double sdUsed = 0.0, sdMax = 0.0;
  double gpsLat = 0.0, gpsLon = 0.0, gpsAlt = 0.0;
  double barometerAlt = 0.0;

  // ---------- États des capteurs ----------
  SensorState odbSensorState = SensorState.unknown;
  SensorState batterySensorState = SensorState.unknown;
  SensorState imuSensorState = SensorState.unknown;
  SensorState accHighGSensorState = SensorState.unknown;
  SensorState sdSensorState = SensorState.unknown;
  SensorState gpsSensorState = SensorState.unknown;
  SensorState barometerSensorState = SensorState.unknown;
  RadioState radioState = RadioState.disconnected;


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
          case 'odb':
            odbState = value;
            odbSensorState = value.isNotEmpty ? SensorState.ok : SensorState.error;
            break;

          case 'bat':
            batteryVoltage = double.tryParse(value) ?? batteryVoltage;
            batterySensorState = batteryVoltage > 0 ? SensorState.ok : SensorState.error;
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

          case 'baro_alt':
            barometerAlt = double.tryParse(value) ?? barometerAlt;
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
            btService.addLog('Clé inconnue: $key -> $value');
        }
      }

      notifyListeners();
    } catch (e) {
      btService.addLog('Erreur parseMessage: $e');
    }
  }
}
