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
      'DATA,time_boot_ms=12345,battery_mv=7400,gps_alt=123400,temp_celsius=2150,pressure_hpa=101325,kalman_v=345,pyro_roles=1:2:3:4\r\n',
    );

    await tester.pump();

    expect(find.byKey(const Key('overview-view')), findsOneWidget);
    expect(find.text('overview:12345|7400|123.4'), findsOneWidget);
    expect(find.byKey(const Key('settings-view')), findsOneWidget);
    expect(find.text('settings:21.5|1013.25|3.45'), findsOneWidget);
  });

  test('parser accepts configuration pyro roles', () {
    final bluetoothService = BluetoothServiceManager();
    final dataService = DataServiceManager(bluetoothService);

    dataService.parseMessage('CFG:PYRO_ROLE=0,0\r\n');
    dataService.parseMessage('CFG:PYRO_ROLE=1,1\r\n');
    dataService.parseMessage('CFG:PYRO_ROLE=2,2\r\n');
    dataService.parseMessage('CFG:PYRO_ROLE=3,3\r\n');

    expect(dataService.pyroRoles, equals(<int>[0, 1, 2, 3]));
  });

  test('parser keeps telemetry pyro roles separate from config roles', () {
    final bluetoothService = BluetoothServiceManager();
    final dataService = DataServiceManager(bluetoothService);

    dataService.parseMessage('DATA,pyro_roles=1:2:3:4');

    expect(dataService.pyroRoles, equals(<int>[0, 0, 0, 0]));
  });
}
