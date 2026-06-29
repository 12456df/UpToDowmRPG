


#include "Game/AuraGameModeBase.h"
#include "Player/AuraPlayerState.h"

AAuraGameModeBase::AAuraGameModeBase()
{
    PlayerStateClass = AAuraPlayerState::StaticClass();
}


//TODO:地牢中的建筑需要修改对projectile的碰撞，重叠事件的处理，对可视化，Camera的忽略