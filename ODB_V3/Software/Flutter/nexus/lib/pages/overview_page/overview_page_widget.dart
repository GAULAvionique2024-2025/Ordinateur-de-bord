import '/flutter_flow/flutter_flow_theme.dart';
import '/flutter_flow/flutter_flow_util.dart';
import '/flutter_flow/flutter_flow_widgets.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:provider/provider.dart';
import 'overview_page_model.dart';
export 'overview_page_model.dart';
import 'package:nexus/services/bluetooth_service.dart';
import 'package:nexus/services/data_service.dart';
import 'package:nexus/widgets/status_bluetooth_card.dart';

/// Créer un homepage.
///
/// cette page me permet de me connecter par bluetooth à l'ordinateur de bord.
/// Affiche dans le carte l'appareil connecté (information bluettoth) si
/// connecté ou non. Un gros bouton plus me permette de commencer une
/// connexion bluetooth avec un appareil. Pour le reste ajoute ce que tu
/// penses intéressant à retrouver sur une page overview pour mon ordinateur
/// de bord de fusée
class OverviewPageWidget extends StatefulWidget {
  const OverviewPageWidget({super.key});

  static String routeName = 'OverviewPage';
  static String routePath = '/overviewPage';

  @override
  State<OverviewPageWidget> createState() => _OverviewPageWidgetState();
}

class _OverviewPageWidgetState extends State<OverviewPageWidget> {
  late OverviewPageModel _model;

  final scaffoldKey = GlobalKey<ScaffoldState>();

  int _preflightStep = 0;

  @override
  void initState() {
    super.initState();
    _model = createModel(context, () => OverviewPageModel());
  }

  @override
  void dispose() {
    _model.dispose();

    super.dispose();
  }

  Widget _buildMetricCard(
    BuildContext context, {
    required IconData icon,
    required Color iconColor,
    required String title,
    required String value,
  }) {
    return Expanded(
      child: Container(
        width: 100.0,
        decoration: BoxDecoration(
          color: FlutterFlowTheme.of(context).secondaryBackground,
          boxShadow: const [
            BoxShadow(
              blurRadius: 4.0,
              color: Color(0x33000000),
              offset: Offset(0.0, 2.0),
            )
          ],
          borderRadius: BorderRadius.circular(12.0),
        ),
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            mainAxisSize: MainAxisSize.max,
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Icon(icon, color: iconColor, size: 32.0),
              Padding(
                padding:
                    const EdgeInsetsDirectional.fromSTEB(0.0, 8.0, 0.0, 4.0),
                child: Text(
                  title,
                  style: FlutterFlowTheme.of(context).titleSmall.override(
                        font: GoogleFonts.interTight(
                          fontWeight: FontWeight.w600,
                          fontStyle:
                              FlutterFlowTheme.of(context).titleSmall.fontStyle,
                        ),
                        letterSpacing: 0.0,
                        fontWeight: FontWeight.w600,
                        fontStyle:
                            FlutterFlowTheme.of(context).titleSmall.fontStyle,
                      ),
                ),
              ),
              Text(
                value,
                style: FlutterFlowTheme.of(context).headlineSmall.override(
                      font: GoogleFonts.interTight(
                        fontWeight: FontWeight.bold,
                        fontStyle: FlutterFlowTheme.of(context)
                            .headlineSmall
                            .fontStyle,
                      ),
                      color: iconColor,
                      letterSpacing: 0.0,
                      fontWeight: FontWeight.bold,
                      fontStyle:
                          FlutterFlowTheme.of(context).headlineSmall.fontStyle,
                    ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildStatusRow(
    BuildContext context, {
    required String label,
    required bool isActive,
    required String activeText,
    required String inactiveText,
    required Color activeColor,
  }) {
    return Padding(
      padding: const EdgeInsetsDirectional.fromSTEB(0.0, 0.0, 0.0, 8.0),
      child: Row(
        mainAxisSize: MainAxisSize.max,
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          Text(
            label,
            style: FlutterFlowTheme.of(context).bodyMedium.override(
                  font: GoogleFonts.inter(
                    fontWeight:
                        FlutterFlowTheme.of(context).bodyMedium.fontWeight,
                    fontStyle:
                        FlutterFlowTheme.of(context).bodyMedium.fontStyle,
                  ),
                  letterSpacing: 0.0,
                  fontWeight:
                      FlutterFlowTheme.of(context).bodyMedium.fontWeight,
                  fontStyle: FlutterFlowTheme.of(context).bodyMedium.fontStyle,
                ),
          ),
          Row(
            mainAxisSize: MainAxisSize.max,
            children: [
              Container(
                width: 8.0,
                height: 8.0,
                decoration: BoxDecoration(
                  color: isActive
                      ? activeColor
                      : FlutterFlowTheme.of(context).secondaryText,
                  shape: BoxShape.circle,
                ),
              ),
              Text(
                isActive ? activeText : inactiveText,
                style: FlutterFlowTheme.of(context).bodySmall.override(
                      font: GoogleFonts.inter(
                        fontWeight: FontWeight.w600,
                        fontStyle:
                            FlutterFlowTheme.of(context).bodySmall.fontStyle,
                      ),
                      color: isActive
                          ? activeColor
                          : FlutterFlowTheme.of(context).secondaryText,
                      letterSpacing: 0.0,
                      fontWeight: FontWeight.w600,
                      fontStyle:
                          FlutterFlowTheme.of(context).bodySmall.fontStyle,
                    ),
              ),
            ].divide(const SizedBox(width: 8.0)),
          ),
        ],
      ),
    );
  }

  Widget _buildMissionCard(
    BuildContext context, {
    required bool connected,
    required BluetoothServiceManager bt,
    required DataServiceManager data,
  }) {
    return Container(
      width: double.infinity,
      decoration: BoxDecoration(
        boxShadow: const [
          BoxShadow(
            blurRadius: 4.0,
            color: Color(0x33000000),
            offset: Offset(0.0, 2.0),
          )
        ],
        gradient: LinearGradient(
          colors: [
            FlutterFlowTheme.of(context).primary,
            FlutterFlowTheme.of(context).accent1,
          ],
          stops: const [0.0, 1.0],
          begin: const AlignmentDirectional(1.0, -1.0),
          end: const AlignmentDirectional(-1.0, 1.0),
        ),
        borderRadius: BorderRadius.circular(16.0),
      ),
      child: Padding(
        padding: const EdgeInsets.all(20.0),
        child: Column(
          mainAxisSize: MainAxisSize.max,
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
                      'Statut de mission',
                      style: FlutterFlowTheme.of(context).titleMedium.override(
                            font: GoogleFonts.interTight(
                              fontWeight: FontWeight.w600,
                              fontStyle: FlutterFlowTheme.of(context)
                                  .titleMedium
                                  .fontStyle,
                            ),
                            color: Colors.white,
                            letterSpacing: 0.0,
                            fontWeight: FontWeight.w600,
                            fontStyle: FlutterFlowTheme.of(context)
                                .titleMedium
                                .fontStyle,
                          ),
                    ),
                    Padding(
                      padding: const EdgeInsetsDirectional.fromSTEB(
                          0.0, 4.0, 0.0, 0.0),
                      child: Text(
                        connected
                            ? 'Appareil connecté: ${bt.connectedDevice?.platformName ?? bt.connectedDevice?.remoteId.str}\nMission: ${data.missionStatus}\n${data.timeBootFormatted}'
                            : 'En attente de connexion Bluetooth',
                        style: FlutterFlowTheme.of(context).bodyMedium.override(
                              font: GoogleFonts.inter(
                                fontWeight: FlutterFlowTheme.of(context)
                                    .bodyMedium
                                    .fontWeight,
                                fontStyle: FlutterFlowTheme.of(context)
                                    .bodyMedium
                                    .fontStyle,
                              ),
                              color: Colors.white,
                              letterSpacing: 0.0,
                              fontWeight: FlutterFlowTheme.of(context)
                                  .bodyMedium
                                  .fontWeight,
                              fontStyle: FlutterFlowTheme.of(context)
                                  .bodyMedium
                                  .fontStyle,
                            ),
                      ),
                    ),
                  ],
                ),
              ],
            ),
            Padding(
              padding:
                  const EdgeInsetsDirectional.fromSTEB(0.0, 16.0, 0.0, 0.0),
              child: Row(
                mainAxisSize: MainAxisSize.max,
                children: [
                  Expanded(
                    child: Container(
                      height: 40.0,
                      decoration: BoxDecoration(
                        color: const Color(0x40FFFFFF),
                        borderRadius: BorderRadius.circular(20.0),
                      ),
                      child: Align(
                        alignment: const AlignmentDirectional(0.0, 0.0),
                        child: Padding(
                          padding: const EdgeInsets.symmetric(horizontal: 12.0),
                          child: Text(
                            connected
                                ? 'Appareil connecté: ${bt.connectedDevice?.platformName ?? bt.connectedDevice?.remoteId.str}'
                                : 'Bluetooth déconnecté',
                            style:
                                FlutterFlowTheme.of(context).bodySmall.override(
                                      font: GoogleFonts.inter(
                                        fontWeight: FontWeight.w600,
                                        fontStyle: FlutterFlowTheme.of(context)
                                            .bodySmall
                                            .fontStyle,
                                      ),
                                      color: Colors.white,
                                      letterSpacing: 0.0,
                                      fontWeight: FontWeight.w600,
                                      fontStyle: FlutterFlowTheme.of(context)
                                          .bodySmall
                                          .fontStyle,
                                    ),
                          ),
                        ),
                      ),
                    ),
                  ),
                ].divide(const SizedBox(width: 12.0)),
              ),
            ),
            Padding(
              padding: const EdgeInsetsDirectional.fromSTEB(0.0, 12.0, 0.0, 0.0),
              child: Align(
                alignment: const AlignmentDirectional(0.0, 0.0),
                child: FFButtonWidget(
                  // Le bouton est inactif si non connecté, ou si l'étape 3 est atteinte (Prêt)
                  onPressed: connected && _preflightStep < 3
                      ? () async {
                          if (_preflightStep == 0) {
                            await data.testArmingModule();
                            setState(() {
                              _preflightStep = 1;
                            });
                          } else if (_preflightStep == 1) {
                            await data.testPyrosContinuity(); 
                            setState(() {
                              _preflightStep = 2;
                            });
                          } else if (_preflightStep == 2) {
                            await data.setReadyFlight();
                            setState(() {
                              _preflightStep = 3;
                            });
                          }
                        }
                      : null,
                  text: _preflightStep == 0
                      ? 'Tester le module d\'armement'
                      : _preflightStep == 1
                          ? 'Tester la continuité pyros'
                          : _preflightStep == 2
                              ? 'Activer la mise en départ'
                              : '🚀 Prêt pour le vol',
                  options: FFButtonOptions(
                    height: 60.0,
                    padding: const EdgeInsetsDirectional.fromSTEB(24.0, 0.0, 24.0, 0.0),
                    iconPadding: const EdgeInsetsDirectional.fromSTEB(0.0, 0.0, 0.0, 0.0),
                    color: _preflightStep == 3 
                        ? const Color(0x60FFFFFF)
                        : Colors.transparent,
                    borderSide: BorderSide(
                      color: connected ? Colors.white : const Color(0x40FFFFFF),
                      width: 2.0,
                    ),
                    textStyle: FlutterFlowTheme.of(context).bodySmall.override(
                          font: GoogleFonts.inter(
                            fontWeight: FontWeight.w600,
                            fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                          ),
                          color: Colors.white,
                          letterSpacing: 0.0,
                          fontWeight: FontWeight.w600,
                          fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                        ),
                    elevation: 0.0,
                    borderRadius: BorderRadius.circular(25.0),
                  ),
                ),
              ),
            )
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final bt = context.watch<BluetoothServiceManager>();
    final data = context.watch<DataServiceManager>();
    final connected = data.hasConnection;

    if (!connected && _preflightStep != 0) {
      WidgetsBinding.instance.addPostFrameCallback((_) {
        setState(() {
          _preflightStep = 0;
        });
      });
    }

    return GestureDetector(
      onTap: () {
        FocusScope.of(context).unfocus();
        FocusManager.instance.primaryFocus?.unfocus();
      },
      child: Scaffold(
        key: scaffoldKey,
        backgroundColor: FlutterFlowTheme.of(context).primaryBackground,
        body: SafeArea(
          top: true,
          child: Padding(
            padding: const EdgeInsetsDirectional.fromSTEB(16.0, 0.0, 16.0, 0.0),
            child: SingleChildScrollView(
              child: Column(
                mainAxisSize: MainAxisSize.max,
                children: [
                  const StatusBluetoothCard(),
                  Row(
                    mainAxisSize: MainAxisSize.max,
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Column(
                        mainAxisSize: MainAxisSize.max,
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Overview',
                            style: FlutterFlowTheme.of(context)
                                .displaySmall
                                .override(
                                  font: GoogleFonts.interTight(
                                    fontWeight: FontWeight.bold,
                                    fontStyle: FlutterFlowTheme.of(context)
                                        .displaySmall
                                        .fontStyle,
                                  ),
                                  fontSize: 28.0,
                                  letterSpacing: 0.0,
                                  fontWeight: FontWeight.bold,
                                  fontStyle: FlutterFlowTheme.of(context)
                                      .displaySmall
                                      .fontStyle,
                                ),
                          ),
                          Text(
                            'Dashboard de mission',
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
                    ],
                  ),
                  Row(
                    mainAxisSize: MainAxisSize.max,
                    children: [
                      _buildMetricCard(
                        context,
                        icon: Icons.speed,
                        iconColor: FlutterFlowTheme.of(context).primary,
                        title: 'Altitude',
                        value: connected ? data.altitudeDisplay : '—',
                      ),
                      _buildMetricCard(
                        context,
                        icon: Icons.thermostat,
                        iconColor: FlutterFlowTheme.of(context).tertiary,
                        title: 'Température',
                        value: connected ? data.temperatureDisplay : '—',
                      ),
                    ].divide(const SizedBox(width: 16.0)),
                  ),
                  Container(
                    width: double.infinity,
                    decoration: BoxDecoration(
                      color: FlutterFlowTheme.of(context).secondaryBackground,
                      boxShadow: const [
                        BoxShadow(
                          blurRadius: 4.0,
                          color: Color(0x33000000),
                          offset: Offset(0.0, 2.0),
                        )
                      ],
                      borderRadius: BorderRadius.circular(16.0),
                    ),
                    child: Padding(
                      padding: const EdgeInsets.all(20.0),
                      child: Column(
                        mainAxisSize: MainAxisSize.max,
                        children: [
                          Padding(
                            padding: const EdgeInsetsDirectional.fromSTEB(
                                0.0, 0.0, 0.0, 16.0),
                            child: Text(
                              'Statuts Système',
                              style: FlutterFlowTheme.of(context)
                                  .titleMedium
                                  .override(
                                    font: GoogleFonts.interTight(
                                      fontWeight: FontWeight.w600,
                                      fontStyle: FlutterFlowTheme.of(context)
                                          .titleMedium
                                          .fontStyle,
                                    ),
                                    letterSpacing: 0.0,
                                    fontWeight: FontWeight.w600,
                                    fontStyle: FlutterFlowTheme.of(context)
                                        .titleMedium
                                        .fontStyle,
                                  ),
                            ),
                          ),
                          _buildStatusRow(
                            context,
                            label: 'Ordinateur de vol',
                            isActive: connected,
                            activeText: 'En ligne',
                            inactiveText: 'Hors ligne',
                            activeColor: FlutterFlowTheme.of(context).success,
                          ),
                          _buildStatusRow(
                            context,
                            label: 'Réseau de capteurs',
                            isActive: connected && data.odbSensorState,
                            activeText: 'Actif',
                            inactiveText:
                                connected ? 'Inactif / Partiel' : 'Inactif',
                            activeColor: connected
                                ? (data.odbSensorState
                                    ? FlutterFlowTheme.of(context).success
                                    : FlutterFlowTheme.of(context).warning)
                                : FlutterFlowTheme.of(context).secondaryText,
                          ),
                          _buildStatusRow(
                            context,
                            label: 'Connexion à la télémétrie',
                            isActive: connected &&
                                data.radioState == RadioState.connected,
                            activeText: 'Actif',
                            inactiveText: 'Inactif',
                            activeColor: FlutterFlowTheme.of(context).success,
                          ),
                          _buildStatusRow(
                            context,
                            label: 'Niveau de batterie',
                            isActive: connected &&
                                data.batterySensorState == SensorState.ok,
                            activeText:
                                '${data.batteryPercent.toStringAsFixed(1)}%',
                            inactiveText: '—',
                            activeColor: data.batteryPercent > 60
                                ? FlutterFlowTheme.of(context).primary
                                : (data.batteryPercent > 30
                                    ? FlutterFlowTheme.of(context).warning
                                    : FlutterFlowTheme.of(context).error),
                          ),
                        ],
                      ),
                    ),
                  ),
                  _buildMissionCard(
                    context,
                    connected: connected,
                    bt: bt,
                    data: data,
                  ),
                ]
                    .divide(const SizedBox(height: 24.0))
                    .addToStart(const SizedBox(height: 24.0))
                    .addToEnd(const SizedBox(height: 24.0)),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
