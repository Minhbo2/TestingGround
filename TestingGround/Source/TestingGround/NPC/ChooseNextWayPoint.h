// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseNextWayPoint.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUND_API UChooseNextWayPoint : public UBTTaskNode
{
	GENERATED_BODY()


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
protected:

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector IndexKey;

	UPROPERTY(EditAnywhere, Category = "BlackBoard")
	struct FBlackboardKeySelector WayPoint;
};
