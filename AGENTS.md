# Aura — AI 协作单一事实来源（SSOT）

> **所有 AI 工具（Cursor、Hermes、UEC）应优先读本文件，再按需展开链接文档。**
> 事实变更时：先改这里或 `.agents/ue-project-context.md`，再同步到各工具侧配置。

*最后更新：2026-06-25*  
*对齐提交：`a9c6cb0`（`upgrade/ue5.7`）*

---

## 1. 文档分层（谁写什么）

| 层级 | 文件 | 职责 | 谁维护 |
|------|------|------|--------|
| **入口** | `AGENTS.md`（本文件） | 工具分工、工作流、文档索引 | 你 + AI 审核后提交 Git |
| **项目事实** | `.agents/ue-project-context.md` | 引擎版本、模块、插件、关键类名、GAS 架构 | 升级/大改后更新 |
| **编码规范** | `.cursor/rules/aura-ue57.mdc` | Cursor 写 C++ 时的强制约定 | 规范变更时改 |
| **回答习惯** | `.cursor/rules/skill-attribution.mdc` | 用过 Skill 时在末尾标注 | 一般不动 |
| **参考手册** | `.docs/GAS-Tranek-Documentation.md` | 第三方 GAS 百科（只读参考） | 偶尔 `Invoke-WebRequest` 刷新 |
| **源码** | `Source/Aura/` | 最终实现真相 | 正常开发 |
| **UEC 记忆** | UEC 面板 GDD / Memory | 编辑器内 AI 的简短摘要 | **从本仓库 SSOT 复制粘贴**，勿当主库 |

**原则：** 项目事实只维护一份（`.agents/ue-project-context.md`）。Rules 只写「怎么做」，不重复「是什么」。

---

## 2. 工具栈与边界

| 工具 | 用途 | 不负责 |
|------|------|--------|
| **Cursor** | C++、架构讨论、Git、Rules/Skills | 不直接改 `.uasset`（用 UEC） |
| **UEC** (`uecp-aura`) | 蓝图、关卡、资产、PIE 相关 MCP | 不替代 C++ 模块设计 |
| **Hermes** | 终端、长会话、可复用工作流 | 记忆不自动与 Cursor 同步 |

### MCP（Cursor）

- `uecp-aura` → `http://localhost:9876/uecp/Aura/mcp`（需 `UEC_MCP_TOKEN`）
- `mempalace` → 跨会话记忆（可选，**不能替代** Git 内 SSOT）

### 已移除（勿恢复）

- `UE_MCP_Bridge` / `ue-mcp` npm 桥 — 与 UEC 冲突

---

## 3. 项目一句话

**Aura**：UE 5.7 第三人称动作 RPG 学习项目，核心为 **GAS + Enhanced Input + WidgetController UI**，含敌人 AI 与近战战斗。

---

## 4. 关键架构（摘要）

详细类表见 [`.agents/ue-project-context.md`](.agents/ue-project-context.md)。

- **玩家 ASC**：`APlayerState`（`AAuraPlayerState`）上
- **敌人 ASC**：`AEnemy` Pawn 上
- **能力基类**：`UAuraGameplayAbility` → `UAuraDamageGameplayAbility` / `UAuraProjectileSpell`
- **属性**：`UAuraAttributeSet`；UI 数据经 `UAttributeInfo`
- **输入**：`DA_AuraInputConfig` + `UAuraInputComponent::BindAbilityActions`
- **UI**：被动 Widget + `UAuraWidgetController`（如 `UOverlayWidgetController`）
- **战斗接口**：`ICombatInterface`、`IEnemyInterface`
- **编辑器自动化**：BpGeneratorUltimate（UEC 生态）

---

## 5. 分支与备份

| 项 | 值 |
|----|-----|
| 当前工作分支 | `upgrade/ue5.7` |
| 升级前备份分支 | `backup/pre-ue5.7` |
| 升级前标签 | `backup/pre-ue5.7-2026-06-25` |
| 文件夹备份 | `e:\Unreal Projects\Aura_backup_pre-ue5.7_2026-06-25` |

---

## 6. 新 AI 会话开场白（复制即用）

```
项目：Aura（UE 5.7）。请先读 AGENTS.md 和 .agents/ue-project-context.md。
C++ 在 Cursor；蓝图/关卡用 UEC MCP uecp-aura。不要引用已删除的 ue-mcp。
```

Hermes 新建会话：`cd` 到本目录 → `hermes` → `/new Aura`，首条消息贴上面一段。

---

## 7. 何时更新 SSOT

| 事件 | 更新 |
|------|------|
| 引擎小版本 / API 迁移 | `ue-project-context.md` + `aura-ue57.mdc` |
| 新增模块或插件 | `ue-project-context.md` + 本文件 §4 |
| 换 MCP / 工具链 | 本文件 §2、§6 |
| 完成里程碑 | 可选：UEC GDD 从 §4 摘抄 |

**维护时：** 用 `git diff <对齐提交>..HEAD` 查看自上次 SSOT 对齐以来的代码变更；更新后把本文件与 `ue-project-context.md` 的「对齐提交」改为当前 `git rev-parse --short HEAD`。

---

## 8. 不要做的事

- 不要在 Cursor User Rules 里写长篇项目设定（会跟仓库 SSOT 分叉）
- 不要让 Hermes SOUL、UEC Memory 各自维护不同版本的引擎号
- 不要把 `aura-ue57.mdc` 当成项目百科（它是编码规范，不是事实库）
