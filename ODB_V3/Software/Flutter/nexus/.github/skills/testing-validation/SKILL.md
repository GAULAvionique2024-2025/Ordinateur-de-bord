# Testing and Validation Skill

Use this skill when the user wants to validate Flutter changes in this repository.

## Goal

Choose the lightest validation that still gives confidence: analyzer checks, targeted tests, or manual app verification when automated coverage is thin.

## Workflow

1. Identify what changed: page UI, shared service logic, routing, dependencies, or platform configuration.
2. Check the relevant validation path before running anything:
   - [analysis_options.yaml](analysis_options.yaml) for lint expectations.
   - [test/widget_test.dart](test/widget_test.dart) for the current test baseline.
   - [pubspec.yaml](pubspec.yaml) if dependencies or assets changed.
3. Run the smallest useful validation first.
4. If that fails, fix the root cause and rerun the same check before expanding scope.
5. Escalate to broader validation only when the change touches shared state, routing, or platform files.

## Decision Points

- If the change is a widget or page edit, prefer targeted widget tests plus a manual app check.
- If the change is service logic or state plumbing, prefer tests around the affected service or model first.
- If the change is navigation, verify the route and page flow in the app after automated checks.
- If the change touches dependencies or assets, run `flutter pub get` before testing.
- If the change affects platform code, validate on the relevant platform in addition to Flutter tests.

## Validation Order

- `flutter test` for the main automated signal.
- `flutter analyze` when the change could introduce lints or type issues.
- `flutter run` for visual, interaction, or navigation confirmation.
- Platform-specific checks only when Android or iOS files changed.

## Quality Rules

- Prefer targeted checks over running the whole app unless the change demands it.
- Do not treat the placeholder smoke test as sufficient coverage for real features.
- Keep validation aligned with the repository’s current analyzer settings and Flutter lints.
- Report what was run and what remains unverified if full validation is not practical.

## Completion Checks

- The relevant tests pass, or there is a clear reason they cannot run.
- The analyzer is clean, or any remaining issues are unrelated and documented.
- Manual app verification covers UI or navigation changes that automated tests do not.
- Dependency or asset updates have been refreshed with `flutter pub get`.

## Good Follow-Up Prompts

- Create a skill for writing widget tests in this Flutter app.
- Create a skill for validating Bluetooth/service changes.
- Create a skill for manual QA of navigation and page flows.