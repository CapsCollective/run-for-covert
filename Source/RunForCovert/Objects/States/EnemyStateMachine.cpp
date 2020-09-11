// Caps Collective 2020


#include "EnemyStateMachine.h"
#include "CombatStateMachine.h"
#include "../States/PatrolState.h"


void UEnemyStateMachine::Initialise()
{
    // Create state objects
    UCombatStateMachine* CombatStateMachine = NewObject<UCombatStateMachine>();
    CombatStateMachine->Initialise();

    States = {
            NewObject<UPatrolState>(),
            CombatStateMachine
    };

    // Initialise to first state
    Super::Initialise();
}
