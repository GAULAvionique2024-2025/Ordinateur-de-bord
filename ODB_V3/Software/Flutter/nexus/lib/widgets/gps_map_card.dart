import 'package:flutter/material.dart';
import 'package:flutter_map/flutter_map.dart';
import 'package:google_fonts/google_fonts.dart';
import 'package:latlong2/latlong.dart' as latlong;
import 'package:provider/provider.dart';

import '/flutter_flow/flutter_flow_theme.dart';
import 'package:nexus/services/data_service.dart';
import 'package:nexus/services/location_service.dart';

class GpsMapCard extends StatefulWidget {
  const GpsMapCard({super.key});

  @override
  State<GpsMapCard> createState() => _GpsMapCardState();
}

class _GpsMapCardState extends State<GpsMapCard> {
  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance.addPostFrameCallback((_) {
      if (!mounted) return;
      context.read<LocationServiceManager>().refreshPhoneLocation();
    });
  }

  latlong.LatLng? _getOdbLocation(DataServiceManager data, bool connected) {
    if (!connected || !data.hasValidGpsFix) {
      return null;
    }

    return latlong.LatLng(data.gpsLat, data.gpsLon);
  }

  latlong.LatLng _getMapCenter(
    latlong.LatLng? phoneLocation,
    latlong.LatLng? odbLocation,
  ) {
    if (phoneLocation != null && odbLocation != null) {
      return latlong.LatLng(
        (phoneLocation.latitude + odbLocation.latitude) / 2,
        (phoneLocation.longitude + odbLocation.longitude) / 2,
      );
    }

    return phoneLocation ?? odbLocation ?? const latlong.LatLng(0.0, 0.0);
  }

  Widget _buildLocationMarker({
    required BuildContext context,
    required IconData icon,
    required Color color,
    required String label,
  }) {
    return Column(
      mainAxisSize: MainAxisSize.min,
      children: [
        Icon(icon, color: color, size: 34),
        Text(
          label,
          style: FlutterFlowTheme.of(context).bodySmall.override(
                font: GoogleFonts.inter(
                  fontWeight: FontWeight.w600,
                  fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
                ),
                color: color,
                letterSpacing: 0.0,
                fontWeight: FontWeight.w600,
                fontStyle: FlutterFlowTheme.of(context).bodySmall.fontStyle,
              ),
        ),
      ],
    );
  }

  @override
  Widget build(BuildContext context) {
    final data = context.watch<DataServiceManager>();
    final location = context.watch<LocationServiceManager>();
    final connected = data.hasConnection;
    final phoneLocation = location.phoneLocation;
    final odbLocation = _getOdbLocation(data, connected);
    final hasAnyLocation = phoneLocation != null || odbLocation != null;
    final mapCenter = _getMapCenter(phoneLocation, odbLocation);

    final markers = <Marker>[];
    if (phoneLocation != null) {
      markers.add(
        Marker(
          point: phoneLocation,
          width: 140,
          height: 56,
          child: _buildLocationMarker(
            context: context,
            icon: Icons.person_pin_circle,
            color: FlutterFlowTheme.of(context).primary,
            label: 'Téléphone',
          ),
        ),
      );
    }
    if (odbLocation != null) {
      markers.add(
        Marker(
          point: odbLocation,
          width: 140,
          height: 56,
          child: _buildLocationMarker(
            context: context,
            icon: Icons.location_on,
            color: FlutterFlowTheme.of(context).error,
            label: 'ODB',
          ),
        ),
      );
    }

    return Material(
      color: Colors.transparent,
      elevation: 2.0,
      shape: RoundedRectangleBorder(
        borderRadius: BorderRadius.circular(16.0),
      ),
      child: Container(
        width: double.infinity,
        decoration: BoxDecoration(
          color: FlutterFlowTheme.of(context).secondaryBackground,
          borderRadius: BorderRadius.circular(16.0),
          border: Border.all(
            color: FlutterFlowTheme.of(context).alternate,
            width: 1.0,
          ),
        ),
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            mainAxisSize: MainAxisSize.max,
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Row(
                mainAxisSize: MainAxisSize.max,
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Row(
                    children: [
                      Icon(
                        Icons.map_outlined,
                        color: FlutterFlowTheme.of(context).primary,
                        size: 24.0,
                      ),
                      const SizedBox(width: 12.0),
                      Text(
                        'Map',
                        style: FlutterFlowTheme.of(context).titleMedium.override(
                              font: GoogleFonts.interTight(
                                fontWeight: FontWeight.bold,
                                fontStyle: FlutterFlowTheme.of(context)
                                    .titleMedium
                                    .fontStyle,
                              ),
                              letterSpacing: 0.0,
                              fontWeight: FontWeight.bold,
                              fontStyle: FlutterFlowTheme.of(context)
                                  .titleMedium
                                  .fontStyle,
                            ),
                      ),
                    ],
                  ),
                  Container(
                    width: 12.0,
                    height: 12.0,
                    decoration: BoxDecoration(
                      color: connected
                          ? (data.hasValidGpsFix
                              ? FlutterFlowTheme.of(context).success
                              : FlutterFlowTheme.of(context).error)
                          : FlutterFlowTheme.of(context).secondaryText,
                      shape: BoxShape.circle,
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 12.0),
              SizedBox(
                height: 260.0,
                width: double.infinity,
                child: ClipRRect(
                  borderRadius: BorderRadius.circular(14.0),
                  child: FlutterMap(
                    options: MapOptions(
                      initialCenter: hasAnyLocation
                          ? mapCenter
                          : const latlong.LatLng(0.0, 0.0),
                      initialZoom: hasAnyLocation ? 14.0 : 2.0,
                    ),
                    children: [
                      TileLayer(
                        urlTemplate:
                            'https://tile.openstreetmap.org/{z}/{x}/{y}.png',
                        userAgentPackageName: 'com.mycompany.nexus',
                      ),
                      if (markers.isNotEmpty) MarkerLayer(markers: markers),
                    ],
                  ),
                ),
              ),
              const SizedBox(height: 12.0),
              Row(
                children: [
                  Expanded(
                    child: Row(
                      children: [
                        Container(
                          width: 10.0,
                          height: 10.0,
                          decoration: BoxDecoration(
                            color: FlutterFlowTheme.of(context).primary,
                            shape: BoxShape.circle,
                          ),
                        ),
                        const SizedBox(width: 8.0),
                        Expanded(
                          child: Text(
                            location.isLoading
                                ? 'Téléphone: chargement...'
                                : location.status,
                            style: FlutterFlowTheme.of(context).bodySmall.override(
                                  font: GoogleFonts.inter(
                                    fontWeight: FontWeight.w500,
                                    fontStyle: FlutterFlowTheme.of(context)
                                        .bodySmall
                                        .fontStyle,
                                  ),
                                  color: FlutterFlowTheme.of(context)
                                      .secondaryText,
                                  letterSpacing: 0.0,
                                  fontWeight: FontWeight.w500,
                                  fontStyle: FlutterFlowTheme.of(context)
                                      .bodySmall
                                      .fontStyle,
                                ),
                          ),
                        ),
                      ],
                    ),
                  ),
                  const SizedBox(width: 16.0),
                  Expanded(
                    child: Row(
                      children: [
                        Container(
                          width: 10.0,
                          height: 10.0,
                          decoration: BoxDecoration(
                            color: data.hasValidGpsFix
                                ? FlutterFlowTheme.of(context).error
                                : FlutterFlowTheme.of(context).secondaryText,
                            shape: BoxShape.circle,
                          ),
                        ),
                        const SizedBox(width: 8.0),
                        Expanded(
                          child: Text(
                            data.hasValidGpsFix
                                ? 'ODB: ${data.gpsLat.toStringAsFixed(5)}°, ${data.gpsLon.toStringAsFixed(5)}°'
                                : 'ODB: position indisponible',
                            style: FlutterFlowTheme.of(context).bodySmall.override(
                                  font: GoogleFonts.inter(
                                    fontWeight: FontWeight.w500,
                                    fontStyle: FlutterFlowTheme.of(context)
                                        .bodySmall
                                        .fontStyle,
                                  ),
                                  color: FlutterFlowTheme.of(context)
                                      .secondaryText,
                                  letterSpacing: 0.0,
                                  fontWeight: FontWeight.w500,
                                  fontStyle: FlutterFlowTheme.of(context)
                                      .bodySmall
                                      .fontStyle,
                                ),
                          ),
                        ),
                      ],
                    ),
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}