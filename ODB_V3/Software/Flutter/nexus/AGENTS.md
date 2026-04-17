# Agent Instructions

## Project Snapshot

This is a Flutter app named Nexus. The app entry point is [lib/main.dart](lib/main.dart), and most UI lives under [lib/pages](lib/pages). Navigation is routed through [lib/flutter_flow/nav/nav.dart](lib/flutter_flow/nav/nav.dart), and shared app infrastructure lives under [lib/flutter_flow](lib/flutter_flow).

## Working Rules

- Prefer small, focused edits that match the existing FlutterFlow-generated style.
- Treat generated FlutterFlow files as framework-managed unless the task clearly requires changing them.
- Keep the app architecture intact: `main.dart` wires `MultiProvider`, app state, and router setup.
- Use the existing service layer in [lib/services](lib/services) for Bluetooth and data logic rather than adding new state plumbing inside pages.
- Follow the established page pattern: page widget plus companion model file under the same directory.
- Preserve the French UI copy and existing navigation/page names unless the task explicitly changes product language or routes.

## Useful Commands

- Run tests with `flutter test`.
- Launch locally with `flutter run`.
- Refresh dependencies with `flutter pub get`.

## Notes For Changes

- Update [pubspec.yaml](pubspec.yaml) if you add packages or assets.
- Keep Android and iOS platform changes aligned with the Flutter package setup.
- If you touch generated or platform files, make the minimum change required and avoid broad formatting-only edits.