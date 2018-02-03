// Fill out your copyright notice in the Description page of Project Settings.

#include "PoolActorComponent.h"


// Sets default values for this component's properties
UPoolActorComponent::UPoolActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

AActor * UPoolActorComponent::Checkout()
{
	if (Pool.Num() < 1) 
	{
		UE_LOG(LogTemp, Error, TEXT("No NavMesh To Checkout!"));
		return nullptr; 
	}
	return Pool.Pop();
}


void UPoolActorComponent::Return(AActor * ActorToReturn)
{
	if (ActorToReturn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Actor To Return!"));
		return;
	}
	AddActorToPool(ActorToReturn);
}


void UPoolActorComponent::AddActorToPool(AActor * ActorToAdd)
{
	if (ActorToAdd == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Actor To Add!"));
		return;
	}
	Pool.Push(ActorToAdd);
}
