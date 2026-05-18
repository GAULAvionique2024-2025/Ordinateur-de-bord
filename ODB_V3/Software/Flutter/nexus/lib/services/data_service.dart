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
  int eventStates = 0;
  int missionState = -1;
  String odbState = '';
  int vinMv = 0;
  double batteryVoltage = 0.0;
  double batteryVoltageMax = 24.0;
  bool goodPowerState = false;
  double temperature = 0.0;
  List<bool> pyros = List.filled(4, false);
  bool pyrosArmed = false;
  double roll = 0.0, pitch = 0.0, yaw = 0.0;
  double imuAccX = 0.0, imuAccY = 0.0, imuAccZ = 0.0;
  double imuAccVertical = 0.0;
  double imuGyroX = 0.0, imuGyroY = 0.0, imuGyroZ = 0.0;
  double imuMagX = 0.0, imuMagY = 0.0, imuMagZ = 0.0;
  double accHighGX = 0.0, accHighGY = 0.0, accHighGZ = 0.0;
  double highGAccVertical = 0.0;
  double sdUsed = 0.0, sdMax = 0.0;
  double gpsLat = 0.0, gpsLon = 0.0, gpsAlt = 0.0;
  double gpsVelocity = 0.0, gpsCourse = 0.0;
  int gpsSatellites = 0;
  bool gpsFix = false;
  double barometerPressure = 0.0;
  double altitudeMslM = 0.0;
  double kalmanAltitudeM = 0.0;
  double kalmanVelocityMS = 0.0;

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
        return 'PREFLIGHT';
      case 1:
        return 'ARMED';
      case 2:
        return 'INFLIGHT';
      case 3:
        return 'POSTFLIGHT';
      default:
        return '—';
    }
  }
  String get systemStateDisplay => systemStates > 0 ? 'Flags $systemStates' : '—';
  String get eventStateDisplay => eventStates > 0 ? 'Events $eventStates' : '—';
  String get vinDisplay {
    if (vinMv > 0) return '$vinMv mV';
    if (batteryVoltage > 0) return '${(batteryVoltage * 1000).round()} mV';
    return '—';
  }
  String get timeBootDisplay => timeBootMs > 0 ? '$timeBootMs ms' : '—';
  
  String get timeBootFormatted {
    if (timeBootMs <= 0) return '00h:00m:00s:00ms';
    final hours = timeBootMs ~/ 3600000;
    final minutes = (timeBootMs % 3600000) ~/ 60000;
    final seconds = (timeBootMs % 60000) ~/ 1000;
    final milliseconds = timeBootMs % 1000;
    return '${hours.toString().padLeft(2, '0')}h:${minutes.toString().padLeft(2, '0')}m:${seconds.toString().padLeft(2, '0')}s:${milliseconds.toString().padLeft(2, '0')}ms';
  }
  
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
  
  // --- IMU / Accelerations ---
  String get imuAccDisplay => hasConnection && imuSensorState == SensorState.ok
      ? 'X: ${imuAccX.toStringAsFixed(2)} | Y: ${imuAccY.toStringAsFixed(2)} | Z: ${imuAccZ.toStringAsFixed(2)} m/s²'
      : '—';
  String get imuGyroDisplay => hasConnection && imuSensorState == SensorState.ok
      ? 'X: ${imuGyroX.toStringAsFixed(2)} | Y: ${imuGyroY.toStringAsFixed(2)} | Z: ${imuGyroZ.toStringAsFixed(2)} °/s'
      : '—';
  String get highGAccDisplay => hasConnection && accHighGSensorState == SensorState.ok
      ? 'X: ${accHighGX.toStringAsFixed(2)} | Y: ${accHighGY.toStringAsFixed(2)} | Z: ${accHighGZ.toStringAsFixed(2)} m/s²'
      : '—';

    String get imuAccVerticalDisplay => hasConnection && imuSensorState == SensorState.ok
      ? '${imuAccVertical.toStringAsFixed(2)} m/s²'
      : '—';

    String get highGAccVerticalDisplay => hasConnection && accHighGSensorState == SensorState.ok
      ? '${highGAccVertical.toStringAsFixed(2)} m/s²'
      : '—';
  
  // --- Barometer ---
  String get pressureDisplay => hasConnection && barometerSensorState == SensorState.ok
      ? '${barometerPressure.toStringAsFixed(2)} hPa'
      : '—';

    String get altitudeMslDisplay => hasConnection
      ? '${altitudeMslM.toStringAsFixed(2)} m'
      : '—';

  String get kalmanAltitudeDisplay => hasConnection
      ? kalmanAltitudeM.toStringAsFixed(2)
      : '—';

  String get kalmanVelocityDisplay => hasConnection
      ? kalmanVelocityMS.toStringAsFixed(2)
      : '—';
  
  // --- GPS ---
  String get gpsLatDisplay => hasConnection && gpsSensorState == SensorState.ok
      ? '${gpsLat.toStringAsFixed(6)}°'
      : '—';
  String get gpsLonDisplay => hasConnection && gpsSensorState == SensorState.ok
      ? '${gpsLon.toStringAsFixed(6)}°'
      : '—';
  String get gpsAltDisplay => hasConnection && gpsSensorState == SensorState.ok
      ? '${gpsAlt.toStringAsFixed(1)} m'
      : '—';
  String get gpsSatellitesDisplay => hasConnection && gpsSensorState == SensorState.ok
      ? '$gpsSatellites satellites'
      : '—';
  String get gpsFixDisplay => hasConnection 
      ? (gpsFix ? '✓ Actif' : '✗ Aucun fix')
      : '—';

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
      final entries = <String, String>{};

      if (normalized.startsWith('DATA,')) {
        final payload = normalized.substring(5);
        for (final entry in payload.split(',')) {
          if (entry.isEmpty) continue;
          final parts = entry.split('=');
          if (parts.length != 2) continue;
          entries[parts[0].trim().toLowerCase()] = parts[1].trim();
        }
      } else {
        for (final match in RegExp(r'([A-Za-z0-9_]+)=([-+]?[0-9]*\.?[0-9]+)').allMatches(normalized)) {
          entries[match.group(1)!.trim().toLowerCase()] = match.group(2)!.trim();
        }

        if (entries.isEmpty) {
          for (final match in RegExp(r'([A-Za-z0-9_]+):\s*([^,;\r\n]+)').allMatches(normalized)) {
            entries[match.group(1)!.trim().toLowerCase()] = match.group(2)!.trim();
          }
        }
      }
      //ConsoleService().log('✅ Parsed ${entries.length} entries');

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
          case 'event_states':
            eventStates = int.tryParse(value) ?? eventStates;
            break;
          case 'mission':
          case 'mission_state':
            missionState = int.tryParse(value) ?? missionState;
            break;
          case 'temp_celsius':
            temperature = double.tryParse(value) ?? temperature;
            if (temperature >= -55 && temperature <= 150) {
              temperatureSensorState = SensorState.ok;
            } else {
              temperatureSensorState = SensorState.error;
            }
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
            vinMv = int.tryParse(value) ?? vinMv;
            batteryVoltage = vinMv > 0
                ? vinMv / 1000.0
                : (int.tryParse(value) ?? (batteryVoltage * 1000).round()) / 1000.0;
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
          case 'imu_acc_x':
            imuAccX = double.tryParse(value) ?? imuAccX;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_acc_vertical':
            imuAccVertical = double.tryParse(value) ?? imuAccVertical;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_acc_y':
            imuAccY = double.tryParse(value) ?? imuAccY;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_acc_z':
            imuAccZ = double.tryParse(value) ?? imuAccZ;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_gyro_x':
            imuGyroX = double.tryParse(value) ?? imuGyroX;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_gyro_y':
            imuGyroY = double.tryParse(value) ?? imuGyroY;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_gyro_z':
            imuGyroZ = double.tryParse(value) ?? imuGyroZ;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_mag_x':
            imuMagX = double.tryParse(value) ?? imuMagX;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_mag_y':
            imuMagY = double.tryParse(value) ?? imuMagY;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_mag_z':
            imuMagZ = double.tryParse(value) ?? imuMagZ;
            imuSensorState = SensorState.ok;
            break;

          case 'highg_acc_x':
            accHighGX = double.tryParse(value) ?? accHighGX;
            accHighGSensorState = SensorState.ok;
            break;
          case 'highg_acc_vertical':
            highGAccVertical = double.tryParse(value) ?? highGAccVertical;
            accHighGSensorState = SensorState.ok;
            break;
          case 'highg_acc_y':
            accHighGY = double.tryParse(value) ?? accHighGY;
            accHighGSensorState = SensorState.ok;
            break;
          case 'highg_acc_z':
            accHighGZ = double.tryParse(value) ?? accHighGZ;
            accHighGSensorState = SensorState.ok;
            break;

          case 'sd_used':
            sdUsed = double.tryParse(value) ?? sdUsed;
            sdSensorState = SensorState.ok;
            break;
          case 'sd_max':
            sdMax = double.tryParse(value) ?? sdMax;
            sdSensorState = SensorState.ok;
            break;

          case 'gps_lat':
            gpsLat = (int.tryParse(value) ?? (gpsLat * 10000000).round()) / 10000000.0;
            gpsSensorState = SensorState.ok;
            break;
          case 'lat':
            gpsLat = (int.tryParse(value) ?? (gpsLat * 10000000).round()) / 10000000.0;
            gpsSensorState = SensorState.ok;
            break;
          case 'gps_lon':
            gpsLon = (int.tryParse(value) ?? (gpsLon * 10000000).round()) / 10000000.0;
            gpsSensorState = SensorState.ok;
            break;
          case 'lon':
            gpsLon = (int.tryParse(value) ?? (gpsLon * 10000000).round()) / 10000000.0;
            gpsSensorState = SensorState.ok;
            break;
          case 'gps_alt':
            gpsAlt = int.tryParse(value)?.toDouble() ?? gpsAlt;
            gpsSensorState = SensorState.ok;
            break;
          case 'satellites_nb':
            gpsSatellites = int.tryParse(value) ?? gpsSatellites;
            gpsSensorState = SensorState.ok;
            break;
          case 'gps_fix':
            gpsFix = value == '1' || value.toLowerCase() == 'true';
            gpsSensorState = gpsFix ? SensorState.ok : SensorState.error;
            break;
          case 'vel':
            gpsVelocity = int.tryParse(value)?.toDouble() ?? gpsVelocity;
            gpsSensorState = SensorState.ok;
            break;
          case 'cog':
            gpsCourse = int.tryParse(value)?.toDouble() ?? gpsCourse;
            gpsSensorState = SensorState.ok;
            break;

          case 'pressure_hpa':
            barometerPressure = double.tryParse(value) ?? barometerPressure;
            barometerSensorState = SensorState.ok;
            break;

          case 'altitude_msl_m':
            altitudeMslM = double.tryParse(value) ?? altitudeMslM;
            barometerSensorState = SensorState.ok;
            break;

          case 'kalman_z':
            kalmanAltitudeM = double.tryParse(value) ?? kalmanAltitudeM;
            break;

          case 'kalman_v':
            kalmanVelocityMS = double.tryParse(value) ?? kalmanVelocityMS;
            break;

          case 'radio':
            radioState = value.toLowerCase() == 'connected'
                ? RadioState.connected
                : value.toLowerCase() == 'connecting'
                    ? RadioState.connecting
                    : RadioState.disconnected;
            break;

          case 'ack':
          case 'res':
          case 'err':
            ConsoleService().log('Réponse STM32: $value');
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
