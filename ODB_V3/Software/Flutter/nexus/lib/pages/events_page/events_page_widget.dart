import '/flutter_flow/flutter_flow_theme.dart';
import '/flutter_flow/flutter_flow_util.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:provider/provider.dart';
import 'events_page_model.dart';
export 'events_page_model.dart';
import 'package:nexus/services/data_service.dart';

class EventsPageWidget extends StatefulWidget {
  const EventsPageWidget({super.key});

  static String routeName = 'EventsPage';
  static String routePath = '/eventsPage';

  @override
  State<EventsPageWidget> createState() => _EventsPageWidgetState();
}

class _EventsPageWidgetState extends State<EventsPageWidget> {
  late EventsPageModel _model;
  final scaffoldKey = GlobalKey<ScaffoldState>();

  @override
  void initState() {
    super.initState();
    _model = createModel(context, () => EventsPageModel());
  }

  @override
  void dispose() {
    _model.dispose();
    super.dispose();
  }

  String _formatTimeMs(int timeMs) {
    if (timeMs == 0) return '—';
    return '${(timeMs / 1000.0).toStringAsFixed(2)} s';
  }

  @override
  Widget build(BuildContext context) {
    final data = context.watch<DataServiceManager>();
    final connected = data.hasConnection;
    final stats = connected ? data.lastFlightStats : null;

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
                            'Événements du vol',
                            style: FlutterFlowTheme.of(context).displaySmall.override(
                                  font: GoogleFonts.interTight(fontWeight: FontWeight.bold),
                                  fontSize: 28.0,
                                ),
                          ),
                          Text(
                            'Données sauvegardées du dernier vol',
                            style: FlutterFlowTheme.of(context).bodyMedium.override(
                                  font: GoogleFonts.inter(),
                                  color: FlutterFlowTheme.of(context).secondaryText,
                                ),
                          ),
                        ],
                      ),
                      Container(
                        decoration: BoxDecoration(
                          color: connected ? FlutterFlowTheme.of(context).primary : FlutterFlowTheme.of(context).secondaryText,
                          borderRadius: BorderRadius.circular(8.0),
                        ),
                        child: IconButton(
                          icon: const Icon(Icons.download_rounded, color: Colors.white),
                          onPressed: connected ? () => data.requestLastFlightEvents() : null,
                          tooltip: 'Télécharger les données',
                        ),
                      ),
                    ],
                  ),
                  const SizedBox(height: 24.0),

                  if (stats == null)
                    Center(
                      child: Padding(
                        padding: const EdgeInsets.all(32.0),
                        child: Text(
                          'Aucune donnée récupérée. Appuyez sur le bouton de téléchargement pour interroger l\'ODB.',
                          textAlign: TextAlign.center,
                          style: FlutterFlowTheme.of(context).bodyMedium.override(
                                font: GoogleFonts.inter(),
                                color: FlutterFlowTheme.of(context).secondaryText,
                              ),
                        ),
                      ),
                    ),

                  if (stats != null) ...[
                    _buildSectionCard(
                      context,
                      'Altitudes Max',
                      Icons.terrain,
                      [
                        _buildDataRow(context, 'Kalman', stats.maxAltitudeKalman.valid ? '${stats.maxAltitudeKalman.value.toStringAsFixed(1)} m' : '—', _formatTimeMs(stats.maxAltitudeKalman.timeMs)),
                        _buildDataRow(context, 'Baromètre', stats.maxAltitudeBaro.valid ? '${stats.maxAltitudeBaro.value.toStringAsFixed(1)} m' : '—', _formatTimeMs(stats.maxAltitudeBaro.timeMs)),
                        _buildDataRow(context, 'GPS', stats.maxAltitudeGps.valid ? '${stats.maxAltitudeGps.value.toStringAsFixed(1)} m' : '—', _formatTimeMs(stats.maxAltitudeGps.timeMs)),
                        _buildDataRow(context, 'Apogée Détectée', stats.apogee.valid ? '${stats.apogee.value.toStringAsFixed(1)} m' : '—', _formatTimeMs(stats.apogee.timeMs)),
                      ],
                    ),
                    const SizedBox(height: 16.0),
                    _buildSectionCard(
                      context,
                      'Cinématique Max',
                      Icons.speed,
                      [
                        _buildDataRow(context, 'Vitesse Ascendante', stats.maxAscendSpeed.valid ? '${stats.maxAscendSpeed.value.toStringAsFixed(1)} m/s' : '—', _formatTimeMs(stats.maxAscendSpeed.timeMs)),
                        _buildDataRow(context, 'Vitesse Descendante', stats.maxDescendSpeed.valid ? '${stats.maxDescendSpeed.value.toStringAsFixed(1)} m/s' : '—', _formatTimeMs(stats.maxDescendSpeed.timeMs)),
                        _buildDataRow(context, 'Accélération Ascendante', stats.maxAscendAccel.valid ? '${stats.maxAscendAccel.value.toStringAsFixed(1)} m/s²' : '—', _formatTimeMs(stats.maxAscendAccel.timeMs)),
                        _buildDataRow(context, 'Accélération Descendante', stats.maxDescendAccel.valid ? '${stats.maxDescendAccel.value.toStringAsFixed(1)} m/s²' : '—', _formatTimeMs(stats.maxDescendAccel.timeMs)),
                      ],
                    ),
                    const SizedBox(height: 16.0),
                    _buildSectionCard(
                      context,
                      'Déploiements & Pyros',
                      Icons.local_fire_department,
                      [
                        _buildDataRow(context, 'Déploiement Drogue', stats.drogueDeploy.valid ? '${stats.drogueDeploy.value.toStringAsFixed(1)} m' : '—', _formatTimeMs(stats.drogueDeploy.timeMs)),
                        _buildDataRow(context, 'Déploiement Principal', stats.mainDeploy.valid ? '${stats.mainDeploy.value.toStringAsFixed(1)} m' : '—', _formatTimeMs(stats.mainDeploy.timeMs)),
                        _buildDataRow(context, 'Pyro 1 (Main)', stats.pyroEvents[0].fired ? 'Déclenché' : '—', _formatTimeMs(stats.pyroEvents[0].timeMs)),
                        _buildDataRow(context, 'Pyro 2 (Drogue)', stats.pyroEvents[1].fired ? 'Déclenché' : '—', _formatTimeMs(stats.pyroEvents[1].timeMs)),
                        _buildDataRow(context, 'Pyro 3 (Main #2)', stats.pyroEvents[2].fired ? 'Déclenché' : '—', _formatTimeMs(stats.pyroEvents[2].timeMs)),
                        _buildDataRow(context, 'Pyro 4 (Drogue #2)', stats.pyroEvents[3].fired ? 'Déclenché' : '—', _formatTimeMs(stats.pyroEvents[3].timeMs)),
                      ],
                    ),
                    const SizedBox(height: 16.0),
                    _buildSectionCard(
                      context,
                      'Informations Générales',
                      Icons.info_outline,
                      [
                        _buildDataRow(context, 'Temps de Vol Total', _formatTimeMs(stats.flightTimeMs), ''),
                        _buildDataRow(context, 'Dernière Latitude', '${(stats.lastLat / 10000000.0).toStringAsFixed(5)}°', ''),
                        _buildDataRow(context, 'Dernière Longitude', '${(stats.lastLon / 10000000.0).toStringAsFixed(5)}°', ''),
                      ],
                    ),
                    const SizedBox(height: 24.0),
                  ],
                ],
              ),
            ),
          ),
        ),
      )
    );
    }

    Widget _buildSectionCard(BuildContext context, String title, IconData icon, List<Widget> rows) {
      return Material(
        color: Colors.transparent,
        elevation: 2.0,
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16.0)),
        child: Container(
          width: double.infinity,
          decoration: BoxDecoration(
            color: FlutterFlowTheme.of(context).secondaryBackground,
            borderRadius: BorderRadius.circular(16.0),
            border: Border.all(color: FlutterFlowTheme.of(context).alternate, width: 1.0),
          ),
          child: Padding(
            padding: const EdgeInsets.all(16.0),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Row(
                  children: [
                    Icon(icon, color: FlutterFlowTheme.of(context).primary, size: 24.0),
                    const SizedBox(width: 12.0),
                    Text(
                      title,
                      style: FlutterFlowTheme.of(context).titleMedium.override(
                            font: GoogleFonts.interTight(fontWeight: FontWeight.bold),
                          ),
                    ),
                  ],
                ),
                const Divider(height: 24.0, thickness: 1.0, color: Color(0xFFE0E3E7)),
                ...rows.divide(const SizedBox(height: 12.0)),
              ],
            ),
          ),
        ),
      );
    }

    Widget _buildDataRow(BuildContext context, String label, String value, String time) {
      return Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          Expanded(
            flex: 2,
            child: Text(
              label,
              style: FlutterFlowTheme.of(context).bodyMedium.override(
                    font: GoogleFonts.inter(),
                    color: FlutterFlowTheme.of(context).secondaryText,
                  ),
            ),
          ),
          Expanded(
            flex: 1,
            child: Text(
              value,
              textAlign: TextAlign.right,
              style: FlutterFlowTheme.of(context).bodyMedium.override(
                    font: GoogleFonts.inter(fontWeight: FontWeight.w600),
                  ),
            ),
          ),
          Expanded(
            flex: 1,
            child: Text(
              time,
              textAlign: TextAlign.right,
              style: FlutterFlowTheme.of(context).bodySmall.override(
                    font: GoogleFonts.inter(),
                    color: FlutterFlowTheme.of(context).tertiary,
                  ),
            ),
          ),
        ],
      );
    }
}