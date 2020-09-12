// Caps Collective 2020


#include "EnemyStateMachine.h"
#include "CombatStateMachine.h"
#include "../States/PatrolState.h"


void UEnemyStateMachine::Initialise(AEnemyAIController* ObjectOwner)
{
    // Create state objects
    UCombatStateMachine* CombatStateMachine = NewObject<UCombatStateMachine>();
    CombatStateMachine->Initialise(Owner);
    States = {
            NewObject<UPatrolState>(),
            CombatStateMachine
    };

    // Initialise to first state
    Super::Initialise(ObjectOwner);
}
