# UE Project Context — Aura

*Last updated: 2026-06-26*  
*对齐提交：`f0a7d8f`（`main`）*

## Engine & Project Overview

**Engine version:** UE 5.7（Launcher，`EngineAssociation` GUID `{2FDF9238-4EA5-3C5A-DBBB-79AA37268A9C}`）

**Project name:** Aura

**Description:** 第三人称动作 RPG 教程/学习项目，以 Gameplay Ability System 为核心。

**Project type:** Game

**Genre / domain:** Action RPG，法术与近战战斗，敌人 AI

**Target platforms:**

- Windows（当前主要开发平台）

**Default map:** `/Game/Maps/StartupMap`

## Module Structure

**Primary game module:** `Aura`

| Module | Type | Notes |
|--------|------|-------|
| Aura | Runtime | 唯一游戏模块 |

**Key dependencies (`Aura.Build.cs`):**

- Public: `Core`, `CoreUObject`, `Engine`, `InputCore`, `EnhancedInput`, `GameplayAbilities`, `UMG`
- Private: `GameplayTags`, `GameplayTasks`, `NavigationSystem`, `Niagara`, `AIModule`

**Build targets:** `Aura.Target.cs`, `AuraEditor.Target.cs` — `BuildSettingsVersion.V6`, `IncludeOrderVersion.Latest`

## Plugin Dependencies

**Engine plugins enabled:**

- `GameplayAbilities` — GAS
- `MotionWarping` — 动画位移
- `PythonScriptPlugin` — 编辑器脚本
- `ModelingToolsEditorMode` — 仅 Editor

**Marketplace / Fab:**

- `BpGeneratorUltimate` — UEC 相关蓝图生成（已启用）

**Custom plugins:** 无（`Plugins/` 下无自研插件；已移除 `UE_MCP_Bridge`）

## Coding Conventions

**Naming prefixes:** 标准 UE（`A`/`U`/`F`/`E`/`I`）+ 资产前缀 `GA_`/`GE_`/`BP_`/`WBP_`/`AM_`/`AN_`/`IA_`/`IMC_`/`DA_`

**Header style:** `#pragma once`，`Public/` / `Private/` 分目录

**Asset manager:** `UAuraAssetManager`（`Config/DefaultEngine.ini` → `AssetManagerClassName`）

**Gameplay tags:** 原生标签在 `FAuraGameplayTags`（`AuraGameplayTags.h/.cpp`），启动于 `UAuraAssetManager::StartInitialLoading()`

**Additional rules:** 见 `.cursor/rules/aura-ue57.mdc`（UE 5.7 API 与 GAS 约定）

## Subsystems in Use

**Gameplay framework:**

- GameMode: `AAuraGameModeBase`
- PlayerController: `AAuraPlayerController`
- PlayerState / 玩家 ASC 宿主: `AAuraPlayerState`
- Player Character: `AAuraCharacter` : `ACharacterBase`
- Enemy: `AEnemy` : `ACharacterBase`（ASC 在 Pawn）
- AIController: `AAuraAIController`
- HUD: `AAuraHUD`

**GAS usage:**

| 角色 | 类 |
|------|-----|
| ASC | `UAuraAbilitySystemComponent` |
| AttributeSet | `UAuraAttributeSet` |
| Ability 基类 | `UAuraGameplayAbility` |
| 伤害能力 | `UAuraDamageGameplayAbility` |
| 投射物法术 | `UAuraProjectileSpell` |
| GE Context | `FAuraGameplayEffectContext` |
| Globals | `UAuraAbilitySystemGlobals` |
| 库函数 | `UAuraAbilitySystemLibrary` |

**输入：** `UAuraInputConfig`（DataAsset）+ `UAuraInputComponent` + `IMC_AuraContext`

**UI：** `UAuraUserWidget`（被动）+ `UAuraWidgetController` / `UOverlayWidgetController` / `UAttributeMenuWidgetController`

**战斗 / 交互：** `ICombatInterface`, `IEnemyInterface`

**Ability tasks:** `UTargetDataUnderMouse`（鼠标瞄准模式）

## Build Configuration

**Build targets:** Game, Editor

**UE 5.7 升级注意点（已在代码中处理）：**

- `FOverlapResult` 需 `#include "Engine/OverlapResult.h"`
- `FGameplayAbilitySpec::GetDynamicSpecSourceTags()` 替代 `DynamicAbilityTags`
- 勿在 Client RPC 中传递 `FGameplayEffectSpec`；用 `FGameplayTagContainer` 等轻量类型
- `FAuraGameplayEffectContext::GetScriptStruct()` / `Duplicate()` 须与结构体类型一致

**Third-party libraries:** 无额外 C++ 第三方库

**Engine modifications:** 无自定义引擎 fork

## Team Context

**Team size:** 单人学习项目

**Source control:** Git（`e:\Unreal Projects\Aura`）

**Branching:** `main` 为当前主分支（已含 UE 5.7）；`backup/pre-ue5.7` 保留升级前快照

**AI SSOT 入口:** 仓库根目录 `AGENTS.md`
