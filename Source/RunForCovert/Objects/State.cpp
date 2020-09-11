// Caps Collective 2020


#include "State.h"

void UState::Initialise(AEnemyAIController* ObjectOwner)
{
    // Set ownership
    Owner = ObjectOwner;
}

void UState::OnEnter()
{
    // Virtual base method definition
}

void UState::OnExit()
{
    // Virtual base method definition
}

void UState::OnUpdate()
{
    // Virtual base method definition
}

UClass* UState::ToTransition() const
{
    // Virtual base method definition
    return nullptr;
}