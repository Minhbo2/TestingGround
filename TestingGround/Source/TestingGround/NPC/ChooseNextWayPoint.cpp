// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWayPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"


// Note that this function is called by the BT, not every frame by the engine
EBTNodeResult::Type UChooseNextWayPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// Get the controlled Pawn
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	// Cast the Pawn to the safty type or it will just be a pawn and not the PG
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }



	//=====================================================
	// Get patrol points
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();

	// protect of empty patrol points
	if(PatrolPoints.Num() >= 0)
		UE_LOG(LogTemp, Warning, TEXT("No Patrol Point"));

	auto BlackboardComp = OwnerComp.GetBlackboardComponent();



	//==================================================
	// IndexKey must set in BT ChooseNextWayPoint Node
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);

	// set next way point
	BlackboardComp->SetValueAsObject(WayPoint.SelectedKeyName, PatrolPoints[Index]);



	//==================================================
	// Cycle Index 
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);


	return EBTNodeResult::Succeeded;
}
