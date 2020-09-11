// Caps Collective 2020


#include "State.h"

void UState::Initialise()
{
    // Virtual base method definition
}

void UState::OnEnter(AEnemyAIController& Owner)
{
    // Virtual base method definition
}

void UState::OnExit(AEnemyAIController& Owner)
{
    // Virtual base method definition
}

void UState::OnUpdate(AEnemyAIController& Owner)
{
    // Virtual base method definition
}

UClass* UState::ToTransition(AEnemyAIController& Owner) const
{
    // Virtual base method definition
    return nullptr;
}