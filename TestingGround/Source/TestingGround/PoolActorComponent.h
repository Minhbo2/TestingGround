// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUND_API UPoolActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoolActorComponent();



	AActor* Checkout();

	void Return(AActor* ActorToReturn);

	void AddActorToPool(AActor* ActorToAdd);

private:

	TArray<AActor*> Pool;
};
