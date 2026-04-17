import 'package:flutter/foundation.dart';
import 'dart:math';
import 'package:nexus/services/bluetooth_service.dart';
import 'package:nexus/services/console_service.dart';

enum SensorState { unknown, ok, error }
enum RadioState { disconnected, connecting, connected }

class DataServiceManager with ChangeNotifier {
  final BluetoothServiceManager btService;
  DataServiceManager(this.btService);
  bool _isDisposed = false;

  bool get isDisposed => _isDisposed;

  void _safeNotifyListeners() {
    if (!_isDisposed) {
      notifyListeners();
    }
  }

  // ---------- Variables extraites ----------
  int timeBootMs = 0;
  int systemStates = 0;
  int missionState = -1;
  String odbState = '';
  double batteryVoltage = 0.0;
  double batteryVoltageMax = 0.0;
  bool goodPowerState = false;
  double temperature = 0.0;
  List<bool> pyros = List.filled(4, false);
  bool pyrosArmed = false;
  double roll = 0.0, pitch = 0.0, yaw = 0.0;
  double imuAccX = 0.0, imuAccY = 0.0, imuAccZ = 0.0;
  double imuGyroX = 0.0, imuGyroY = 0.0, imuGyroZ = 0.0;
  double imuMagX = 0.0, imuMagY = 0.0, imuMagZ = 0.0;
  double accHighGX = 0.0, accHighGY = 0.0, accHighGZ = 0.0;
  double sdUsed = 0.0, sdMax = 0.0;
  double gpsLat = 0.0, gpsLon = 0.0, gpsAlt = 0.0;
  double gpsVelocity = 0.0, gpsCourse = 0.0;
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
  String get missionStateDisplay {
    switch (missionState) {
      case 0:
        return 'Prévol';
      case 1:
        return 'Prêt';
      case 2:
        return 'En vol';
      case 3:
        return 'Post-vol';
      case 4:
        return 'Erreur';
      default:
        return '—';
    }
  }
  String get systemStateDisplay => systemStates > 0 ? 'Flags $systemStates' : '—';
  String get timeBootDisplay => timeBootMs > 0 ? '$timeBootMs ms' : '—';
  String get attitudeDisplay => hasConnection
      ? 'R ${roll.toStringAsFixed(1)}°  P ${pitch.toStringAsFixed(1)}°  Y ${yaw.toStringAsFixed(1)}°'
      : '—';
  String get gpsVelocityDisplay => gpsSensorState == SensorState.ok
      ? '${gpsVelocity.toStringAsFixed(1)} m/s'
      : '—';
  String get gpsCourseDisplay => gpsSensorState == SensorState.ok
      ? '${gpsCourse.toStringAsFixed(0)}°'
      : '—';
  String get missionStatus {
    if (missionStateDisplay != '—') return missionStateDisplay;
    if (odbState.isNotEmpty) return odbState;
    return systemStateDisplay;
  }
  
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
    if (_isDisposed) return;

    try {
      final normalized = message.trim();
      final isTelemetryFrame = normalized.startsWith('DATA,');
      final entries = <String, String>{};

      if (isTelemetryFrame) {
        final payload = normalized.substring(5);
        for (final entry in payload.split(',')) {
          if (entry.isEmpty) continue;
          final parts = entry.split('=');
          if (parts.length != 2) continue;
          entries[parts[0].trim().toLowerCase()] = parts[1].trim();
        }
      } else {
        for (final entry in normalized.split(';')) {
          if (entry.isEmpty) continue;
          final parts = entry.split(':');
          if (parts.length != 2) continue;
          entries[parts[0].trim().toLowerCase()] = parts[1].trim();
        }
      }

      for (final entry in entries.entries) {
        final key = entry.key;
        final value = entry.value;

        switch (key) {
          case 'time_boot_ms':
            timeBootMs = int.tryParse(value) ?? timeBootMs;
            break;
          case 'system_states':
            systemStates = int.tryParse(value) ?? systemStates;
            break;
          case 'mission':
          case 'mission_state':
            missionState = int.tryParse(value) ?? missionState;
            break;
          case 'temp':
          case 'temp_celsius':
            temperature = double.tryParse(value) ?? temperature;
            temperatureSensorState = temperature >= -30 && temperature <= 80 ? SensorState.ok : SensorState.error;
            break;
          case 'odb':
            odbState = value;
            break;

          case 'bat':
            batteryVoltage = double.tryParse(value) ?? batteryVoltage;
            batterySensorState = batteryVoltage > 0 ? SensorState.ok : SensorState.error;
            goodPowerState = batteryVoltage >= 5.06;
            break;
          case 'battery_mv':
            batteryVoltage = (int.tryParse(value) ?? (batteryVoltage * 1000).round()) / 1000.0;
            batterySensorState = batteryVoltage > 0 ? SensorState.ok : SensorState.error;
            goodPowerState = batteryVoltage >= 5.06;
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

          case 'roll':
            roll = double.tryParse(value) ?? roll;
            imuSensorState = SensorState.ok;
            break;
          case 'pitch':
            pitch = double.tryParse(value) ?? pitch;
            imuSensorState = SensorState.ok;
            break;
          case 'yaw':
            yaw = double.tryParse(value) ?? yaw;
            imuSensorState = SensorState.ok;
            break;
          case 'accx':
          case 'imu_acc_x':
            imuAccX = double.tryParse(value) ?? imuAccX;
            imuSensorState = SensorState.ok;
            break;
          case 'accy':
          case 'imu_acc_y':
            imuAccY = double.tryParse(value) ?? imuAccY;
            break;
          case 'accz':
          case 'imu_acc_z':
            imuAccZ = double.tryParse(value) ?? imuAccZ;
            break;
          case 'gyrox':
          case 'imu_gyro_x':
            imuGyroX = double.tryParse(value) ?? imuGyroX;
            break;
          case 'gyroy':
          case 'imu_gyro_y':
            imuGyroY = double.tryParse(value) ?? imuGyroY;
            break;
          case 'gyroz':
          case 'imu_gyro_z':
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
          case 'highg_acc_x':
            accHighGX = double.tryParse(value) ?? accHighGX;
            accHighGSensorState = SensorState.ok;
            break;
          case 'acc_hg_y':
          case 'highg_acc_y':
            accHighGY = double.tryParse(value) ?? accHighGY;
            break;
          case 'acc_hg_z':
          case 'highg_acc_z':
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
          case 'lat':
            gpsLat = (int.tryParse(value) ?? (gpsLat * 10000000).round()) / 10000000.0;
            gpsSensorState = SensorState.ok;
            break;
          case 'gps_lon':
            gpsLon = double.tryParse(value) ?? gpsLon;
            break;
          case 'lon':
            gpsLon = (int.tryParse(value) ?? (gpsLon * 10000000).round()) / 10000000.0;
            gpsSensorState = SensorState.ok;
            break;
          case 'gps_alt':
            gpsAlt = isTelemetryFrame
                ? (int.tryParse(value) ?? (gpsAlt * 1000).round()) / 1000.0
                : double.tryParse(value) ?? gpsAlt;
            break;
          case 'gps_alt_mm':
            gpsAlt = (int.tryParse(value) ?? (gpsAlt * 1000).round()) / 1000.0;
            break;
          case 'gps_sats':
          case 'satellites_nb':
            gpsSatellites = int.tryParse(value) ?? gpsSatellites;
            gpsSensorState = SensorState.ok;
            break;
          case 'fix':
          case 'gps_fix':
            gpsFix = value == '1' || value.toLowerCase() == 'true';
            gpsSensorState = gpsFix ? SensorState.ok : SensorState.error;
            break;
          case 'vel':
            gpsVelocity = (int.tryParse(value) ?? (gpsVelocity * 100).round()) / 100.0;
            gpsSensorState = SensorState.ok;
            break;
          case 'cog':
            gpsCourse = (int.tryParse(value) ?? (gpsCourse * 100).round()) / 100.0;
            gpsSensorState = SensorState.ok;
            break;

          case 'baro_pressure':
          case 'pressure_hpa':
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

      _safeNotifyListeners();
    } catch (e) {
      if (_isDisposed) return;
      ConsoleService().log('Erreur parseMessage: $e');
    }
  }

  @override
  void dispose() {
    _isDisposed = true;
    super.dispose();
  }
}
