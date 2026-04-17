# Chat Customization Skill

Use this skill when the user asks to create or update chat customization files for this repository.

## Goal

Produce or refine only chat customization files, mainly [AGENTS.md](AGENTS.md) or [.github/copilot-instructions.md](.github/copilot-instructions.md), so future coding agents can work effectively in this workspace.

## Workflow

1. Discover existing customization files and nearby docs before writing anything.
2. Explore the codebase for conventions that matter to agents: entry points, architecture, build and test commands, generated code boundaries, and common pitfalls.
3. Prefer updating an existing [AGENTS.md](AGENTS.md); create one only if the repo does not already have suitable instructions.
4. Keep the guidance minimal, specific, and actionable.
5. Link to existing documentation instead of copying it.
6. Avoid product code changes unless the user explicitly asks for them.

## What To Capture

- Where the app starts and where the main architecture lives.
- Which directories are generated, shared, or safe to edit.
- Normal verification commands and platform-specific setup notes.
- Project conventions that are easy to miss from generic framework knowledge.
- Any recurring traps that would waste an agent’s time.

## Quality Rules

- Be concise; each line should help an agent make a decision.
- Do not duplicate documentation that already exists in the repo.
- Preserve the project’s language, naming, and architecture unless the user asks to change them.
- Keep generated or framework-managed files on a minimum-change path.

## Completion

When finished, report the customization files that were added or modified and briefly explain why each one is useful to an AI coding agent.

## Good Follow-Up Prompts

- Create a skill for Flutter page editing in this repo.
- Create a skill for safe FlutterFlow-generated file changes.
- Create a skill for testing and validation in this workspace.