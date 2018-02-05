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


	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddToScore();

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const;


protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	class UPoolActorComponent* PoolActor = nullptr;

private:

	int Score = 0;

	void AddToPool(ANavMeshBoundsVolume *VolumeToAdd);
};
