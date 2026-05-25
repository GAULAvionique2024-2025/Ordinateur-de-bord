import 'package:flutter/foundation.dart';
import 'dart:math';
import 'package:nexus/services/bluetooth_service.dart';
import 'package:nexus/services/console_service.dart';

enum SensorState { unknown, ok, error }

enum RadioState { disconnected, connecting, connected }

class DataServiceManager with ChangeNotifier {
  static const int stageRoleBooster = 2;
  static const int stageRoleSustainer = 3;

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
  String odbFrameVersion = '';
  bool hasOdbConfig = false;
  String odbName = '';
  int stageRole = stageRoleSustainer;
  bool debugMode = false;
  bool enableBuzzer = false;
  int minNeededPyroNb = 0;
  int drogueFireAttemptMaxNb = 0;
  int mainFireAttemptMaxNb = 0;
  double accZLaunchThreshold = 0.0;
  double boostPhaseVThreshold = 0.0;
  double apogeeDetectVThreshold = 0.0;
  double mainDeployAltitudeThresholdM = 0.0;
  double landingDetectVThreshold = 0.0;
  int buzzerReportToneHz = 0;
  int landingDetectThresholdMs = 0;
  int fireAttemptDelayMs = 0;
  int pyrosArmingFailsafeTicks = 0;
  int apogeeFailsafeTicks = 0;
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
  int gpsFix = 0;
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
  String get temperatureDisplay =>
      temperature != 0.0 ? '${temperature.toStringAsFixed(1)}°C' : '—';
  double get sdUsagePercent =>
      sdMax > 0 ? (sdUsed / sdMax * 100).clamp(0, 100) : 0.0;
  double pressureToAltitude(
    double pressureHpa, [
    double seaLevelHpa = 1013.25,
  ]) {
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

  String get systemStateDisplay =>
      systemStates > 0 ? 'Flags $systemStates' : '—';
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
  String get highGAccDisplay =>
      hasConnection && accHighGSensorState == SensorState.ok
      ? 'X: ${accHighGX.toStringAsFixed(2)} | Y: ${accHighGY.toStringAsFixed(2)} | Z: ${accHighGZ.toStringAsFixed(2)} m/s²'
      : '—';

  String get imuAccVerticalDisplay =>
      hasConnection && imuSensorState == SensorState.ok
      ? '${imuAccVertical.toStringAsFixed(2)} m/s²'
      : '—';

  String get highGAccVerticalDisplay =>
      hasConnection && accHighGSensorState == SensorState.ok
      ? '${highGAccVertical.toStringAsFixed(2)} m/s²'
      : '—';

  // --- Barometer ---
  String get pressureDisplay =>
      hasConnection && barometerSensorState == SensorState.ok
      ? '${barometerPressure.toStringAsFixed(2)} hPa'
      : '—';

  String get altitudeMslDisplay =>
      hasConnection ? '${altitudeMslM.toStringAsFixed(2)} m' : '—';

  String get kalmanAltitudeDisplay =>
      hasConnection ? kalmanAltitudeM.toStringAsFixed(2) : '—';

  String get kalmanVelocityDisplay =>
      hasConnection ? kalmanVelocityMS.toStringAsFixed(2) : '—';

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
  String get gpsSatellitesDisplay =>
      hasConnection && gpsSensorState == SensorState.ok
      ? '$gpsSatellites satellites'
      : '—';
  String get gpsFixDisplay =>
      hasConnection ? (gpsFix > 0 ? '✓ Actif' : '✗ Aucun fix') : '—';

  bool get odbSensorState =>
      (temperatureSensorState == SensorState.ok &&
      imuSensorState == SensorState.ok &&
      accHighGSensorState == SensorState.ok &&
      sdSensorState == SensorState.ok &&
      gpsSensorState == SensorState.ok &&
      gpsFix >= 1 &&
      barometerSensorState == SensorState.ok &&
      goodPowerState == true &&
      pyrosActiveCount >= 0);
  bool get missionReady => odbSensorState && radioState == RadioState.connected;
  bool get hasConnection => btService.connectedDevice != null;

  void resetOdbConfig() {
    timeBootMs = 0;
    systemStates = 0;
    eventStates = 0;
    missionState = -1;
    odbState = '';
    odbFrameVersion = '';
    hasOdbConfig = false;
    odbName = '';
    stageRole = stageRoleSustainer;
    debugMode = false;
    enableBuzzer = false;
    minNeededPyroNb = 0;
    drogueFireAttemptMaxNb = 0;
    mainFireAttemptMaxNb = 0;
    accZLaunchThreshold = 0.0;
    boostPhaseVThreshold = 0.0;
    apogeeDetectVThreshold = 0.0;
    mainDeployAltitudeThresholdM = 0.0;
    landingDetectVThreshold = 0.0;
    buzzerReportToneHz = 0;
    landingDetectThresholdMs = 0;
    fireAttemptDelayMs = 0;
    pyrosArmingFailsafeTicks = 0;
    apogeeFailsafeTicks = 0;
    _safeNotifyListeners();
  }

  int _parseInt(String value, int fallback) {
    return int.tryParse(value.trim()) ?? fallback;
  }

  double _parseDouble(String value, double fallback) {
    return double.tryParse(value.trim().replaceAll(',', '.')) ?? fallback;
  }

  Future<void> refreshOdb() async {
    if (!hasConnection) {
      ConsoleService().log('Aucune connexion Bluetooth avec l\'ODB');
      return;
    }

    await btService.send('HELLO\r\n');
  }

  Future<void> applyOdbSettings({
    required String odbName,
    required String stageRole,
    required bool debugMode,
    required bool enableBuzzer,
    required String minNeededPyroNb,
    required String drogueFireAttemptMaxNb,
    required String mainFireAttemptMaxNb,
    required String accZLaunchThreshold,
    required String boostPhaseVThreshold,
    required String apogeeDetectVThreshold,
    required String mainDeployAltitudeThresholdM,
    required String landingDetectVThreshold,
    required String buzzerReportToneHz,
    required String landingDetectThresholdMs,
    required String fireAttemptDelayMs,
    required String pyrosArmingFailsafeTicks,
    required String apogeeFailsafeTicks,
  }) async {
    if (!hasConnection) {
      ConsoleService().log('Aucune connexion Bluetooth avec l\'ODB');
      return;
    }

    final commands = <String>[
      'CFG:NAME=${odbName.trim()}',
      'CFG:ROLE=${_parseInt(stageRole, this.stageRole)}',
      'CFG:DEBUG=${debugMode ? 1 : 0}',
      'CFG:BUZZER=${enableBuzzer ? 1 : 0}',
      'CFG:MIN_PYRO=${_parseInt(minNeededPyroNb, this.minNeededPyroNb)}',
      'CFG:MAX_DROGUE=${_parseInt(drogueFireAttemptMaxNb, this.drogueFireAttemptMaxNb)}',
      'CFG:MAX_MAIN=${_parseInt(mainFireAttemptMaxNb, this.mainFireAttemptMaxNb)}',
      'CFG:ACC_LAUNCH=${_parseDouble(accZLaunchThreshold, this.accZLaunchThreshold).toStringAsFixed(2)}',
      'CFG:V_BOOST=${_parseDouble(boostPhaseVThreshold, this.boostPhaseVThreshold).toStringAsFixed(2)}',
      'CFG:V_APOGEE=${_parseDouble(apogeeDetectVThreshold, this.apogeeDetectVThreshold).toStringAsFixed(2)}',
      'CFG:ALT_MAIN=${_parseDouble(mainDeployAltitudeThresholdM, this.mainDeployAltitudeThresholdM).toStringAsFixed(2)}',
      'CFG:V_LAND=${_parseDouble(landingDetectVThreshold, this.landingDetectVThreshold).toStringAsFixed(2)}',
      'CFG:TONE=${_parseInt(buzzerReportToneHz, this.buzzerReportToneHz)}',
      'CFG:T_LAND=${_parseInt(landingDetectThresholdMs, this.landingDetectThresholdMs)}',
      'CFG:DELAY_FIRE=${_parseInt(fireAttemptDelayMs, this.fireAttemptDelayMs)}',
      'CFG:FAIL_ARM=${_parseInt(pyrosArmingFailsafeTicks, this.pyrosArmingFailsafeTicks)}',
      'CFG:FAIL_APOGEE=${_parseInt(apogeeFailsafeTicks, this.apogeeFailsafeTicks)}',
      'CFG:APPLY',
    ];

    for (final command in commands) {
      await btService.send('$command\r\n');
    }

    // delay
    await Future.delayed(const Duration(milliseconds: 1000));

    await btService.send('HELLO\r\n');
  }

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
      } else if (normalized.startsWith('CFG:')) {
        final payload = normalized.substring(4);
        final equalsIndex = payload.indexOf('=');
        if (equalsIndex > 0 && equalsIndex < payload.length - 1) {
          entries[payload.substring(0, equalsIndex).trim().toLowerCase()] =
              payload.substring(equalsIndex + 1).trim();
        }
      } else {
        for (final match in RegExp(
          r'([A-Za-z0-9_]+)=([-+]?[0-9]*\.?[0-9]+)',
        ).allMatches(normalized)) {
          entries[match.group(1)!.trim().toLowerCase()] = match
              .group(2)!
              .trim();
        }

        if (entries.isEmpty) {
          for (final match in RegExp(
            r'([A-Za-z0-9_]+):\s*([^,;\r\n]+)',
          ).allMatches(normalized)) {
            entries[match.group(1)!.trim().toLowerCase()] = match
                .group(2)!
                .trim();
          }
        }
      }
      //ConsoleService().log('✅ Parsed ${entries.length} entries');

      double? parseScaledDouble(String value, double factor) {
        final parsed = double.tryParse(value);
        return parsed == null ? null : parsed / factor;
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
          case 'event_states':
            eventStates = int.tryParse(value) ?? eventStates;
            break;
          case 'mission_state':
            missionState = int.tryParse(value) ?? missionState;
            break;
          case 'ver':
            odbFrameVersion = value;
            break;
          case 'name':
            odbName = value;
            hasOdbConfig = true;
            break;
          case 'role':
            stageRole = int.tryParse(value) ?? stageRole;
            hasOdbConfig = true;
            break;
          case 'debug':
            debugMode = value == '1' || value.toLowerCase() == 'true';
            hasOdbConfig = true;
            break;
          case 'buzzer':
            enableBuzzer = value == '1' || value.toLowerCase() == 'true';
            hasOdbConfig = true;
            break;
          case 'min_pyro':
            minNeededPyroNb = int.tryParse(value) ?? minNeededPyroNb;
            hasOdbConfig = true;
            break;
          case 'max_drogue':
            drogueFireAttemptMaxNb =
                int.tryParse(value) ?? drogueFireAttemptMaxNb;
            hasOdbConfig = true;
            break;
          case 'max_main':
            mainFireAttemptMaxNb = int.tryParse(value) ?? mainFireAttemptMaxNb;
            hasOdbConfig = true;
            break;
          case 'acc_launch':
            accZLaunchThreshold = double.tryParse(value) ?? accZLaunchThreshold;
            hasOdbConfig = true;
            break;
          case 'v_boost':
            boostPhaseVThreshold =
                double.tryParse(value) ?? boostPhaseVThreshold;
            hasOdbConfig = true;
            break;
          case 'v_apogee':
            apogeeDetectVThreshold =
                double.tryParse(value) ?? apogeeDetectVThreshold;
            hasOdbConfig = true;
            break;
          case 'alt_main':
            mainDeployAltitudeThresholdM =
                double.tryParse(value) ?? mainDeployAltitudeThresholdM;
            hasOdbConfig = true;
            break;
          case 'v_land':
            landingDetectVThreshold =
                double.tryParse(value) ?? landingDetectVThreshold;
            hasOdbConfig = true;
            break;
          case 'tone':
            buzzerReportToneHz = int.tryParse(value) ?? buzzerReportToneHz;
            hasOdbConfig = true;
            break;
          case 't_land':
            landingDetectThresholdMs =
                int.tryParse(value) ?? landingDetectThresholdMs;
            hasOdbConfig = true;
            break;
          case 'delay_fire':
            fireAttemptDelayMs = int.tryParse(value) ?? fireAttemptDelayMs;
            hasOdbConfig = true;
            break;
          case 'fail_arm':
            pyrosArmingFailsafeTicks =
                int.tryParse(value) ?? pyrosArmingFailsafeTicks;
            hasOdbConfig = true;
            break;
          case 'fail_apogee':
            apogeeFailsafeTicks = int.tryParse(value) ?? apogeeFailsafeTicks;
            hasOdbConfig = true;
            break;
          case 'temp_celsius':
            temperature = parseScaledDouble(value, 100) ?? temperature;
            if (temperature >= -55 && temperature <= 150) {
              temperatureSensorState = SensorState.ok;
            } else {
              temperatureSensorState = SensorState.error;
            }
            break;

          case 'battery_mv':
            vinMv = int.tryParse(value) ?? vinMv;
            batteryVoltage = vinMv > 0
                ? vinMv / 1000.0
                : (int.tryParse(value) ?? (batteryVoltage * 1000).round()) /
                      1000.0;
            batterySensorState = batteryVoltage > 0
                ? SensorState.ok
                : SensorState.error;
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
            roll = parseScaledDouble(value, 100) ?? roll;
            imuSensorState = SensorState.ok;
            break;
          case 'pitch':
            pitch = parseScaledDouble(value, 100) ?? pitch;
            imuSensorState = SensorState.ok;
            break;
          case 'yaw':
            yaw = parseScaledDouble(value, 100) ?? yaw;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_acc_x':
            imuAccX = parseScaledDouble(value, 100) ?? imuAccX;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_acc_vertical':
            imuAccVertical = parseScaledDouble(value, 100) ?? imuAccVertical;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_acc_y':
            imuAccY = parseScaledDouble(value, 100) ?? imuAccY;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_acc_z':
            imuAccZ = parseScaledDouble(value, 100) ?? imuAccZ;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_gyro_x':
            imuGyroX = parseScaledDouble(value, 100) ?? imuGyroX;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_gyro_y':
            imuGyroY = parseScaledDouble(value, 100) ?? imuGyroY;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_gyro_z':
            imuGyroZ = parseScaledDouble(value, 100) ?? imuGyroZ;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_mag_x':
            imuMagX = parseScaledDouble(value, 100) ?? imuMagX;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_mag_y':
            imuMagY = parseScaledDouble(value, 100) ?? imuMagY;
            imuSensorState = SensorState.ok;
            break;
          case 'imu_mag_z':
            imuMagZ = parseScaledDouble(value, 100) ?? imuMagZ;
            imuSensorState = SensorState.ok;
            break;

          case 'highg_acc_x':
            accHighGX = parseScaledDouble(value, 100) ?? accHighGX;
            accHighGSensorState = SensorState.ok;
            break;
          case 'highg_acc_vertical':
            highGAccVertical =
                parseScaledDouble(value, 100) ?? highGAccVertical;
            accHighGSensorState = SensorState.ok;
            break;
          case 'highg_acc_y':
            accHighGY = parseScaledDouble(value, 100) ?? accHighGY;
            accHighGSensorState = SensorState.ok;
            break;
          case 'highg_acc_z':
            accHighGZ = parseScaledDouble(value, 100) ?? accHighGZ;
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

          case 'lat':
            gpsLat = parseScaledDouble(value, 10000000) ?? gpsLat;
            gpsSensorState = SensorState.ok;
            break;
          case 'lon':
            gpsLon = parseScaledDouble(value, 10000000) ?? gpsLon;
            gpsSensorState = SensorState.ok;
            break;
          case 'gps_alt':
            gpsAlt = parseScaledDouble(value, 1000) ?? gpsAlt;
            gpsSensorState = SensorState.ok;
            break;
          case 'satellites_nb':
            gpsSatellites = int.tryParse(value) ?? gpsSatellites;
            gpsSensorState = SensorState.ok;
            break;
          case 'gps_fix':
            gpsFix = int.tryParse(value) ?? gpsFix;
            gpsSensorState = SensorState.ok;
            break;
          case 'vel':
            gpsVelocity = parseScaledDouble(value, 100) ?? gpsVelocity;
            gpsSensorState = SensorState.ok;
            break;
          case 'cog':
            gpsCourse = parseScaledDouble(value, 100) ?? gpsCourse;
            gpsSensorState = SensorState.ok;
            break;

          case 'pressure_hpa':
            barometerPressure =
                parseScaledDouble(value, 100) ?? barometerPressure;
            barometerSensorState = SensorState.ok;
            break;

          case 'altitude_msl_m':
            altitudeMslM = parseScaledDouble(value, 100) ?? altitudeMslM;
            barometerSensorState = SensorState.ok;
            break;

          case 'kalman_z':
            kalmanAltitudeM = parseScaledDouble(value, 100) ?? kalmanAltitudeM;
            break;

          case 'kalman_v':
            kalmanVelocityMS =
                parseScaledDouble(value, 100) ?? kalmanVelocityMS;
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
          //ConsoleService().log('Clé inconnue: $key -> $value');
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
