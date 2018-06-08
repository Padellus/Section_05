// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "PatrollingGuard.h" // TODO remove coupling
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get patrol points
	auto AIController = OwnerComp.GetAIOwner();
	auto ControlledPawn = AIController->GetPawn();
	auto PatrollingGuard = Cast<APatrollingGuard>(ControlledPawn);
	if (!PatrollingGuard) return EBTNodeResult::Failed;
	auto PatrolPoints = PatrollingGuard->PatrolPoints;
	if (PatrolPoints.Num() == 0) return EBTNodeResult::Failed;

	// Set next waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	UE_LOG(LogTemp, Warning, TEXT("UChooseNextWaypoint::ExecuteTask %s Index: %i"), *ControlledPawn->GetName(), Index);

	// Cycle index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}

