import 'package:flutter/foundation.dart';
import 'package:geolocator/geolocator.dart';
import 'package:latlong2/latlong.dart' as latlong;

class LocationServiceManager with ChangeNotifier {
  latlong.LatLng? phoneLocation;
  bool isLoading = true;
  String status = 'Localisation du téléphone en attente';

  Future<void> refreshPhoneLocation() async {
    try {
      final locationEnabled = await Geolocator.isLocationServiceEnabled();
      if (!locationEnabled) {
        _updateState(
          isLoading: false,
          status: 'Services de localisation désactivés',
        );
        return;
      }

      var permission = await Geolocator.checkPermission();
      if (permission == LocationPermission.denied) {
        permission = await Geolocator.requestPermission();
      }

      if (permission == LocationPermission.denied) {
        _updateState(
          isLoading: false,
          status: 'Permission localisation refusée',
        );
        return;
      }

      if (permission == LocationPermission.deniedForever) {
        _updateState(
          isLoading: false,
          status: 'Permission localisation bloquée',
        );
        return;
      }

      final lastKnownPosition = await Geolocator.getLastKnownPosition();
      if (lastKnownPosition != null) {
        _updateState(
          phoneLocation: latlong.LatLng(
            lastKnownPosition.latitude,
            lastKnownPosition.longitude,
          ),
          status: 'Dernière position GPS valide disponible',
        );
      }

      final position = await Geolocator.getCurrentPosition(
        locationSettings: const LocationSettings(
          accuracy: LocationAccuracy.high,
        ),
      );

      _updateState(
        phoneLocation: latlong.LatLng(position.latitude, position.longitude),
        isLoading: false,
        status: 'Position du téléphone disponible',
      );
    } catch (_) {
      _updateState(
        isLoading: false,
        status: phoneLocation != null
            ? 'Dernière position GPS valide conservée'
            : 'Impossible de récupérer la position du téléphone',
      );
    }
  }

  void _updateState({
    latlong.LatLng? phoneLocation,
    bool? isLoading,
    String? status,
  }) {
    if (phoneLocation != null) {
      this.phoneLocation = phoneLocation;
    }
    if (isLoading != null) {
      this.isLoading = isLoading;
    }
    if (status != null) {
      this.status = status;
    }
    notifyListeners();
  }
}