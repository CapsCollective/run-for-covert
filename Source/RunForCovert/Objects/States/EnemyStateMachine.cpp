// Caps Collective 2020


#include "EnemyStateMachine.h"
#include "CombatStateMachine.h"
#include "../States/PatrolState.h"
#include "../Transitions/PlayerSeenTransition.h"

void UEnemyStateMachine::Initialise()
{
    // Create state objects
    UPatrolState* PatrolState = NewObject<UPatrolState>();
    UCombatStateMachine* CombatStateMachine = NewObject<UCombatStateMachine>();
    CombatStateMachine->Initialise();

    // Add state transitions to state machine
    StateTransitions.Add(PatrolState, {
            TPair<UTransition*, UState*>(NewObject<UPlayerSeenTransition>(), CombatStateMachine),
    });
    StateTransitions.Add(CombatStateMachine, {
            //TPair<UTransition*, UState*>(NewObject<UPlayerUnseenTransition>(), PatrolState),
    });

    // Initialise to first state
    Super::Initialise();
}