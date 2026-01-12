// ...existing code...
import 'dart:async';
import 'dart:io';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:google_fonts/google_fonts.dart';
import '/flutter_flow/flutter_flow_theme.dart';
import '/flutter_flow/flutter_flow_util.dart';
import 'connection_page_model.dart';
export 'connection_page_model.dart';
import 'package:provider/provider.dart';
import '../../services/bluetooth_service.dart';

class ConnectionPageWidget extends StatefulWidget {
  const ConnectionPageWidget({super.key});

  static String routeName = 'ConnectionPage';
  static String routePath = '/connectionPage';

  @override
  State<ConnectionPageWidget> createState() => _ConnectionPageWidgetState();
}

class _ConnectionPageWidgetState extends State<ConnectionPageWidget> {
  late ConnectionPageModel _model;

  final scaffoldKey = GlobalKey<ScaffoldState>();

  @override
  void initState() {
    super.initState();
    _model = createModel(context, () => ConnectionPageModel());

    _model.switchValue = false;

    // Écoute l'état du Bluetooth
    FlutterBluePlus.adapterState.listen((state) {
      debugPrint('Bluetooth state: $state');
      if (state != BluetoothAdapterState.on) {
        _showBluetoothDisabledDialog();
      }
    });
  }

  @override
  void dispose() {
    // Stop scan via service if available
    try {
      final svc = context.read<BluetoothServiceManager>();
      svc.stopScan();
    } catch (_) {}
    _model.dispose();
    super.dispose();
  }

  void _showBluetoothDisabledDialog() {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: const Text('Bluetooth désactivé'),
        content: const Text(
          'Le Bluetooth est requis pour rechercher des appareils.\n'
          'Veuillez l’activer dans les paramètres.',
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text('Annuler'),
          ),
          ElevatedButton(
            onPressed: () {
              Navigator.pop(context);
              if (!kIsWeb && Platform.isAndroid) {
                FlutterBluePlus.turnOn();
              }
            },
            child: const Text('Activer'),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final bt = context.watch<BluetoothServiceManager>();

    return GestureDetector(
      onTap: () {
        FocusScope.of(context).unfocus();
        FocusManager.instance.primaryFocus?.unfocus();
      },
      child: PopScope(
        canPop: false,
        child: Scaffold(
          key: scaffoldKey,
          backgroundColor: FlutterFlowTheme.of(context).primaryBackground,
          body: SafeArea(
            child: Padding(
              padding: const EdgeInsets.symmetric(horizontal: 16.0),
              child: SingleChildScrollView(
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const SizedBox(height: 24.0),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Text(
                              'Connexion',
                              style: FlutterFlowTheme.of(context)
                                  .displaySmall
                                  .copyWith(fontSize: 28.0),
                            ),
                            Text(
                              'Connexion de l\'application Nexus à l\'ODB',
                              style: FlutterFlowTheme.of(context).bodyMedium,
                            ),
                          ],
                        ),
                        Icon(
                          Icons.settings,
                          color: FlutterFlowTheme.of(context).primaryText,
                        ),
                      ],
                    ),
                    const SizedBox(height: 16.0),
                    Container(
                      width: double.infinity,
                      height: 500,
                      decoration: BoxDecoration(
                        color: FlutterFlowTheme.of(context).secondaryBackground,
                        boxShadow: [
                          BoxShadow(
                              blurRadius: 4.0,
                              color: Colors.black.withValues(alpha: 0.2),
                              offset: const Offset(0, 2)),
                        ],
                        borderRadius: BorderRadius.circular(12.0),
                      ),
                      child: Padding(
                        padding: const EdgeInsets.all(16.0),
                        child: Column(
                          children: [
                            Row(
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Row(
                                  children: [
                                    Icon(
                                      Icons.bluetooth,
                                      color: FlutterFlowTheme.of(context).primary,
                                    ),
                                    const SizedBox(width: 8),
                                    Text(
                                      'Scan Bluetooth',
                                      style: FlutterFlowTheme.of(context)
                                          .titleMedium,
                                    ),
                                  ],
                                ),
                                Switch(
                                  value: bt.isScanning,
                                  onChanged: (newValue) async {
                                    if (newValue) {
                                      await context.read<BluetoothServiceManager>().startScan();
                                    } else {
                                      await context.read<BluetoothServiceManager>().stopScan();
                                    }
                                  },
                                  activeThumbColor:
                                      FlutterFlowTheme.of(context).primary,
                                ),
                              ],
                            ),
                            Text(
                              bt.isScanning
                                  ? 'Recherche d\'appareils en cours...'
                                  : 'Scan Bluetooth arrêté',
                              textAlign: TextAlign.center,
                              style: FlutterFlowTheme.of(context).bodyMedium,
                            ),
                            const Divider(),
                            Expanded(
                              child: ListView.builder(
                                itemCount: bt.scanResults.length,
                                itemBuilder: (context, index) {
                                  final result = bt.scanResults[index];
                                  final device = result.device;
                                  final connected =
                                      bt.connectedDevice?.remoteId == device.remoteId;

                                  return ListTile(
                                    leading: const Icon(Icons.bluetooth),
                                    title: Text(device.platformName.isNotEmpty
                                        ? device.platformName
                                        : 'Appareil inconnu'),
                                    subtitle: Text(device.remoteId.str),
                                    trailing: ElevatedButton(
                                      onPressed: connected
                                          ? null
                                          : () async {
                                              try {
                                                await context.read<BluetoothServiceManager>().connect(device);
                                              } catch (e) {
                                                if (mounted) {
                                                  ScaffoldMessenger.of(context).showSnackBar(
                                                    SnackBar(content: Text('Erreur connexion: $e')),
                                                  );
                                                }
                                              }
                                            },
                                      child: Text(connected ? 'Connecté' : 'Connecter'),
                                    ),
                                  );
                                },
                              ),
                            ),
                            if (bt.connectedDevice != null)
                              Padding(
                                padding: const EdgeInsets.only(top: 12.0),
                                child: ListTile(
                                  leading: const Icon(Icons.bluetooth_connected),
                                  title: Text(bt.connectedDevice?.platformName ?? bt.connectedDevice?.remoteId.str ?? 'Connecté'),
                                  subtitle: const Text('Appareil connecté via service'),
                                  trailing: ElevatedButton(
                                    onPressed: () => context.read<BluetoothServiceManager>().disconnect(),
                                    child: const Text('Déconnecter'),
                                  ),
                                ),
                              ),
                          ],
                        ),
                      ),
                    ),
                  ],
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }
}