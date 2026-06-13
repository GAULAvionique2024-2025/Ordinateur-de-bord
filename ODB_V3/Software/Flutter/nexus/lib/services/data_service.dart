import 'package:flutter/foundation.dart';
import 'dart:math';
import 'dart:convert';
import 'package:nexus/services/bluetooth_service.dart';
import 'package:nexus/services/console_service.dart';

enum SensorState { unknown, ok, error }

enum RadioState { disconnected, connecting, connected }

class DataServiceManager with ChangeNotifier {
  static const int stageRoleBooster = 2;
  static const int stageRoleSustainer = 3;
  static const int eventFlagPyrosArmed = 1 << 0;
  static const int eventFlagPyro1Fired = 1 << 1;
  static const int eventFlagPyro2Fired = 1 << 2;
  static const int eventFlagPyro3Fired = 1 << 3;
  static const int eventFlagPyro4Fired = 1 << 4;
  static const int eventFlagApogeeDetected = 1 << 5;
  static const int eventFlagMainDeployed = 1 << 6;
  static const int eventFlagDrogueDeployed = 1 << 7;
  static const int eventFlagMachLockEnabled = 1 << 8;

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
  String odbConfigFrameVersion = '';
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
  int pyrosArmingFailsafeMs = 0;
  int apogeeFailsafeMs = 0;
  int idefixFrequencyHz = 0;
  List<int> pyroRoles = List.filled(4, 0);
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
    if (hasValidGpsFix) {
      return '${gpsAlt.toStringAsFixed(0)} m';
    }
    return '—';
  }

  int get pyrosActiveCount => pyros.where((p) => p).length;
  String get pyrosSummary => pyros.map((p) => p ? '1' : '0').join(',');
  static const List<String> _pyroRoleNames = [
    'None',
    'Main',
    'Drogue',
    'Main #2',
    'Drogue #1',
  ];

  String pyroRoleLabel(int pyroIndex, {bool connected = true}) {
    if (!connected) return '-';
    final roleIndex = pyroIndex < pyroRoles.length ? pyroRoles[pyroIndex] : 0;
    if (roleIndex < 0 || roleIndex >= _pyroRoleNames.length) {
      return _pyroRoleNames.first;
    }
    return _pyroRoleNames[roleIndex];
  }

  String pyroDisplayLabel(int pyroIndex, {required bool connected}) {
    return connected
        ? 'Pyro ${pyroIndex + 1} (${pyroRoleLabel(pyroIndex, connected: connected)})'
        : 'Pyro ${pyroIndex + 1} (-)';
  }

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
  String get eventStateDisplay {
    if (eventStates <= 0) return '—';

    final activeFlags = <String>[];
    if (eventPyrosArmed) activeFlags.add('Pyros armed');
    if (eventPyro1Fired) activeFlags.add('Pyro 1 fired');
    if (eventPyro2Fired) activeFlags.add('Pyro 2 fired');
    if (eventPyro3Fired) activeFlags.add('Pyro 3 fired');
    if (eventPyro4Fired) activeFlags.add('Pyro 4 fired');
    if (eventApogeeDetected) activeFlags.add('Apogee detected');
    if (eventMainDeployed) activeFlags.add('Main deployed');
    if (eventDrogueDeployed) activeFlags.add('Drogue deployed');
    if (eventMachLockEnabled) activeFlags.add('Mach lock enabled');

    return activeFlags.isEmpty ? 'Events $eventStates' : activeFlags.join(', ');
  }

  bool get eventPyrosArmed => (eventStates & eventFlagPyrosArmed) != 0;
  bool get eventPyro1Fired => (eventStates & eventFlagPyro1Fired) != 0;
  bool get eventPyro2Fired => (eventStates & eventFlagPyro2Fired) != 0;
  bool get eventPyro3Fired => (eventStates & eventFlagPyro3Fired) != 0;
  bool get eventPyro4Fired => (eventStates & eventFlagPyro4Fired) != 0;
  bool get eventApogeeDetected => (eventStates & eventFlagApogeeDetected) != 0;
  bool get eventMainDeployed => (eventStates & eventFlagMainDeployed) != 0;
  bool get eventDrogueDeployed => (eventStates & eventFlagDrogueDeployed) != 0;
  bool get eventMachLockEnabled => (eventStates & eventFlagMachLockEnabled) != 0;
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
    String get gpsVelocityDisplay => hasValidGpsFix
      ? '${gpsVelocity.toStringAsFixed(1)} m/s'
      : '—';
    String get gpsCourseDisplay => hasValidGpsFix
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
    String get gpsLatDisplay => hasValidGpsFix
      ? '${gpsLat.toStringAsFixed(6)}°'
      : '—';
    String get gpsLonDisplay => hasValidGpsFix
      ? '${gpsLon.toStringAsFixed(6)}°'
      : '—';
    String get gpsAltDisplay => hasValidGpsFix
      ? '${gpsAlt.toStringAsFixed(1)} m'
      : '—';
  String get gpsSatellitesDisplay =>
      hasValidGpsFix
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
  bool get hasValidGpsFix => hasConnection && gpsSensorState == SensorState.ok && gpsFix > 0;

  void resetOdbConfig() {
    timeBootMs = 0;
    systemStates = 0;
    eventStates = 0;
    missionState = -1;
    odbState = '';
    odbFrameVersion = '';
    odbConfigFrameVersion = '';
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
    pyrosArmingFailsafeMs = 0;
    apogeeFailsafeMs = 0;
    idefixFrequencyHz = 0;
    pyroRoles = List.filled(4, 0);
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
    await btService.sendBinary(0x03, [0x06]);
  }

  Future<void> commandPing() async {
    if (!hasConnection) {
      ConsoleService().log('Aucune connexion Bluetooth avec l\'ODB');
      return;
    }
    await btService.sendBinary(0x03, [0x01]);
  }

  Future<void> commandArm(bool arm) async {
    if (!hasConnection) {
      ConsoleService().log('Aucune connexion Bluetooth avec l\'ODB');
      return;
    }
    await btService.sendBinary(0x03, [0x02, arm ? 1 : 0]);
  }

  Future<void> commandFire(int pyroIndex) async {
    if (!hasConnection) {
      ConsoleService().log('Aucune connexion Bluetooth avec l\'ODB');
      return;
    }
    await btService.sendBinary(0x03, [0x03, pyroIndex]);
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
    required String pyrosArmingFailsafeMs,
    required String apogeeFailsafeMs,
    required String idefixFrequencyHz,
    required List<int> pyroRoles,
  }) async {
    if (!hasConnection) {
      ConsoleService().log('Aucune connexion Bluetooth avec l\'ODB');
      return;
    }

    final byteData = ByteData(92);
    int offset = 0;

    // 1. magic_number & Header
    byteData.setUint32(offset, 0x434F4E46, Endian.little); 
    offset += 4;

    byteData.setUint8(offset, 1);
    offset += 1;
    byteData.setUint8(offset, 0);
    offset += 1;
    byteData.setUint16(offset, 92, Endian.little);
    offset += 2;

    // 2. odb_name[32]
    final nameBytes = utf8.encode(odbName.trim());
    for (int i = 0; i < 32; i++) {
      // On remplit avec le nom et on padde avec des zéros (null-terminator)
      byteData.setUint8(offset + i, i < nameBytes.length && i < 31 ? nameBytes[i] : 0);
    }
    offset += 32;

    // 3. stage_role (2 = BOOSTER, 3 = SUSTAINER)
    byteData.setUint8(offset, _parseInt(stageRole, this.stageRole)); 
    offset += 1;

    // 4. debug_mode
    byteData.setUint8(offset, debugMode ? 1 : 0); 
    offset += 1;

    // 5. fire_attempt_delay_ms
    byteData.setUint32(offset, _parseInt(fireAttemptDelayMs, this.fireAttemptDelayMs), Endian.little); 
    offset += 4;

    // 6. pyros_arming_failsafe_ms
    byteData.setUint32(offset, _parseInt(pyrosArmingFailsafeMs, this.pyrosArmingFailsafeMs), Endian.little); 
    offset += 4;

    // 7. min_needed_pyro_nb
    byteData.setUint8(offset, _parseInt(minNeededPyroNb, this.minNeededPyroNb)); 
    offset += 1;

    // 8. pyro_roles[4]
    for (int i = 0; i < 4; i++) {
      byteData.setUint8(offset + i, i < pyroRoles.length ? pyroRoles[i] : 0);
    }
    offset += 4;

    // 9. acc_z_launch_threshold
    byteData.setFloat32(offset, _parseDouble(accZLaunchThreshold, this.accZLaunchThreshold), Endian.little); 
    offset += 4;

    // 10. boost_phase_v_threshold
    byteData.setFloat32(offset, _parseDouble(boostPhaseVThreshold, this.boostPhaseVThreshold), Endian.little); 
    offset += 4;

    // 11. apogee_detect_v_threshold
    byteData.setFloat32(offset, _parseDouble(apogeeDetectVThreshold, this.apogeeDetectVThreshold), Endian.little); 
    offset += 4;

    // 12. landing_detect_v_threshold
    byteData.setFloat32(offset, _parseDouble(landingDetectVThreshold, this.landingDetectVThreshold), Endian.little); 
    offset += 4;

    // 13. landing_detect_threshold_ms
    byteData.setUint32(offset, _parseInt(landingDetectThresholdMs, this.landingDetectThresholdMs), Endian.little); 
    offset += 4;

    // 14. apogee_failsafe_ms
    byteData.setUint32(offset, _parseInt(apogeeFailsafeMs, this.apogeeFailsafeMs), Endian.little); 
    offset += 4;

    // 15. main_deploy_altitude_threshold_m
    byteData.setFloat32(offset, _parseDouble(mainDeployAltitudeThresholdM, this.mainDeployAltitudeThresholdM), Endian.little); 
    offset += 4;

    // 16. drogue_fire_attempt_max_nb
    byteData.setUint8(offset, _parseInt(drogueFireAttemptMaxNb, this.drogueFireAttemptMaxNb)); 
    offset += 1;

    // 17. main_fire_attempt_max_nb
    byteData.setUint8(offset, _parseInt(mainFireAttemptMaxNb, this.mainFireAttemptMaxNb)); 
    offset += 1;

    // 18. enable_buzzer
    byteData.setUint8(offset, enableBuzzer ? 1 : 0); 
    offset += 1;

    // 19. buzzer_report_tone_hz
    byteData.setUint16(offset, _parseInt(buzzerReportToneHz, this.buzzerReportToneHz), Endian.little); 
    offset += 2;

    // 20. idefix_frequency_hz
    byteData.setUint32(offset, _parseInt(idefixFrequencyHz, this.idefixFrequencyHz), Endian.little); 
    offset += 4;

    await btService.sendBinary(0x02, byteData.buffer.asUint8List());
    ConsoleService().log('Envoi de la nouvelle configuration ...');

    await Future.delayed(const Duration(milliseconds: 200)); 

    await btService.sendBinary(0x03, [0x04]);
    ConsoleService().log('Demande de sauvegarde Flash et de redémarrage envoyée.');
  }

  Future<void> resetOdbSettingsToDefault() async {
    if (!hasConnection) {
      ConsoleService().log('Aucune connexion Bluetooth avec l\'ODB');
      return;
    }
    
    await btService.sendBinary(0x03, [0x05]);
    ConsoleService().log('Demande de réinitialisation usine envoyée.');
  }

  Future<void> resetOdbMemory() async {
    if (!hasConnection) {
      ConsoleService().log('Aucune connexion Bluetooth avec l\'ODB');
      return;
    }

    await btService.sendBinary(0x03, [0x07]);
    ConsoleService().log('Demande de réinitialisation mémoire envoyée.');
  }

  // ---------- PARSER ----------
  void parseBinaryMessage(int type, List<int> payload) {
    if (_isDisposed) return;
    final ByteData view = ByteData.sublistView(Uint8List.fromList(payload));

    try {
      if (type == 0x01) { // MSG_TELEMETRY
        int offset = 0;
        
        int versionMajor = view.getUint8(offset); offset += 1;
        int versionMinor = view.getUint8(offset); offset += 1;
        int payloadSize = view.getUint16(offset, Endian.little); offset += 2;
        odbFrameVersion = 'v$versionMajor.$versionMinor';

        if (versionMajor == 1 && versionMinor == 0) {
          // --- Status ---
          timeBootMs = view.getUint32(offset, Endian.little); offset += 4;
          systemStates = view.getUint16(offset, Endian.little); offset += 2;
          eventStates = view.getUint16(offset, Endian.little); offset += 2;
          missionState = view.getUint8(offset); offset += 1;
          vinMv = view.getUint16(offset, Endian.little); offset += 2;
          batteryVoltage = vinMv / 1000.0;
          
          // --- IMU ---
          roll = view.getFloat32(offset, Endian.little); offset += 4;
          pitch = view.getFloat32(offset, Endian.little); offset += 4;
          yaw = view.getFloat32(offset, Endian.little); offset += 4;
          
          imuAccX = view.getFloat32(offset, Endian.little); offset += 4;
          imuAccY = view.getFloat32(offset, Endian.little); offset += 4;
          imuAccZ = view.getFloat32(offset, Endian.little); offset += 4;
          
          imuGyroX = view.getFloat32(offset, Endian.little); offset += 4;
          imuGyroY = view.getFloat32(offset, Endian.little); offset += 4;
          imuGyroZ = view.getFloat32(offset, Endian.little); offset += 4;
          
          imuMagX = view.getFloat32(offset, Endian.little); offset += 4;
          imuMagY = view.getFloat32(offset, Endian.little); offset += 4;
          imuMagZ = view.getFloat32(offset, Endian.little); offset += 4;
          
          // --- Pressure & Temp ---
          altitudeMslM = view.getFloat32(offset, Endian.little); offset += 4;
          barometerPressure = view.getFloat32(offset, Endian.little); offset += 4;
          temperature = view.getFloat32(offset, Endian.little); offset += 4;
          
          // --- High-G ---
          accHighGX = view.getFloat32(offset, Endian.little); offset += 4;
          accHighGY = view.getFloat32(offset, Endian.little); offset += 4;
          accHighGZ = view.getFloat32(offset, Endian.little); offset += 4;
          
          // --- GPS ---
          gpsFix = view.getUint8(offset); offset += 1;
          gpsLat = view.getInt32(offset, Endian.little) / 10000000.0; offset += 4; // Format degE7
          gpsLon = view.getInt32(offset, Endian.little) / 10000000.0; offset += 4; // Format degE7
          gpsAlt = view.getInt32(offset, Endian.little) / 1000.0; offset += 4;     // Format mm -> m
          gpsVelocity = view.getUint16(offset, Endian.little) / 100.0; offset += 2; // Format cm/s -> m/s
          gpsCourse = view.getUint16(offset, Endian.little) / 100.0; offset += 2;   // Format cdeg -> deg
          gpsSatellites = view.getUint8(offset); offset += 1;
          
          // --- Statistics ---
          imuAccVertical = view.getFloat32(offset, Endian.little); offset += 4;
          highGAccVertical = view.getFloat32(offset, Endian.little); offset += 4;
          kalmanAltitudeM = view.getFloat32(offset, Endian.little); offset += 4;
          kalmanVelocityMS = view.getFloat32(offset, Endian.little); offset += 4;
          
          gpsSensorState = (systemStates & (1 << 8)) != 0 ? SensorState.ok : SensorState.error;
          barometerSensorState = (systemStates & (1 << 7)) != 0 ? SensorState.ok : SensorState.error;
          imuSensorState = (systemStates & (1 << 6)) != 0 ? SensorState.ok : SensorState.error;
          accHighGSensorState = (systemStates & (1 << 9)) != 0 ? SensorState.ok : SensorState.error;
          temperatureSensorState = (systemStates & (1 << 10)) != 0 ? SensorState.ok : SensorState.error;
          sdSensorState = (systemStates & (1 << 11)) != 0 ? SensorState.ok : SensorState.error;
          
          batterySensorState = batteryVoltage > 0 ? SensorState.ok : SensorState.error;
          goodPowerState = batteryVoltage >= 7.0;
          
          pyrosArmed = eventPyrosArmed;

          bool pyro1Conn = (systemStates & (1 << 3)) != 0;
          bool pyro1Fired = eventPyro1Fired;
          pyros[0] = pyro1Conn && !pyro1Fired;

          bool pyro2Conn = (systemStates & (1 << 2)) != 0;
          bool pyro2Fired = eventPyro2Fired;
          pyros[1] = pyro2Conn && !pyro2Fired;

          bool pyro3Conn = (systemStates & (1 << 1)) != 0;
          bool pyro3Fired = eventPyro3Fired;
          pyros[2] = pyro3Conn && !pyro3Fired;

          bool pyro4Conn = (systemStates & (1 << 0)) != 0;
          bool pyro4Fired = eventPyro4Fired;
          pyros[3] = pyro4Conn && !pyro4Fired;

          _safeNotifyListeners();

        } else {
          ConsoleService().log('Erreur: Télémétrie v$versionMajor.$versionMinor non supportée.');
        }

      } else if (type == 0x02) { // MSG_CONFIG_SET
        int offset = 0;
        int magicNumber = view.getUint32(offset, Endian.little); offset += 4;
        
        if (magicNumber == 0x434F4E46) {
           int versionMajor = view.getUint8(offset); offset += 1;
           int versionMinor = view.getUint8(offset); offset += 1;
           int payloadSize = view.getUint16(offset, Endian.little); offset += 2;
           odbConfigFrameVersion = 'v$versionMajor.$versionMinor';
           
           if (versionMajor == 1 && versionMinor == 0) {
             // 1. Extraction du nom
             List<int> nameBytes = [];
             for (int i = 0; i < 32; i++) {
               int b = view.getUint8(offset + i);
               if (b != 0) nameBytes.add(b);
             }
             odbName = utf8.decode(nameBytes);
             offset += 32;

             // 2. Variables simples
             stageRole = view.getUint8(offset); offset += 1;
             debugMode = view.getUint8(offset) == 1; offset += 1;
             fireAttemptDelayMs = view.getUint32(offset, Endian.little); offset += 4;
             pyrosArmingFailsafeMs = view.getUint32(offset, Endian.little); offset += 4;
             minNeededPyroNb = view.getUint8(offset); offset += 1;

             // 3. Tableau des rôles pyros
             pyroRoles = [];
             for (int i = 0; i < 4; i++) {
               pyroRoles.add(view.getUint8(offset + i));
             }
             offset += 4;

             // 4. Seuils
             accZLaunchThreshold = view.getFloat32(offset, Endian.little); offset += 4;
             boostPhaseVThreshold = view.getFloat32(offset, Endian.little); offset += 4;
             apogeeDetectVThreshold = view.getFloat32(offset, Endian.little); offset += 4;
             landingDetectVThreshold = view.getFloat32(offset, Endian.little); offset += 4;
             landingDetectThresholdMs = view.getUint32(offset, Endian.little); offset += 4;
             apogeeFailsafeMs = view.getUint32(offset, Endian.little); offset += 4;
             mainDeployAltitudeThresholdM = view.getFloat32(offset, Endian.little); offset += 4;
             drogueFireAttemptMaxNb = view.getUint8(offset); offset += 1;
             mainFireAttemptMaxNb = view.getUint8(offset); offset += 1;
             enableBuzzer = view.getUint8(offset) == 1; offset += 1;
             buzzerReportToneHz = view.getUint16(offset, Endian.little); offset += 2;
             idefixFrequencyHz = view.getUint32(offset, Endian.little); offset += 4;

             // 5. Validation et réveil de l'Interface UI
             hasOdbConfig = true;
             _safeNotifyListeners();
             ConsoleService().log('Configuration ODB lue et synchronisée avec succès !');
           } else {
             ConsoleService().log('Erreur: Configuration ODB v$versionMajor.$versionMinor non supportée.');
           }
        } else {
          ConsoleService().log('Erreur: Magic Number invalide lors de la réception de la configuration.');
        }

      } else if (type == 0x04) { // MSG_ACK
        int cmdAcked = view.getUint8(0);
        int status = view.getUint8(1);
        ConsoleService().log('ACK Reçu (CMD: 0x${cmdAcked.toRadixString(16)}, Status: ${status == 1 ? "OK" : "FAIL"})');
      }
    } catch (e) {
      ConsoleService().log('Erreur parsing binaire (Type $type): $e');
    }
  }

  @override
  void dispose() {
    _isDisposed = true;
    super.dispose();
  }
}
