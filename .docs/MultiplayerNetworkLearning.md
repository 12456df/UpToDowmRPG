# 多人游戏学习路径

> 从零基础到部署 Dedicated Server 的完整学习计划。
> 每个阶段按顺序推进，完成后在 `[ ]` 中打 `[x]` 标记进度。
>
> 最后更新：2026-07-12

---

## 阶段一：理解多人网络基础概念

目标：搞清楚客户端-服务器模型、Authority、Replication、RPC，**不写代码也要先理解**。

### 1.1 Multiplayer Network Compendium（社区口碑最佳入门）

- [ ] 阅读完整文档
- 链接：https://dev.epicgames.com/community/learning/tutorials/jO9e/unreal-engine-multiplayer-network-compendium
- 作者：eXi (Cedric Neukirchen)
- 内容：类体系、所有权、复制、RPC 全覆盖
- **这是最重要的一篇，务必第一个读完**

### 1.2 Multiplayer Networking 视频系列（Part 1-4）

- [ ] Part 1: What is Replication?
- [ ] Part 2: Game Instances
- [ ] Part 3: Network Relevancy
- [ ] Part 4: Multicast & RepNotify
- 链接：
  - Part 1: https://dev.epicgames.com/community/learning/tutorials/O5a/multiplayer-networking-part-1-what-is-replication
  - Part 2: https://dev.epicgames.com/community/learning/tutorials/GZd/multiplayer-networking-part-2-game-instances
  - Part 3: https://dev.epicgames.com/community/learning/tutorials/8x3/multiplayer-networking-part-3-network-relevancy
  - Part 4: https://dev.epicgames.com/community/learning/tutorials/3LJ/multiplayer-networking-part-4-multicast-repnotify
- 配合 Compendium 一起看效果最好

### 1.3 Epic 官方 — Networking Overview

- [ ] 阅读完整文档
- 链接：https://dev.epicgames.com/documentation/en-us/unreal-engine/networking-and-multiplayer-in-unreal-engine
- 官方网络总览，作为目录使用，后续阶段不断回来看

### 1.4 Networking Basics 新手视频教程（辅助）

- [ ] 观看视频
- 链接：https://dev.epicgames.com/community/learning/tutorials/rpEn/networking-basics-in-unreal-engine-beginner-tutorial
- Ahmed Fouad 出品，零基础友好，动手做颜色同步 demo

### 1.5 UE5-Multiplayer-Replication-Guide（辅助）

- [ ] 阅读英文指南
- [ ] 下载并运行示例项目
- 链接：https://github.com/droganaida/UE5-Multiplayer-Replication-Guide
- 覆盖 NetMode、Authority、Proxy、GameFramework 类体系

---

## 阶段二：动手写第一个多人游戏

目标：用 C++ 实现简单多人项目，掌握变量复制、Server RPC、Multicast、RepNotify 的实际用法。

### 2.1 Epic 官方 — Multiplayer Programming Quick Start

- [ ] 跟着教程完成整个 Quick Start
- 链接：https://dev.epicgames.com/documentation/unreal-engine/multiplayer-programming-quick-start-for-unreal-engine
- 用 C++ 从 Third Person Template 做简单多人项目
- 学 `DOREPLIFETIME`、`Server RPC`、`OnRep_` 回调
- **这是动手的起点**

### 2.2 Epic 官方 — Replicate Actor Properties

- [ ] 阅读文档
- [ ] 在项目中实践条件复制
- 链接：https://dev.epicgames.com/documentation/unreal-engine/replicate-actor-properties-in-unreal-engine
- 深入属性复制：条件复制、自定义条件、对象引用复制

### 2.3 Epic 官方 — Actor Component Replication

- [ ] 阅读文档
- [ ] 在项目中实践组件复制
- 链接：https://dev.epicgames.com/documentation/unreal-engine/actor-component-replication-in-unreal-engine
- 组件复制逻辑，与 Actor 复制的区别

### 2.4 Epic 官方 — Travelling in Multiplayer

- [ ] 阅读文档
- 链接：https://dev.epicgames.com/documentation/unreal-engine/travelling-in-multiplayer-in-unreal-engine
- 无缝旅行（Seamless Travel）、地图切换在多人中的工作方式

---

## 阶段三：深入复制优化与 GameFramework 类

目标：理解哪些类在服务器/客户端存在，掌握带宽优化手段。

### 3.1 Actor Network Dormancy

- [ ] 阅读文档
- [ ] 在项目中实践 Dormancy 设置
- 链接：https://dev.epicgames.com/documentation/unreal-engine/actor-network-dormancy-in-unreal-engine
- 休眠机制：不活跃的 Actor 停止复制，大幅节省带宽

### 3.2 Object Replication

- [ ] 阅读文档
- 链接：https://dev.epicgames.com/documentation/unreal-engine/object-replication-in-unreal-engine
- UObject 的复制，子对象复制细节

### 3.3 Iris Replication System（UE5.4+）

- [ ] 阅读 Introduction to Iris
- [ ] 在项目中启用 Iris（可选）
- 链接：https://dev.epicgames.com/documentation/unreal-engine/introduction-to-iris-in-unreal-engine
- UE5.4 引入的新复制系统，替代 Replication Graph，性能更好
- **做 DS 必须了解**

### 3.4 GameFramework 类在多人中的角色（知识梳理）

- [ ] 理解 AGameMode（仅服务器存在）
- [ ] 理解 AGameState（复制到所有客户端）
- [ ] 理解 APlayerState（每个玩家一份，复制到所有）
- [ ] 理解 APlayerController（仅拥有者客户端 + 服务器）
- [ ] 理解 APawn（复制到所有，输入仅 Autonomous Proxy）
- 参考来源：Compendium + 官方文档，对照已有 Aura 项目代码理解

---

## 阶段四：Session 管理与 Online Subsystem

目标：让玩家能"找到服务器并加入"，从局域网多人走向互联网多人。

### 4.1 UE5 Multiplayer in 2026（全景文）

- [ ] 阅读完整文章
- 链接：https://www.strayspark.studio/blog/ue5-multiplayer-2026-replication-eos
- 2026 年最新的多人开发全景文
- 涵盖 EOS、Listen Server vs DS 选择、测试策略、实用建议
- **必读**

### 4.2 Mastering Unreal Engine Multiplayer: EOS, Steam, and Replication

- [ ] 阅读完整文章
- [ ] 理解 EOS + Steam 双 OSS 配置
- 链接：https://artistscompany.net/blog/ue-multiplayer-networking-guide/
- Session 管理 C++ 代码示例、DefaultEngine.ini 配置详解

### 4.3 Epic 官方 — Setting up a Dedicated Server to Host EOS Sessions

- [ ] 跟着教程完成 EOS Session 配置
- 链接：https://dev.epicgames.com/community/learning/tutorials/eK7q/unreal-engine-setting-up-a-dedicated-server-to-host-eos-sessions
- 在 EOS 后端注册 DS、创建 Session、客户端 Find/Join Session 的完整流程
- 包含 AGameSession 派生类的 C++ 实现
- **核心教程**

### 4.4 EOS Developer Portal 配置（实践）

- [ ] 注册 EOS 开发者账号
- [ ] 创建 Product 和 Sandbox
- [ ] 配置 Client ID / Client Secret（客户端 + TrustedServer 各一套）
- [ ] 理解 Client Policy 和 TrustedServer Policy 的区别
- 参考来源：4.3 教程中的步骤

---

## 阶段五：构建与部署 Dedicated Server

目标：真正打包和部署 DS，让外部客户端能连接。

### 前置条件：编译源码版引擎

- [ ] 从 GitHub 克隆 Unreal Engine 源码
- [ ] 编译源码版引擎（DebugGame 或 Development 配置）
- [ ] 用源码引擎打开 Aura 项目验证编译通过
- **硬性前提**：Epic Launcher 版无法编译 Server Target

### 5.1 Epic 官方 — Setting Up Dedicated Servers

- [ ] 跟着教程完成 DS 构建与本地测试
- 链接：https://dev.epicgames.com/documentation/unreal-engine/setting-up-dedicated-servers-in-unreal-engine
- 以 Lyra 为模板，覆盖 Target.cs 配置、打包、本地测试

### 5.2 Couch Learn — How to Build a Dedicated Server for Your UE5 Game

- [ ] 跟着教程完成整个流程
- 链接：https://couchlearn.com/how-to-build-a-dedicated-server-for-your-unreal-engine-5-game/
- 口碑极好的社区教程，步骤清晰
- 从源码引擎编译到 DS 打包到多客户端连接测试
- **强烈推荐跟着做一遍**

### 5.3 Building and Configuring a Dedicated Server for UE5 Games（综合指南）

- [ ] 阅读文档
- 链接：https://startmygame.com/building-and-configuring-a-dedicated-server-for-unreal-engine-5-games-a-comprehensive-guide/
- 更详细的综合指南，覆盖 Server Target 优化参数

### 5.4 Docker 容器化部署

- [ ] 阅读 Unreal Containers — Dedicated Servers
- [ ] 编写项目的 Dockerfile
- [ ] 本地 Docker 运行 DS 并连接测试
- 链接：https://unrealcontainers.com/docs/use-cases/dedicated-servers
- Docker 容器化 DS 的标准方案，Adam Rehn 维护

### 5.5 UE5.7 Dedicated Server in Docker（Gameye 指南）

- [ ] 阅读 Gameye 指南
- 链接：https://gameye.com/blog/unreal-engine-5-dedicated-server-docker-container/
- 2026 年最新，覆盖 UE5.3-5.7，Dockerfile + RunUAT + Session API

### 5.6 EOS Online Framework — Agones/Kubernetes 部署（进阶）

- [ ] 阅读 Preparing Your Game Server
- [ ] （可选）搭建 Agones 测试环境
- 链接：https://docs.redpoint.games/docs/dedis/preparing_your_game_server/
- Agones/Kubernetes 环境下的 DS 部署，TrustedServer 策略配置
- 适合正式上线前的生产环境部署

---

## 阶段六：系统课程（可选但推荐）

如果需要系统性地学习，以下付费课程口碑最好。

### 6.1 Stephen Ulibarri — UE5.8 C++ Multiplayer FPS（Udemy）

- [ ] 完成课程
- 链接：在 Udemy 搜索 "Unreal Engine 5.8 C++ Multiplayer First Person Shooter"
- 适合：有 C++ 基础，想从零做完整多人 FPS
- 特点：数据驱动架构、Data Assets + Gameplay Tags、Client-Side Prediction、DS 兼容
- 2026 年 7 月更新

### 6.2 Artem Chaika — Advanced UE5 Multiplayer Gameplay Programming（Udemy）

- [ ] 完成课程
- 链接：在 Udemy 搜索 "Advanced Unreal Engine 5 Multiplayer Gameplay Programming"
- 适合：已掌握基础复制，想学 GAS + 多人
- 特点：Ability System 理论+实践、复制背包、Combat Abilities
- **与 Aura 项目技术栈最匹配（GAS）**

---

## 关键提醒

- [ ] 源码版引擎是构建 DS 的硬性前提，Epic Launcher 版无法编译 Server Target
- [ ] 从第一天就按多人架构写代码，事后改造单人项目为多人的成本是 5-10 倍
- [ ] 测试时用 Standalone 而不是 PIE，PIE 的网络行为与真实客户端不一致
- [ ] 尽早引入延迟测试（100ms+），localhost 上感觉良好不代表真实网络下可用
- [ ] Aura 项目已使用 GAS，做多人时重点理解 GAS 的复制行为（AbilitySpec、GameplayEffect 的网络同步）

---

## 学习进度追踪

| 阶段 | 状态 | 开始日期 | 完成日期 | 笔记 |
|------|------|----------|----------|------|
| 阶段一：概念 | 未开始 | | | |
| 阶段二：动手 | 未开始 | | | |
| 阶段三：深入 | 未开始 | | | |
| 阶段四：Session | 未开始 | | | |
| 阶段五：DS 部署 | 未开始 | | | |
| 阶段六：系统课 | 未开始 | | | |
