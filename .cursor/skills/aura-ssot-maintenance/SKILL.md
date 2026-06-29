---
name: aura-ssot-maintenance
description: >-
  Refresh Aura project SSOT after milestones. Use when the user asks to maintain,
  update, or sync AGENTS.md, ue-project-context.md, aligned commit baseline, or
  says "维护 SSOT", "更新对齐提交", "refresh project context". Runs git diff
  from aligned commit to HEAD and patches only stale sections.
---

# Aura SSOT 维护

## 必读

1. `AGENTS.md` — 入口与「对齐提交」hash
2. `.agents/ue-project-context.md` — 项目事实
3. 编码规范（不重复进 SSOT）：`.cursor/rules/aura-ue57.mdc`

## 流程

### 1. 读取对齐点

从 `AGENTS.md` 文件头读取 `对齐提交`（short hash）与分支名。

### 2. Git 增量范围

```powershell
git rev-parse --short HEAD
git branch --show-current
git log <对齐提交>..HEAD --oneline
git diff <对齐提交>..HEAD --stat
```

不要通读全仓库。只根据 diff 涉及的路径决定更新哪些章节。

### 3. 按变更类型更新

| diff 涉及 | 更新 |
|-----------|------|
| `Aura.uproject`, `Plugins/` | `ue-project-context.md` → Plugin Dependencies |
| `Source/*/Build.cs`, 新模块 | Module Structure |
| GAS / Character / UI 类 | Subsystems、GAS usage、`AGENTS.md` §4 |
| `.cursor/rules/`, MCP, 工具链 | `AGENTS.md` §2 |
| 仅小 bug、数值、单个 BP | **通常不更新** |

### 4. 写回对齐点

两份文件头同步更新：

```markdown
*最后更新：YYYY-MM-DD*
*对齐提交：`<HEAD short>`（`<当前分支>`）*
```

### 5. 输出给用户

- 列出改了哪些节
- 列出需人工确认的三条（若有）
- 提醒：UEC GDD 仅在里程碑后从 `AGENTS.md` §3–§4 摘抄

## 禁止

- 编造未在代码或 diff 中出现的类名、插件
- 把 `aura-ue57.mdc` 全文复制进 SSOT
- 在没有架构变更时重写整份文档
