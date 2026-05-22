import '/flutter_flow/flutter_flow_theme.dart';
import '/flutter_flow/flutter_flow_util.dart';
import '/flutter_flow/flutter_flow_widgets.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:provider/provider.dart';
import 'commands_page_model.dart';
export 'commands_page_model.dart';
import 'package:nexus/services/bluetooth_service.dart';
import 'package:nexus/services/data_service.dart';
import 'package:nexus/services/console_service.dart';
import 'package:nexus/widgets/status_bluetooth_card.dart';


class CommandsPageWidget extends StatefulWidget {
  const CommandsPageWidget({super.key});

  static String routeName = 'CommandsPage';
  static String routePath = '/commandsPage';

  @override
  State<CommandsPageWidget> createState() => _CommandsPageWidgetState();
}

class _CommandsPageWidgetState extends State<CommandsPageWidget> {
  late CommandsPageModel _model;

  final scaffoldKey = GlobalKey<ScaffoldState>();

  @override
  void initState() {
    super.initState();
    _model = createModel(context, () => CommandsPageModel());
  }

  @override
  void dispose() {
    _model.dispose();

    super.dispose();
  }

  Widget _buildSectionTitle(BuildContext context, String title) {
    return Text(
      title,
      style: FlutterFlowTheme.of(context).titleLarge.override(
            font: GoogleFonts.interTight(
              fontWeight: FontWeight.bold,
              fontStyle: FlutterFlowTheme.of(context).titleLarge.fontStyle,
            ),
            letterSpacing: 0.0,
            fontWeight: FontWeight.bold,
            fontStyle: FlutterFlowTheme.of(context).titleLarge.fontStyle,
          ),
    );
  }

  Widget _buildSectionCard(BuildContext context, Widget child) {
    return Container(
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
        borderRadius: BorderRadius.circular(12.0),
        border: Border.all(
          color: FlutterFlowTheme.of(context).alternate,
          width: 1.0,
        ),
      ),
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: child,
      ),
    );
  }

  Widget _buildActionRow(
    BuildContext context, {
    required String title,
    required String description,
    required String buttonText,
    required Color buttonColor,
    required VoidCallback? onPressed,
    Color? buttonTextColor,
  }) {
    return Row(
      mainAxisSize: MainAxisSize.max,
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: [
        Expanded(
          child: Column(
            mainAxisSize: MainAxisSize.max,
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                title,
                style: FlutterFlowTheme.of(context).bodyMedium.override(
                      font: GoogleFonts.inter(
                        fontWeight: FontWeight.w600,
                        fontStyle:
                            FlutterFlowTheme.of(context).bodyMedium.fontStyle,
                      ),
                      fontSize: 16.0,
                      letterSpacing: 0.0,
                      fontWeight: FontWeight.w600,
                      fontStyle:
                          FlutterFlowTheme.of(context).bodyMedium.fontStyle,
                    ),
              ),
              Text(
                description,
                style: FlutterFlowTheme.of(context).bodySmall.override(
                      font: GoogleFonts.inter(
                        fontWeight:
                            FlutterFlowTheme.of(context).bodySmall.fontWeight,
                        fontStyle:
                            FlutterFlowTheme.of(context).bodySmall.fontStyle,
                      ),
                      color: FlutterFlowTheme.of(context).secondaryText,
                      fontSize: 12.0,
                      letterSpacing: 0.0,
                      fontWeight:
                          FlutterFlowTheme.of(context).bodySmall.fontWeight,
                      fontStyle:
                          FlutterFlowTheme.of(context).bodySmall.fontStyle,
                    ),
              ),
            ],
          ),
        ),
        FFButtonWidget(
          onPressed: onPressed,
          text: buttonText,
          options: FFButtonOptions(
            height: 36.0,
            padding: const EdgeInsetsDirectional.fromSTEB(16.0, 0.0, 16.0, 0.0),
            iconPadding: const EdgeInsetsDirectional.fromSTEB(0.0, 0.0, 0.0, 0.0),
            color: buttonColor,
            textStyle: FlutterFlowTheme.of(context).bodySmall.override(
                  font: GoogleFonts.inter(
                    fontWeight: FontWeight.w600,
                    fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                  ),
                  color: buttonTextColor ?? FlutterFlowTheme.of(context).primaryBackground,
                  letterSpacing: 0.0,
                  fontWeight: FontWeight.w600,
                  fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                ),
            elevation: 0.0,
            borderRadius: BorderRadius.circular(8.0),
          ),
        ),
      ],
    );
  }

  Widget _buildPyroTile(
    BuildContext context, {
    required String title,
    required String subtitle,
    required bool enabled,
    required VoidCallback? onPressed,
    required String buttonLabel,
  }) {
    return Container(
      width: 100.0,
      height: 100.0,
      decoration: BoxDecoration(
        color: FlutterFlowTheme.of(context).secondaryBackground,
        boxShadow: const [
          BoxShadow(
            blurRadius: 4.0,
            color: Color(0x33000000),
            offset: Offset(0.0, 2.0),
          )
        ],
        borderRadius: BorderRadius.circular(8.0),
        border: Border.all(
          color: Colors.transparent,
          width: 1.0,
        ),
      ),
      child: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          mainAxisSize: MainAxisSize.max,
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(
              title,
              style: FlutterFlowTheme.of(context).bodyMedium.override(
                    font: GoogleFonts.inter(
                      fontWeight: FontWeight.w600,
                      fontStyle: FlutterFlowTheme.of(context).bodyMedium.fontStyle,
                    ),
                    color: FlutterFlowTheme.of(context).primaryText,
                    fontSize: 14.0,
                    letterSpacing: 0.0,
                    fontWeight: FontWeight.w600,
                    fontStyle: FlutterFlowTheme.of(context).bodyMedium.fontStyle,
                  ),
            ),
            Text(
              subtitle,
              textAlign: TextAlign.center,
              style: FlutterFlowTheme.of(context).bodySmall.override(
                    font: GoogleFonts.inter(
                      fontWeight: FlutterFlowTheme.of(context).bodySmall.fontWeight,
                      fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                    ),
                    color: FlutterFlowTheme.of(context).secondaryText,
                    fontSize: 10.0,
                    letterSpacing: 0.0,
                    fontWeight: FlutterFlowTheme.of(context).bodySmall.fontWeight,
                    fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                  ),
            ),
            Padding(
              padding: const EdgeInsetsDirectional.fromSTEB(0.0, 4.0, 0.0, 0.0),
              child: FFButtonWidget(
                onPressed: onPressed,
                text: buttonLabel,
                options: FFButtonOptions(
                  height: 28.0,
                  padding: const EdgeInsetsDirectional.fromSTEB(12.0, 0.0, 12.0, 0.0),
                  iconPadding: const EdgeInsetsDirectional.fromSTEB(0.0, 0.0, 0.0, 0.0),
                  color: enabled ? FlutterFlowTheme.of(context).error : FlutterFlowTheme.of(context).secondaryText,
                  textStyle: FlutterFlowTheme.of(context).bodySmall.override(
                        font: GoogleFonts.inter(
                          fontWeight: FontWeight.w600,
                          fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                        ),
                        color: FlutterFlowTheme.of(context).primaryBackground,
                        fontSize: 10.0,
                        letterSpacing: 0.0,
                        fontWeight: FontWeight.w600,
                        fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                      ),
                  elevation: 0.0,
                  borderRadius: BorderRadius.circular(6.0),
                ),
              ),
            ),
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

    return GestureDetector(
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
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  const StatusBluetoothCard(),
                  Column(
                    mainAxisSize: MainAxisSize.max,
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        'Commandes',
                        style: FlutterFlowTheme.of(context).displaySmall.override(
                              font: GoogleFonts.interTight(
                                fontWeight: FontWeight.bold,
                                fontStyle: FlutterFlowTheme.of(context).displaySmall.fontStyle,
                              ),
                              fontSize: 28.0,
                              letterSpacing: 0.0,
                              fontWeight: FontWeight.bold,
                              fontStyle: FlutterFlowTheme.of(context).displaySmall.fontStyle,
                            ),
                      ),
                      Text(
                        'Commandes et tests des systèmes',
                        style: FlutterFlowTheme.of(context).bodyMedium.override(
                              font: GoogleFonts.inter(
                                fontWeight: FlutterFlowTheme.of(context).bodyMedium.fontWeight,
                                fontStyle: FlutterFlowTheme.of(context).bodyMedium.fontStyle,
                              ),
                              color: FlutterFlowTheme.of(context).secondaryText,
                              letterSpacing: 0.0,
                              fontWeight: FlutterFlowTheme.of(context).bodyMedium.fontWeight,
                              fontStyle: FlutterFlowTheme.of(context).bodyMedium.fontStyle,
                            ),
                      ),
                    ],
                  ),
                  _buildSectionTitle(context, 'Tests Unitaires'),
                  _buildSectionCard(
                    context,
                    Column(
                      mainAxisSize: MainAxisSize.max,
                      children: [
                        _buildActionRow(
                          context,
                          title: 'Ping',
                          description: 'Vérification de la connectivité',
                          buttonText: 'Exécuter',
                          buttonColor: connected ? FlutterFlowTheme.of(context).primary : FlutterFlowTheme.of(context).secondaryText,
                          onPressed: connected ? () {
                            ConsoleService().log('Test Ping demandé');
                            bt.send('PING\r\n');
                          } : null,
                        ),
                        _buildActionRow(
                          context,
                          title: 'Test Capteurs',
                          description: 'Vérification accéléromètre, gyroscope, baromètre',
                          buttonText: 'Exécuter',
                          buttonColor: connected ? FlutterFlowTheme.of(context).primary : FlutterFlowTheme.of(context).secondaryText,
                          onPressed: connected ? () {
                            ConsoleService().log('Test Capteurs demandé');
                            bt.send('TEST1\r\n');
                          } : null,
                        ),
                        _buildActionRow(
                          context,
                          title: 'Test Communication',
                          description: 'Vérification liaison radio et télémétrie',
                          buttonText: 'Exécuter',
                          buttonColor: connected ? FlutterFlowTheme.of(context).primary : FlutterFlowTheme.of(context).secondaryText,
                          onPressed: connected ? () {
                            ConsoleService().log('Test Communication demandé');
                            bt.send('TEST2\r\n');
                          } : null,
                        ),
                        _buildActionRow(
                          context,
                          title: 'Test Mémoire',
                          description: 'Vérification stockage et lecture données',
                          buttonText: 'Exécuter',
                          buttonColor: connected ? FlutterFlowTheme.of(context).primary : FlutterFlowTheme.of(context).secondaryText,
                          onPressed: connected ? () {
                            ConsoleService().log('Test Mémoire demandé');
                            bt.send('TEST3\r\n');
                          } : null,
                        ),
                      ].divide(const SizedBox(height: 12.0)),
                    ),
                  ),
                  _buildSectionTitle(context, 'Calibration'),
                  _buildSectionCard(
                    context,
                    Column(
                      mainAxisSize: MainAxisSize.max,
                      children: [
                        _buildActionRow(
                          context,
                          title: 'Calibration Accéléromètre',
                          description: 'Calibrer les axes X, Y, Z',
                          buttonText: 'Calibrer',
                          buttonColor: connected ? FlutterFlowTheme.of(context).tertiary : FlutterFlowTheme.of(context).secondaryText,
                          onPressed: connected ? () {
                            ConsoleService().log('Calibration Accéléromètre demandé');
                            bt.send('CALIB1\r\n');
                          } : null,
                        ),
                        _buildActionRow(
                          context,
                          title: 'Calibration Baromètre',
                          description: 'Réglage altitude de référence',
                          buttonText: 'Calibrer',
                          buttonColor: connected ? FlutterFlowTheme.of(context).tertiary : FlutterFlowTheme.of(context).secondaryText,
                          onPressed: connected ? () {
                            ConsoleService().log('Calibration Baromètre demandé');
                            bt.send('CALIB2\r\n');
                          } : null,
                        ),
                      ].divide(const SizedBox(height: 12.0)),
                    ),
                  ),
                  _buildSectionTitle(context, 'Déclenchement Pyrotechnique'),
                  _buildSectionCard(
                    context,
                    Column(
                      mainAxisSize: MainAxisSize.max,
                      children: [
                        Align(
                          alignment: const AlignmentDirectional(0.0, 0.0),
                          child: Padding(
                            padding: const EdgeInsets.all(12.0),
                            child: FFButtonWidget(
                              onPressed: connected ? () {
                                ConsoleService().log('Verrouiller demandé');
                                bt.send(data.pyrosArmed ? 'ARM0\r\n' : 'ARM1\r\n');
                              } : null,
                              text: (connected && data.pyrosArmed) ? 'Armé' : 'Désarmé',
                              options: FFButtonOptions(
                                width: 150.0,
                                height: 48.0,
                                padding: const EdgeInsetsDirectional.fromSTEB(16.0, 0.0, 16.0, 0.0),
                                iconPadding: const EdgeInsetsDirectional.fromSTEB(0.0, 0.0, 0.0, 0.0),
                                color: connected ? FlutterFlowTheme.of(context).error : FlutterFlowTheme.of(context).secondaryText,
                                textStyle: FlutterFlowTheme.of(context).bodySmall.override(
                                      font: GoogleFonts.inter(
                                        fontWeight: FontWeight.w600,
                                        fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                                      ),
                                      color: FlutterFlowTheme.of(context).primaryBackground,
                                      letterSpacing: 0.0,
                                      fontWeight: FontWeight.w600,
                                      fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                                    ),
                                elevation: 0.0,
                                borderRadius: BorderRadius.circular(8.0),
                              ),
                            ),
                          ),
                        ),
                        GridView(
                          padding: EdgeInsets.zero,
                          gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                            crossAxisCount: 2,
                            crossAxisSpacing: 12.0,
                            mainAxisSpacing: 12.0,
                            childAspectRatio: 1.0,
                          ),
                          shrinkWrap: true,
                          scrollDirection: Axis.vertical,
                          children: [
                            _buildPyroTile(
                              context,
                              title: 'Pyro 1',
                              subtitle: 'Main Parachute',
                              enabled: connected && data.pyrosArmed && data.pyros[0],
                              buttonLabel: connected ? ((data.pyrosArmed && data.pyros[0]) ? 'Déclencher' : 'Désarmé / Déconnecté') : 'Inconnu',
                              onPressed: (connected && data.pyrosArmed && data.pyros[0]) ? () {
                                ConsoleService().log('Déclenchement pyro 1 demandé');
                                bt.send('P1\r\n');
                              } : null,
                            ),
                            _buildPyroTile(
                              context,
                              title: 'Pyro 2',
                              subtitle: 'Drogue Parachute',
                              enabled: connected && data.pyrosArmed && data.pyros[1],
                              buttonLabel: connected ? ((data.pyrosArmed && data.pyros[1]) ? 'Déclencher' : 'Désarmé / Déconnecté') : 'Inconnu',
                              onPressed: (connected && data.pyrosArmed && data.pyros[1]) ? () {
                                ConsoleService().log('Déclenchement pyro 2 demandé');
                                bt.send('P2\r\n');
                              } : null,
                            ),
                            _buildPyroTile(
                              context,
                              title: 'Pyro 3',
                              subtitle: 'Auxilliaire',
                              enabled: connected && data.pyrosArmed && data.pyros[2],
                              buttonLabel: connected ? ((data.pyrosArmed && data.pyros[2]) ? 'Déclencher' : 'Désarmé / Déconnecté') : 'Inconnu',
                              onPressed: (connected && data.pyrosArmed && data.pyros[2]) ? () {
                                ConsoleService().log('Déclenchement pyro 3 demandé');
                                bt.send('P3\r\n');
                              } : null,
                            ),
                            _buildPyroTile(
                              context,
                              title: 'Pyro 4',
                              subtitle: 'Auxilliaire',
                              enabled: connected && data.pyrosArmed && data.pyros[3],
                              buttonLabel: connected ? ((data.pyrosArmed && data.pyros[3]) ? 'Déclencher' : 'Désarmé / Déconnecté') : 'Inconnu',
                              onPressed: (connected && data.pyrosArmed && data.pyros[3]) ? () {
                                ConsoleService().log('Déclenchement pyro 4 demandé');
                                bt.send('P4\r\n');
                              } : null,
                            ),
                          ],
                        ),
                      ].divide(const SizedBox(height: 12.0)),
                    ),
                  ),
                  _buildSectionCard(
                    context,
                    SizedBox(
                      height: 168,
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
                        ],
                      ),
                    ),
                  ),
                ]
                    .divide(const SizedBox(height: 16.0))
                    .addToStart(const SizedBox(height: 16.0))
                    .addToEnd(const SizedBox(height: 24.0)),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
