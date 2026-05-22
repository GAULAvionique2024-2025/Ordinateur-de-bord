import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:provider/provider.dart';
import 'package:nexus/services/bluetooth_service.dart';
import 'package:nexus/services/data_service.dart';

void main() {
  testWidgets('parses the App_SendFrame telemetry and updates shared consumers', (tester) async {
    final bluetoothService = BluetoothServiceManager();
    final dataService = DataServiceManager(bluetoothService);

    await tester.pumpWidget(
      MultiProvider(
        providers: [
          ChangeNotifierProvider<BluetoothServiceManager>.value(value: bluetoothService),
          ChangeNotifierProvider<DataServiceManager>.value(value: dataService),
        ],
        child: MaterialApp(
          home: Scaffold(
            body: Column(
              children: [
                Consumer<DataServiceManager>(
                  builder: (context, data, child) {
                    return Text(
                      'overview:${data.timeBootMs}|${data.vinMv}|${data.gpsAlt}',
                      key: const Key('overview-view'),
                    );
                  },
                ),
                Consumer<DataServiceManager>(
                  builder: (context, data, child) {
                    return Text(
                      'settings:${data.temperature}|${data.barometerPressure}|${data.kalmanVelocityMS}',
                      key: const Key('settings-view'),
                    );
                  },
                ),
              ],
            ),
          ),
        ),
      ),
    );

    dataService.parseMessage(
      'time_boot_ms=12345\r\n'
      'battery_mv=7400\r\n'
      'gps_alt=123400\r\n'
      'temp_celsius=2150\r\n'
      'pressure_hpa=101325\r\n'
      'kalman_v=345\r\n',
    );

    await tester.pump();

    expect(find.byKey(const Key('overview-view')), findsOneWidget);
    expect(find.text('overview:12345|7400|123.4'), findsOneWidget);
    expect(find.byKey(const Key('settings-view')), findsOneWidget);
    expect(find.text('settings:21.5|1013.25|3.45'), findsOneWidget);
  });

  test('parser ignores the old DATA format', () {
    final bluetoothService = BluetoothServiceManager();
    final dataService = DataServiceManager(bluetoothService);

    dataService.parseMessage('DATA,time_boot_ms=999,battery_mv=9999');

    expect(dataService.timeBootMs, 0);
    expect(dataService.vinMv, 0);
    expect(dataService.batteryVoltage, 0.0);
  });
}
