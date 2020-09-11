// Caps Collective 2020


#include "EnemyStateMachine.h"
#include "CombatStateMachine.h"
#include "../States/PatrolState.h"


void UEnemyStateMachine::Initialise(AEnemyAIController* ObjectOwner)
{
    Owner = ObjectOwner;
    
    // Create state objects
    UCombatStateMachine* CombatStateMachine = NewObject<UCombatStateMachine>();
    CombatStateMachine->Initialise(Owner);
    UPatrolState* PatrolState = NewObject<UPatrolState>();
    PatrolState->Initialise(Owner);

    States = {
            PatrolState,
            CombatStateMachine
    };

    // Initialise to first state
    Super::Initialise(Owner);
}
