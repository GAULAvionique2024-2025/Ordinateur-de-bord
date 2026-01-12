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
    // Stop scan via service si disponible
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
            top: true,
            child: Padding(
              padding: const EdgeInsetsDirectional.fromSTEB(16, 0, 16, 0),
              child: SingleChildScrollView(
                child: Column(
                  mainAxisSize: MainAxisSize.max,
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Row(
                      mainAxisSize: MainAxisSize.max,
                      mainAxisAlignment: MainAxisAlignment.spaceBetween,
                      children: [
                        Column(
                          mainAxisSize: MainAxisSize.max,
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Text(
                              'Connexion',
                              style: FlutterFlowTheme.of(context)
                                  .displaySmall
                                  .override(
                                    font: GoogleFonts.interTight(
                                      fontWeight: FontWeight.bold,
                                      fontStyle: FlutterFlowTheme.of(context)
                                          .displaySmall
                                          .fontStyle,
                                    ),
                                    fontSize: 28,
                                    letterSpacing: 0.0,
                                    fontWeight: FontWeight.bold,
                                    fontStyle: FlutterFlowTheme.of(context)
                                        .displaySmall
                                        .fontStyle,
                                  ),
                            ),
                            Text(
                              'Connexion de l\'application Nexus à l\'ODB',
                              style: FlutterFlowTheme.of(context)
                                  .bodyMedium
                                  .override(
                                    font: GoogleFonts.inter(
                                      fontWeight: FlutterFlowTheme.of(context)
                                          .bodyMedium
                                          .fontWeight,
                                      fontStyle: FlutterFlowTheme.of(context)
                                          .bodyMedium
                                          .fontStyle,
                                    ),
                                    color: FlutterFlowTheme.of(context)
                                        .secondaryText,
                                    letterSpacing: 0.0,
                                    fontWeight: FlutterFlowTheme.of(context)
                                        .bodyMedium
                                        .fontWeight,
                                    fontStyle: FlutterFlowTheme.of(context)
                                        .bodyMedium
                                        .fontStyle,
                                  ),
                            ),
                          ],
                        ),
                        Icon(
                          Icons.settings,
                          color: FlutterFlowTheme.of(context).primaryText,
                          size: 24,
                        ),
                      ],
                    ),
                    Container(
                      width: double.infinity,
                      height: 450,
                      decoration: BoxDecoration(
                        color: FlutterFlowTheme.of(context).secondaryBackground,
                        boxShadow: const [
                          BoxShadow(
                            blurRadius: 4,
                            color: Color(0x33000000),
                            offset: Offset(
                              0,
                              2,
                            ),
                          )
                        ],
                        borderRadius: BorderRadius.circular(12),
                        border: Border.all(
                          color: Colors.transparent,
                          width: 1,
                        ),
                      ),
                      child: Padding(
                        padding: const EdgeInsets.all(16),
                        child: Column(
                          mainAxisSize: MainAxisSize.max,
                          children: [
                            Row(
                              mainAxisSize: MainAxisSize.max,
                              mainAxisAlignment: MainAxisAlignment.spaceBetween,
                              children: [
                                Row(
                                  mainAxisSize: MainAxisSize.max,
                                  children: [
                                    Icon(
                                      Icons.bluetooth,
                                      color:
                                          FlutterFlowTheme.of(context).primary,
                                      size: 24,
                                    ),
                                    Text(
                                      'Scan Bluetooth',
                                      style: FlutterFlowTheme.of(context)
                                          .titleMedium
                                          .override(
                                            font: GoogleFonts.interTight(
                                              fontWeight: FontWeight.w600,
                                              fontStyle:
                                                  FlutterFlowTheme.of(context)
                                                      .titleMedium
                                                      .fontStyle,
                                            ),
                                            letterSpacing: 0.0,
                                            fontWeight: FontWeight.w600,
                                            fontStyle:
                                                FlutterFlowTheme.of(context)
                                                    .titleMedium
                                                    .fontStyle,
                                          ),
                                    ),
                                  ].divide(const SizedBox(width: 8)),
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
                            Divider(
                              thickness: 1,
                              color: FlutterFlowTheme.of(context).alternate,
                            ),
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
                          ].divide(const SizedBox(height: 12)),
                        ),
                      ),
                    ),
                    Container(
                      width: double.infinity,
                      height: 125,
                      decoration: BoxDecoration(
                        color: FlutterFlowTheme.of(context).secondaryBackground,
                        boxShadow: const [
                          BoxShadow(
                            blurRadius: 4,
                            color: Color(0x33000000),
                            offset: Offset(
                              0,
                              2,
                            ),
                          )
                        ],
                        borderRadius: BorderRadius.circular(12),
                        border: Border.all(
                          color: Colors.transparent,
                          width: 1,
                        ),
                      ),
                      child: Padding(
                        padding: const EdgeInsets.all(16),
                        child: Column(
                          mainAxisSize: MainAxisSize.max,
                          children: [
                            Expanded(
                              child: ListView.builder(
                                reverse: false,
                                itemCount: bt.logs.length,
                                itemBuilder: (context, index) {
                                  final log = bt.logs[index];

                                  return Padding(
                                    padding: const EdgeInsets.symmetric(vertical: 2.0),
                                    child: Text(
                                      log,
                                      style: FlutterFlowTheme.of(context).bodySmall.override(
                                            fontFamily: 'monospace',
                                            fontSize: 12,
                                            color: FlutterFlowTheme.of(context).secondaryText,
                                          ),
                                    ),
                                  );
                                },
                              ),
                            ),
                          ].divide(const SizedBox(height: 12)),
                        ),
                      ),
                    ),
                  ]
                      .divide(const SizedBox(height: 24))
                      .addToStart(const SizedBox(height: 24))
                      .addToEnd(const SizedBox(height: 24)),
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
