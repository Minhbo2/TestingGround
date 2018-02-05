// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


USTRUCT(BlueprintType)
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinSpawn = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxSpawn = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinScale = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxScale = 1;
};



class ANavMeshBoundsVolume;
class UPoolActorComponent;



UCLASS()
class TESTINGGROUND_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(EditDefaultsOnly, Category = "AI Tag")
	FName AITag;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector TileNavMeshVolumeOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtend;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtend;


	UFUNCTION(BlueprintCallable, Category = SpawnProps)
	void PlaceActors(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

	UFUNCTION(BlueprintCallable, Category = SpawnProps)
	void SetPool(UPoolActorComponent* Pool);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceAIPawns (TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);



protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	bool HasBeenConquered = false;

	AActor* TileNavMesh = nullptr;

	void PositionNavMesh();

	UPoolActorComponent* PoolActorComp = nullptr;

	bool IsLocationEmpty(FVector Location, float Radius);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	AActor* PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

	FSpawnPosition RandomSpawnPositions(FSpawnPosition SpawnPosition);


	UFUNCTION(BlueprintCallable, Category = "Score")
	void TileConquered();

};
