# Flutter Page Editing Skill

Use this skill when the user wants to change an existing Flutter page in this repository.

## Goal

Make focused, low-risk edits to page widgets while preserving the repo’s FlutterFlow-style structure, French UI copy, and navigation model.

## Workflow

1. Inspect the target page widget and its companion model file.
2. Check whether the page depends on shared services, reusable widgets, or router/navigation state.
3. Identify the smallest safe edit that satisfies the request.
4. Keep the page pattern intact: widget file for UI, model file for page state, and shared logic in `lib/services` or reusable widgets when appropriate.
5. Update routing or shared wiring only if the page change truly requires it.
6. Validate that the page still fits the app’s French language and existing navigation labels.

## What To Check First

- The page entry in [lib/flutter_flow/nav/nav.dart](lib/flutter_flow/nav/nav.dart).
- The page widget and its matching `*_model.dart` file under [lib/pages](lib/pages).
- Shared services in [lib/services](lib/services) that already own Bluetooth or data state.
- Reusable widgets in [lib/widgets](lib/widgets) that should be composed instead of duplicated.

## Decision Points

- If the change is visual only, keep the edit inside the page widget.
- If the change is page state, keep it in the companion model when that matches the existing pattern.
- If the change involves Bluetooth or app data, prefer the existing services instead of adding new state plumbing in the page.
- If the change affects navigation, update the router and bottom navigation together.
- If the page is FlutterFlow-generated, avoid broad rewrites; change only the lines needed.

## Quality Rules

- Preserve the existing component hierarchy and naming conventions.
- Avoid unnecessary refactors, formatting churn, or framework-wide cleanup.
- Keep French text consistent with the rest of the app unless the user asks to change language.
- Prefer composing existing widgets over copying UI blocks between pages.
- Make sure any new page behavior still works with `MultiProvider` in [lib/main.dart](lib/main.dart).

## Completion Checks

- The edited page still compiles with its model and imports.
- Navigation still resolves through the existing router.
- Shared state still comes from the service layer, not ad hoc page state.
- The page remains consistent with the surrounding FlutterFlow-generated code style.

## Good Follow-Up Prompts

- Create a skill for FlutterFlow-generated file safety.
- Create a skill for updating shared Flutter services in this repo.
- Create a skill for validating Flutter page changes with tests.