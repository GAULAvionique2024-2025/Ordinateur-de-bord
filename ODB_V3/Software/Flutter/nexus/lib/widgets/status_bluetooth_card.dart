import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:nexus/flutter_flow/flutter_flow_theme.dart';
import 'package:provider/provider.dart';
import 'package:nexus/services/bluetooth_service.dart';

class StatusBluetoothCard extends StatelessWidget {
  final String title = 'Statut Bluetooth';
  final String? subtitle;
  final IconData? icon;
  final List<Color>? gradientColors;

  const StatusBluetoothCard({
    Key? key,
    this.subtitle,
    this.icon,
    this.gradientColors,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    final bt = context.watch<BluetoothServiceManager?>();
    final connectedDeviceName = bt?.connectedDevice?.platformName;
    final connected = connectedDeviceName != null;
    final isScanning = bt?.isScanning ?? false;

    final effectiveIcon = icon ??
        (connected
            ? Icons.bluetooth_connected
            : (isScanning ? Icons.bluetooth_searching : Icons.bluetooth_disabled));

    final iconColor = connected
        ? FlutterFlowTheme.of(context).success
        : (isScanning
            ? FlutterFlowTheme.of(context).warning
            : FlutterFlowTheme.of(context).secondaryText);

    final colors = gradientColors ??
        [FlutterFlowTheme.of(context).primary, FlutterFlowTheme.of(context).accent1];

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
          colors: colors,
          stops: const [0.0, 1.0],
          begin: const AlignmentDirectional(1.0, -1.0),
          end: const AlignmentDirectional(-1.0, 1.0),
        ),
        borderRadius: BorderRadius.circular(16.0),
      ),
      child: Padding(
        padding: const EdgeInsets.all(20.0),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  title,
                  style: FlutterFlowTheme.of(context).titleMedium.override(
                        font: GoogleFonts.interTight(
                          fontWeight: FontWeight.w600,
                          fontStyle: FlutterFlowTheme.of(context).titleMedium.fontStyle,
                        ),
                        color: FlutterFlowTheme.of(context).info,
                        letterSpacing: 0.0,
                        fontWeight: FontWeight.w600,
                      ),
                ),
                Padding(
                  padding: const EdgeInsetsDirectional.fromSTEB(0.0, 4.0, 0.0, 0.0),
                  child: Text(
                    connectedDeviceName ?? subtitle ?? 'Aucun appareil connecté',
                    style: FlutterFlowTheme.of(context).bodyMedium.override(
                          font: GoogleFonts.inter(),
                          color: FlutterFlowTheme.of(context).info,
                          letterSpacing: 0.0,
                        ),
                  ),
                ),
              ],
            ),
            Opacity(
              opacity: 0.9,
              child: Icon(
                effectiveIcon,
                color: iconColor,
                size: 32.0,
              ),
            ),
          ],
        ),
      ),
    );
  }
}