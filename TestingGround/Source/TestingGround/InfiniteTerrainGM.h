// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "InfiniteTerrainGM.generated.h"

class ANavMeshBoundsVolume;

UCLASS()
class TESTINGGROUND_API AInfiniteTerrainGM : public AGameMode
{
	GENERATED_BODY()
	
	
public: 
	AInfiniteTerrainGM();



	UFUNCTION(BlueprintCallable, Category = "Pool")
	void PopulateBoundsVolume();


protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	class UPoolActorComponent* PoolActor = nullptr;

private:

	void AddToPool(ANavMeshBoundsVolume *VolumeToAdd);
};
