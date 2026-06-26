# Aura Project — Local Documentation Index

This folder holds offline copies of key documentation. They are indexed by
Cursor as part of the codebase, so the AI can reference them automatically.

Use `@Folders` -> `.docs` or `@Files` -> a specific file in Chat / Composer
to inject these into context.

## Files

| File | Source | Topic |
|------|--------|-------|
| `GAS-Tranek-Documentation.md` | https://github.com/tranek/GASDocumentation | The de-facto bible for Unreal Engine Gameplay Ability System. Covers ASC, AttributeSet, GameplayEffect, GameplayCue, prediction, replication, ability tasks, etc. ~330 KB. |

## How to refresh

```powershell
cd "<project root>"
Invoke-WebRequest -Uri https://raw.githubusercontent.com/tranek/GASDocumentation/master/README.md -OutFile .docs/GAS-Tranek-Documentation.md -UseBasicParsing
```

## Notes

- Files in this folder are deliberately NOT excluded by `.cursorignore`, so
  they participate in normal codebase indexing.
- Epic UE 5.7 official HTML docs are not mirrored here. Use Cursor
  `Settings -> Indexing & Docs -> Add Doc` with prefix
  `https://dev.epicgames.com/documentation/en-us/unreal-engine/`
  if you want them.