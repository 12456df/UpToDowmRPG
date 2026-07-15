# Multiplayer Network Compendium 中文翻译

> 原文作者：Cedric Neukirchen (eXi)
> 原文地址：https://cedric-neukirchen.net/docs/category/multiplayer-network-compendium
> 翻译仅供学习，版权归原文作者所有。
>
> 翻译进度追踪：

| 章节 | 状态 |
|------|------|
| 1. Introduction | ✅ 已翻译 |
| 2. Network in Unreal | ✅ 已翻译 |
| 3. Gameplay Framework | ✅ 已翻译 |
| 4. Gameplay Framework + Network | ⬜ 未开始 |
| 5. Dedicated Server vs Listen Server | ⬜ 未开始 |
| 6. Replication | ⬜ 未开始 |
| 7. Remote Procedure Calls | ⬜ 未开始 |
| 8. Ownership | ⬜ 未开始 |
| 9. Actor Relevancy and Priority | ⬜ 未开始 |
| 10. Actor Role and RemoteRole | ⬜ 未开始 |
| 11. Traveling in Multiplayer | ⬜ 未开始 |
| 12. How to Start a Multiplayer Game | ⬜ 未开始 |
| 13. Additional Resources | ⬜ 未开始 |

---

## 第1章：Introduction（简介）

> ⚠️ **注意**
>
> 本手册假定你已具备虚幻引擎单人游戏框架的基础知识。
>
> 本手册**不会**教初学者如何使用虚幻引擎的一般功能。
>
> 如果你是初学者，请先阅读由 Marcos Romero 编写的 [Blueprint Compendium](https://romeroblueprintcompendium.com/) 来入门虚幻引擎。本网络手册要求你掌握蓝图（Blueprint）的使用，以及 C++ 示例所需的 C++ 基础。
>
> 示例基于虚幻引擎 5.1.x 创建，因此可能与您使用的最新 UE 版本略有不同。本手册的目的仅仅是帮助你入门虚幻引擎的网络开发！

### 欢迎来到虚幻引擎网络手册！

虽然官方文档已经相当完善，但我还是想总结一下过去几年使用虚幻引擎开发多人游戏时所积累的基础知识。

接下来的页面将向你介绍虚幻引擎的网络框架，并解释常用类、复制（Replication）、所有权（Ownership）等概念。只要有可能，我都会提供蓝图和 C++ 示例，帮助理解各种机制的工作原理。

当然，这些页面也会包含大量来自官方文档的内容。本手册旨在做一份**摘要总结**，而不是对已有文档的重新定义！

---

## 第2章：Network in Unreal（虚幻引擎中的网络）

虚幻引擎采用标准的**服务器-客户端架构（Server-Client Architecture）**。这意味着服务器是**权威的（Authoritative）**，所有数据必须先由客户端发送到服务器。然后服务器验证数据并根据你的代码做出反应。

### 一个小例子

当你在多人对局中作为客户端移动角色时，你并不是直接移动你的角色，而是**告诉服务器你想要移动它**。服务器随后为包括你在内的所有人更新角色的变换（Transform）。

> 💡 **提示**
>
> 为了避免本地客户端产生"卡顿"感，程序员通常还会**额外**让本地客户端直接控制自己的角色——尽管当客户端开始作弊时，服务器仍然可能会覆盖角色的位置！这意味着客户端（几乎）永远不会直接与其他客户端"对话"。

### 另一个例子

当你向另一个客户端发送聊天消息时，你会先将消息发送到服务器，然后服务器再将消息转发给你想要到达的客户端。这个目标也可以是一个团队、公会、小组等。

> ❗ **重要**
>
> **永远不要信任客户端！** 这里所说的"信任客户端"是指你不验证客户端的动作就直接执行。
>
> 这将允许他们作弊！
>
> 一个简单的例子是开枪射击：务必在**服务器上**检查客户端是否拥有所需的弹药数量以及是否被允许射击，而不是直接处理射击请求！

---

## 第3章：Gameplay Framework（游戏框架）

以下页面将解释虚幻引擎"游戏框架"中最常用的一些类，并提供这些类的简单使用示例。

这个列表是对框架的入门介绍，主要是为了确保我在手册后续部分提到这些类时你已经了解它们。

列出的示例都需要复制（Replication）的相关知识。如果你暂时看不懂某个示例，先跳过，等读完复制相关章节后再回来看。

某些游戏类型可能会以不同方式使用这些类。以下示例和说明并不是使用某个类的唯一方式。

---

### 3.1 AGameMode

> 💡 **提示**
>
> 从 4.14 版本开始，`AGameMode` 类被拆分为 `AGameModeBase` 和 `AGameMode`。`GameModeBase` 功能较少，因为有些游戏可能不需要旧 `AGameMode` 类的完整功能列表。

`AGameMode` 类用于定义你游戏的**规则**。这包括要生成哪些其他游戏框架类，如 `APawn`、`APlayerController`、`APlayerState` 等。

它**仅在服务器上存在**。客户端没有 `AGameMode` 类的实例，尝试获取时只会得到 `nullptr`。

#### 示例与用法

一些 `GameMode` 的用例可以从老式第一人称射击游戏中了解，比如虚幻竞技场（Unreal Tournament）：死斗（Deathmatch）、团队死斗（Team Deathmatch）或夺旗（Capture the Flag）。

这意味着 `GameMode` 可以定义如下内容：

- 我们有队伍还是每个人各自为战？
- 胜利条件是什么？
  - 某人或某队需要多少击杀数？
- 如何获得分数？
  - 击杀某人？
  - 夺取旗帜？
- 使用什么角色？
- 允许什么武器？
  - 只能手枪？
  - 只有一个弹夹？

在多人场景中，`GameMode` 还有一些有趣的函数，帮助我们管理玩家和/或比赛的总体流程。

#### 蓝图示例

**函数**

我们首先来看蓝图版本的重写函数区域：

你可以为这些函数实现自己的逻辑，以适配你游戏的特定规则。这包括更改 `GameMode` 生成 `DefaultPawn` 的方式，或者你想如何决定游戏是否"准备好开始"。

一个例子是检查是否所有玩家都已加入服务器并准备就绪：

`Event OnPostLogin` 是一个很好的例子，我经常使用它。每当新玩家加入游戏时就会调用它。此时它会传递一个有效的 `PlayerController`，该控制器由连接玩家的 `UConnection` 拥有。

这可以用来与新玩家交互，例如为他们生成新的 `Pawn`，或者只是将他们的 `PlayerController` 保存到数组中留作后用。

如前所述，你可以使用 `GameMode` 来管理比赛的总体流程。为此，你可以找到一些函数，其中一些是可重写的，如"Ready To Start Match"。

这些函数和事件可以用来控制当前的 `MatchState`。它们中的大多数会在"Ready To Start Match"函数返回 `TRUE` 时自动调用，但你也可以手动使用它们。

"New State"是一个简单的 `FName` 类型。你可能会问，"为什么这不是在 `AGameState` 类中处理的？"

实际上确实是在 `GameState` 中处理的。这些 `GameMode` 函数与 `GameState` 携手合作。

这只是为了给你一个管理 `MatchState` 的入口，而客户端无法触及，因为 `GameMode` 仅存在于服务器上！

**变量**

`GameMode` 还有一些你可以且想要使用的重要变量。这些是已继承的变量，其中一些可以通过 `GameMode` 蓝图的类默认值设置：

大多数变量都很容易理解，比如"Default Player Name"，它让你能够为每个连接的玩家设置默认玩家名，可以通过 `APlayerState` 类访问。

还有 `bDelayedStart`，即使"Ready To Start Match"的默认实现满足所有其他条件，它也会阻止游戏开始。

更重要的变量之一是所谓的"Options String"。这些是通过 `?` 分隔的选项，你可以通过 `OpenLevel` 函数或作为控制台命令调用 `ServerTravel` 时传递。

你可以使用"Parse Option"来提取传递的选项，例如 `MaxNumPlayers`：

#### C++ 示例

所有蓝图操作同样可以在 C++ 中完成。我不会重复相同的信息，而是提供一些代码示例来重现之前的蓝图示例。

由于 `ReadyToStartMatch` 是一个 `BlueprintNativeEvent`，实际的 C++ 实现函数名为 `ReadyToStartMatch_Implementation`。这是我们想要重写的：

```cpp
// 我们 AGameMode 子类的头文件，类声明内部

// 当前比赛需要/允许的最大玩家数
int32 MaxNumPlayers;

// 重写 ReadyToStartMatch 的实现
virtual bool ReadyToStartMatch_Implementation() override;
```

```cpp
// 我们 GameMode 子类的 CPP 文件

bool ATestGameMode::ReadyToStartMatch_Implementation()
{
    Super::ReadyToStartMatch();
    return MaxNumPlayers == NumPlayers;
}
```

`OnPostLogin` 函数是虚函数，在 C++ 中简称为 `PostLogin`。我们也来重写它：

```cpp
// 我们 AGameMode 子类的头文件，类声明内部

// PlayerController 列表
UPROPERTY()
TArray<APlayerController*> PlayerControllerList;

// 重写 PostLogin 函数
virtual void PostLogin(APlayerController* NewPlayer) override;
```

```cpp
// 我们 GameMode 子类的 CPP 文件

void ATestGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    PlayerControllerList.Add(NewPlayer);
}
```

---

### 3.2 AGameState

> 💡 **提示**
>
> 从 4.14 版本开始，`GameState` 类被拆分为 `AGameStateBase` 和 `AGameState`。`GameStateBase` 功能较少，因为有些游戏可能不需要旧 `GameState` 类的完整功能列表。

`AGameState` 可能是服务器和客户端之间**共享信息最重要的类**。

`GameState` 用于跟踪游戏/比赛的当前状态。对于多人游戏来说，这包括已连接玩家的列表（`APlayerState`）。

此外，它会**复制给所有客户端**，所以每个人都能访问它。这使得 `GameState` 成为多人游戏中信息方面最核心的类之一。

如果说 `GameMode` 定义了需要多少击杀才能获胜，那么 `GameState` 就会跟踪每个玩家和/或团队的当前击杀数！

你在这里存储什么信息完全取决于你自己。可以是分数数组，也可以是用于跟踪小组和公会的自定义结构体数组。

#### 示例与用法

在多人游戏中，`AGameState` 类用于跟踪游戏的当前状态，这也包括玩家及其 `PlayerState`。

`GameMode` 确保 `GameState` 的 `MatchState` 函数被调用，而 `GameState` 本身也允许你在客户端上使用它们。

与 `GameMode` 相比，`GameState` 没有太多可操作的东西，但这仍然允许我们创建自己的逻辑，主要用于向客户端传播信息。

**蓝图示例 — 变量**

我们从基类 `AGameState` 继承了一些可以利用的变量。`PlayerArray`、`MatchState` 和 `ElapsedTime` 都是复制的，所以客户端也可以访问它们。

但 `AuthorityGameMode` 例外，只有服务器能访问它，因为 `GameMode` 只存在于服务器上。

`PlayerArray` 并不是直接复制的，但是每个 `PlayerState` 都会被复制，并且它们在构造时将自己添加到 `PlayerArray` 中。此外，`GameState` 也会收集它们，以确保不会因竞态条件而出问题。

以下是 C++ 中 `PlayerState` 如何被收集到 `PlayerArray` 的简要代码：

`PlayerState` 类内部：

```cpp
void APlayerState::PostInitializeComponents()
{
    // [...]
    UWorld* World = GetWorld();
    // 将此 PlayerState 注册到游戏的 ReplicationInfo
    if (World->GameState != NULL)
    {
        World->GameState->AddPlayerState(this);
    }
    // [...]
}
```

`GameState` 中：

```cpp
void AGameState::PostInitializeComponents()
{
    // [...]
    for (TActorIterator<APlayerState> It(World); It; ++It)
    {
        AddPlayerState(*It);
    }
}

void AGameState::AddPlayerState(APlayerState* PlayerState)
{
    if (!PlayerState->bIsInactive)
    {
        PlayerArray.AddUnique(PlayerState);
    }
}
```

所有这些操作在服务器和客户端的 `PlayerState` 和 `GameState` 实例上都会发生！

**蓝图示例 — 函数**

一个简单的函数示例：跟踪两个队伍"A"和"B"的分数。假设我们有一个自定义事件，在某队得分时被调用。

它传递一个布尔值让我们知道哪队得分了。我们也可以传递 `PlayerState`、`Team` 或你用来识别谁得分的任何东西。

在后面的"复制"章节中，你会读到只有服务器才能（也应该）复制变量的规则，所以我们确保只有服务器能调用此事件。

该事件从另一个类（例如击杀了某人的武器）调用，这应该在**服务器上**发生（始终如此！），所以我们这里不需要 RPC。

由于这些变量和 `GameState` 都是复制的，你可以在任何需要的其他类中使用这两个变量。例如，在记分板 Widget 中显示它们。

**C++ 示例**

要重现这个小示例，我们需要多一点代码，但除了函数本身之外，设置复制所需的代码每个类只需写一次。

```cpp
// 我们 AGameState 子类的头文件，类声明内部

// 需要包含此头文件才能让复制工作
#include "UnrealNetwork.h"

// Replicated 修饰符用于标记此变量需要复制
UPROPERTY(Replicated)
int32 TeamAScore;

UPROPERTY(Replicated)
int32 TeamBScore;

// 增加某队分数的函数
void AddScore(bool bTeamAScored);
```

你将在复制部分读到更多关于此函数的信息！

```cpp
// 我们 AGameState 子类的 CPP 文件

// 此函数是 UPROPERTY 宏中 Replicated 修饰符所要求的，并由它声明
void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ATestGameState, TeamAScore);
    DOREPLIFETIME(ATestGameState, TeamBScore);
}
```

```cpp
// 我们 AGameState 子类的 CPP 文件

void ATestGameState::AddScore(bool bTeamAScored)
{
    if (bTeamAScored)
    {
        TeamAScore++;
    }
    else
    {
        TeamBScore++;
    }
}
```

---

### 3.3 APlayerState

`APlayerState` 类是关于特定玩家共享信息的**最重要**的类。它用于保存玩家的当前信息。每个玩家都有自己的 `PlayerState`。

`PlayerState` 也会**复制给所有人**，可用于在其他客户端上检索和显示数据。访问所有 `PlayerState` 的简单方法是通过 `AGameState` 类中的 `PlayerArray`。

你可能想要存储在 `PlayerState` 中的示例信息：

- **PlayerName** — 玩家的当前名称
- **Score** — 玩家的当前分数
- **Ping** — 玩家的当前延迟
- **TeamID** — 玩家所在队伍的 ID
- 或者其他玩家可能需要了解的复制信息

#### 示例与用法

我能提供的大多数示例都非常具体。因此，我们将看看一些已有的属性以及一些更有趣的函数。

**蓝图示例**

有一些变量暴露给蓝图，或多或少有些用处。遗憾的是，其中一些并没有暴露全部功能，所以最好用自己的版本替换它们。

这些变量都是复制的，所以它们在所有客户端上保持同步。

遗憾的是，它们在蓝图中不容易设置，但没有什么能阻止你创建自己的版本。

设置 `PlayerName` 变量的一个例子是调用 `GameMode` 的 `ChangeName` 函数，并传递该玩家的 `PlayerController`。

`PlayerState` 还用于确保数据在无缝地图切换（Seamless Travel）或意外断线重连时保持持久。

`PlayerState` 有两个专用函数来处理重连的玩家和跟随服务器无缝旅行到新地图的玩家。

这些函数现在也可以在蓝图中使用了。

`PlayerState` 负责将已持有的信息复制到新的 `PlayerState` 中。这个新的 `PlayerState` 可能是通过地图切换创建的，也可能是因为玩家重连而创建的。

**C++ 示例**

让我们看看同样的函数，但用 C++ 实现：

```cpp
// 我们 APlayerState 子类的头文件，类声明内部

// 用于将当前 PlayerState 的属性复制到传入的 PlayerState
virtual void CopyProperties(class APlayerState* PlayerState);

// 用于用传入 PlayerState 的属性覆盖当前 PlayerState
virtual void OverrideWith(class APlayerState* PlayerState);
```

这些函数可以在你自己的 C++ `PlayerState` 子类中实现，以管理你添加到自定义 `PlayerState` 中的数据。

确保在末尾添加 `override` 修饰符，并调用 `Super::` 以保持原始实现活跃。

你的实现可能类似于这样：

```cpp
// 我们 APlayerState 子类的 CPP 文件

void ATestPlayerState::CopyProperties(class APlayerState* PlayerState)
{
    Super::CopyProperties(PlayerState);

    if (IsValid(PlayerState))
    {
        ATestPlayerState* TestPlayerState = Cast<ATestPlayerState>(PlayerState);
        if (IsValid(TestPlayerState))
        {
            TestPlayerState->SomeVariable = SomeVariable;
        }
    }
}

void ATestPlayerState::OverrideWith(class APlayerState* PlayerState)
{
    Super::OverrideWith(PlayerState);

    if (IsValid(PlayerState))
    {
        ATestPlayerState* TestPlayerState = Cast<ATestPlayerState>(PlayerState);
        if (IsValid(TestPlayerState))
        {
            SomeVariable = TestPlayerState->SomeVariable;
        }
    }
}
```

---

### 3.4 APawn 和 ACharacter

`APawn` 类是玩家控制的 `AActor`。大多数时候它是一个人形角色，但也可以是猫、飞机、船、方块等。玩家一次只能拥有（Possess）一个 Pawn，但可以通过取消拥有和重新拥有在 Pawn 之间轻松切换。

Pawn 通常会**复制给所有客户端**。

`APawn` 的子类 `ACharacter` 经常被使用，因为它自带一个已网络化的 `MovementComponent`，负责复制玩家角色的位置、旋转等。

#### 示例与用法

在多人游戏中，我们主要使用 Pawn 的复制功能来显示角色并与他人共享一些信息。一个简单的例子是角色的"生命值（Health）"。但我们复制"生命值"不仅是为了让其他人看到，还为了让**服务器拥有对它的权威**，防止客户端作弊。

**蓝图**

除了标准的可重写函数外，Pawn 还有两个函数让你对被玩家或 AI 控制器拥有/取消拥有做出反应。

> 💡 **提示**
>
> 由于拥有逻辑在服务器上执行，这些事件只会在 Pawn/角色的服务器版本上被调用。

还有一个名为 `ReceiveControllerChanged` 的函数，允许你在客户端一侧对相同的事件做出反应。

以下图片展示如何使用 `EventAnyDamage` 函数和复制的 `Health` 变量来降低玩家的生命值。这发生在**服务器上**，不在客户端上！

由于 Pawn 应该被复制，`DestroyActor` 节点也会销毁 Pawn 的客户端版本，只要由服务器调用即可。在客户端一侧，我们可以使用复制的 `Health` 变量来显示 HUD 或每个人头顶的生命条。

你可以通过创建一个带有 `ProgressBar` 和 Pawn 引用的 `UserWidget` 来轻松实现。

假设我们的 `BP_Character` 类上有 `Health` 和 `MaxHealth` 变量，都设置为复制（如果 `MaxHealth` 运行时从不改变，可以跳过复制它）。

在 `UserWidget` 中创建了 `BP_Character` 引用变量和 `ProgressBar` 后，我们可以将进度条的百分比绑定到以下函数：

进一步地，设置好 `WidgetComponent` 后，我们可以将"Widget Class To Use"设置为你的生命条 `UserWidget`，并在 `BeginPlay` 时执行以下操作：

`BeginPlay` 在 Pawn 的所有实例上被调用，这意味着在服务器和所有客户端上。

所以每个实例现在都将自己设置为它所拥有的 `UserWidget` 的 Pawn 引用。

由于 Pawn 和 `Health` 变量都是复制的，我们能在每个 Pawn 头顶显示正确的百分比。

如果此时复制过程还不清楚，别担心，继续阅读，最后你会理解为什么这如此简单！

**C++**

对于 C++ 示例，我不会重现 `UserWidget` 的例子。在 C++ 中让 `UserWidget` 工作需要太多样板代码，我不想在这里介绍。

所以我们专注于 Possess 和伤害事件。在 C++ 中，两个 Possess 事件分别叫做：

```cpp
virtual void PossessedBy(AController* NewController);
virtual void UnPossessed();
```

`UnPossessed` 事件不会传递旧的 `PlayerController`。

我们还想在 C++ 中重现生命值示例。像往常一样，如果你此刻不理解复制的步骤，不用担心，接下来的章节会为你解释。

如果你觉得复制部分的示例太复杂，现在先跳过即可。

`TakeDamage` 函数等同于 `EventAnyDamage` 节点。要造成伤害，你通常在想要对其造成伤害的 Actor 上调用 `TakeDamage`，如果该 Actor 实现了该函数，它就会做出反应，类似于此示例的做法。

```cpp
// 我们 APawn 子类的头文件，类声明内部

// 复制的生命值变量
UPROPERTY(Replicated)
int32 Health;

// 重写 TakeDamage 事件
virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
```

```cpp
// CPP

// 此函数是必需的，UPROPERTY 宏中的 Replicated 修饰符会声明它，我们只需实现
void ATestPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // 告诉 UE 我们想要复制此变量
    DOREPLIFETIME(ATestPawn, Health);
}

float ATestPawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    // 降低玩家的生命值
    Health -= ActualDamage;
    // 如果生命值小于等于 0 则销毁
    if (Health <= 0.f)
    {
        Destroy();
    }
    return ActualDamage;
}
```

---

### 3.5 APlayerController

`APlayerController` 可能是我们遇到的最有趣、最复杂的类。它也是大量客户端逻辑的中心，因为这是客户端真正"拥有"的**第一个类**。

`PlayerController` 可以看作是玩家的**"输入"**。它是玩家与服务器的连接。这意味着每个客户端都有一个 `PlayerController`。客户端的 `PlayerController` 只存在于其自身端和服务器上。客户端**无法**访问其他客户端的 `PlayerController`。

**每个客户端只知道自己的 `PlayerController`！**

这意味着服务器拥有所有客户端 `PlayerController` 的引用！

"输入"这个词并不直接意味着所有实际输入（按键、鼠标移动、手柄摇杆等）都需要放在 `PlayerController` 中。

一个好的实践是：将 Pawn/角色特定的输入（汽车和人类的操作方式不同）放在 `APawn`/`ACharacter` 类中，将应该在所有角色上通用、或者当角色对象无效时也需要工作的输入放在 `PlayerController` 中。

此外，一个重要的事情：

**如何获取正确的 PlayerController？**

著名的节点 `GetPlayerController(0)` 或代码行 `UGameplayStatics::GetPlayerController(GetWorld(), 0);` 在服务器和客户端上的工作方式不同：

- 在**监听服务器**上调用会返回监听服务器的 `PlayerController`
- 在**客户端**上调用会返回该客户端的 `PlayerController`
- 在**专用服务器**上调用会返回第一个客户端的 `PlayerController`

使用非 `0` 的数字**不会**为客户端返回其他客户端的 `PlayerController`。此索引用于本地多人（分屏），我们这里不会涉及。

#### 示例与用法

尽管 `APlayerController` 是网络方面最重要的类之一，但默认情况下它并没有太多内容。

所以我们将创建一个小示例，来阐明为什么需要它。在所有权章节中，你将了解为什么 `PlayerController` 对 RPC 很重要。

以下示例将展示如何利用 `PlayerController` 通过按下 `UserWidget` 按钮来递增 `GameState` 中的复制变量。

**为什么需要 PlayerController？**

我不想在这里把 RPC 和所有权章节写两遍，所以简短解释：

`UserWidget` 只存在于本地玩家（客户端或监听服务器）上，即使它被客户端拥有，`ServerRPC` 在服务器上也没有它的实例来运行。

它根本不会被复制！

这意味着我们需要一种方式将按钮按下传递到服务器，这样服务器才能递增变量。

**为什么不在 GameState 上直接调用 RPC？**

因为 `GameState` 归服务器所有。`ServerRPC` 需要客户端作为拥有者！

**蓝图**

首先，我们需要一个带按钮的简单 `UserWidget`。

我将按相反顺序贴图，这样你可以看到它在哪里结束以及哪些事件调用了前面图片中的事件。

从我们的目标开始——`GameState`。它有一个普通事件来递增一个复制的整数变量：

此事件在服务器端被调用，位于我们 `PlayerController` 的 `ServerRPC` 中：

最后，我们有我们的按钮，被按下时调用 `ServerRPC`：

所以当我们点击按钮（客户端侧）时，我们使用 `PlayerController` 中的 `ServerRPC` 到达服务器侧（可行，因为 `PlayerController` 由客户端拥有！），然后调用 `GameState` 的 `IncreaseVariable` 事件来递增复制的整数变量。

这个整数变量，由于它是复制并由服务器设置的，现在会在 `GameState` 的所有实例上更新，客户端也能看到更新！

**C++**

对于此示例的 C++ 版本，我将用 `PlayerController` 的 `BeginPlay` 替代 `UserWidget`。这没什么实际意义，但在 C++ 中实现 `UserWidget` 需要更多代码，我不想在这里贴出来。

```cpp
// 我们 APlayerController 子类的头文件，类声明内部

// Server RPC。你将在 RPC 章节读到更多
UFUNCTION(Server, unreliable, WithValidation)
void Server_IncreaseVariable();

// 同时重写 BeginPlay 用于此示例
virtual void BeginPlay() override;
```

```cpp
// 我们 AGameState 子类的头文件，类声明内部

// 复制的整数变量
UPROPERTY(Replicated)
int32 OurVariable;

public:
// 递增变量的函数
void IncreaseVariable();
```

```cpp
// 我们 PlayerController 子类的 CPP 文件

// 否则无法访问 GameState 的函数
#include "TestGameState.h"

// 你稍后会读到关于 RPC 的内容以及为什么需要 _Validate
bool ATestPlayerController::Server_IncreaseVariable_Validate()
{
    return true;
}

// 你稍后会读到关于 RPC 的内容以及为什么需要 _Implementation
void ATestPlayerController::Server_IncreaseVariable_Implementation()
{
    ATestGameState* GameState = Cast<ATestGameState>(UGameplayStatics::GetGameState(GetWorld()));
    GameState->IncreaseVariable();
}

void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();
    // BeginPlay 在 Actor 的每个实例上都被调用，包括此 PlayerController 的服务器版本
    // 我们只想确保只有本地玩家调用此 RPC。再次说明，此示例不一定有多大意义
    // 因为我们可以直接翻转条件就不需要 RPC 了，但是 C++ Widget 你懂的...
    // 我们也可以在这里使用 "IsLocalPlayerController()"
    if (Role < ROLE_Authority)
    {
        Server_IncreaseVariable();
    }
}
```

```cpp
// 我们 AGameState 子类的 CPP 文件

// 此函数是必需的，UPROPERTY 宏中的 Replicated 修饰符会声明它，我们只需实现
void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // 告诉 UE 我们想要复制此变量
    DOREPLIFETIME(ATestGameState, OurVariable);
}

void ATestGameState::IncreaseVariable()
{
    OurVariable++;
}
```

这是相当多的代码。如果你还不理解某些函数的用法和命名，不用担心。接下来的章节将帮助你理解为什么要这样做。

---

### 3.6 AHUD

`AHUD` 类仅在每个客户端上可用，可以通过 `PlayerController` 访问。它由 `PlayerController` 自动生成。

在 UMG（Unreal Motion Graphics）发布之前，`AHUD` 类被用于在客户端视口中绘制文本、纹理等。

如今 `UserWidget` 已经 99% 替代了 HUD 类。

你仍然可以使用 `AHUD` 类来调试，或者将其作为一个独立区域来管理 `UserWidget` 的创建、显示、隐藏和销毁。

由于 HUD 不直接与多人游戏相关联，示例只会展示单人逻辑，所以我们对此类跳过示例。

---

### 3.7 UUserWidget（UMG 控件）

`UUserWidget` 用于 Epic Games 的 UI 系统，称为 Unreal Motion Graphics。

它们继承自 Slate，Slate 是一种用于在 C++ 中创建 UI 的语言，也用于虚幻引擎编辑器本身。

**控件只在本地可用。它们不会复制，也不应包含任何复制代码。** 理想情况下它们也不应包含任何游戏逻辑，但某些游戏可能需要。

要了解更多关于 UMG 和控件的信息，请使用上方提供的 API 链接。

我们已经在 `APawn` 示例中看过了控件的简单使用示例，所以这里不再重复。

---

*下一章：[Gameplay Framework + Network](https://cedric-neukirchen.net/docs/multiplayer-compendium/framework-and-network)*
