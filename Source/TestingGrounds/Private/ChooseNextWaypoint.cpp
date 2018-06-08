// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "PatrollingGuard.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();

	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	UE_LOG(LogTemp, Warning, TEXT("UChooseNextWaypoint::ExecuteTask Index: %i"), Index);

	auto AIController = OwnerComp.GetAIOwner();
	auto PatrollingGuard = Cast<APatrollingGuard>(AIController);
	if (!PatrollingGuard) return EBTNodeResult::Failed;
	auto PatrolPoints = PatrollingGuard->PatrolPoints;
	if (!PatrolPoints) return EBTNodeResult::Failed;

	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints.Get(Index));

	auto NextIndex = (Index + 1) % PatrolPoints.Length;
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}

