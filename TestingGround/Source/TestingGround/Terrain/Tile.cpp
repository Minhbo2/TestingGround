// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PoolActorComponent.h"
#include "Engine/World.h"
#include "AI/Navigation/NavigationSystem.h"
#include "../InfiniteTerrainGM.h"



// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MinExtend = FVector(0, -2000, 0);
	MaxExtend = FVector(4000, 2000, 0);
	TileNavMeshVolumeOffset = FVector(2000, 0,0);
}


/*
	NavMeshBoundsVolume Config
	=============================================
*/

// set reference to the pool component
void ATile::SetPool(UPoolActorComponent * Pool)
{
	PoolActorComp = Pool;
	PositionNavMesh();
}



// checkout a navmesh from the pool component and set its position
void ATile::PositionNavMesh()
{
	TileNavMesh = PoolActorComp->Checkout();
	if (TileNavMesh == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No NavMesh"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *this->GetName(), *(TileNavMesh->GetName()));
	TileNavMesh->SetActorLocation(GetActorLocation() + TileNavMeshVolumeOffset);
	GetWorld()->GetNavigationSystem()->Build();
}


// On this Tile is destroyed, return the navmesh to the pool
void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PoolActorComp->Return(TileNavMesh);
	CleanTile();
}
//END Config
//============================================================






/*
	Environment/Props Spawn Config
	==============================================
*/

// calculate values for SpawnPosition and pass it to placeactor
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	if (ToSpawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mission Actor to Spawn!"));
		return;
	}

	int MinSpawn = SpawnPosition.MinSpawn;
	int MaxSpawn = SpawnPosition.MaxSpawn;

	for (size_t i = MinSpawn; i < MaxSpawn; i++)
	{
		FSpawnPosition NewSpawnPosition = RandomSpawnPositions(SpawnPosition);
		PlaceActor(ToSpawn, NewSpawnPosition);
	}
}


// spawning AI
void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	if (ToSpawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mission Pawn to Spawn!"));
		return;
	}

	int MinSpawn = SpawnPosition.MinSpawn;
	int MaxSpawn = SpawnPosition.MaxSpawn;

	for (size_t i = MinSpawn; i < MaxSpawn; i++)
	{
		FSpawnPosition NewSpawnPosition = RandomSpawnPositions(SpawnPosition);
		APawn* AIPawn = Cast<APawn>(PlaceActor(ToSpawn, NewSpawnPosition));
		if (AIPawn == nullptr) { return; }
		AIPawn->Tags.Add(AITag);
		if (!AIPawn->GetController())
			AIPawn->SpawnDefaultController();
	}
}



// return a set of SpawnPosition struct array with all outputs defined
FSpawnPosition ATile::RandomSpawnPositions(FSpawnPosition SpawnPosition)
{
	FSpawnPosition NewSpawnPosition;

	NewSpawnPosition.Scale = FMath::RandRange(SpawnPosition.MinScale, SpawnPosition.MaxScale);
	bool Found = FindEmptyLocation(NewSpawnPosition.Location, SpawnPosition.Radius * NewSpawnPosition.Scale);
	if (Found)
		NewSpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);

	return NewSpawnPosition;
}



// Choose a random location in the tile and check if it is empty
bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinExtend, MaxExtend);
	const int MAX_ATTEMPS = 100;
	for (auto i = 0; i < MAX_ATTEMPS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (IsLocationEmpty(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
}



// cast a sphere sweep at location to check if there is no actor there
bool ATile::IsLocationEmpty(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, //store hit result 
		GlobalLocation, //start sweep location
		GlobalLocation, //end sweep location
		FQuat::Identity, //sweep rotation = none
		ECollisionChannel::ECC_GameTraceChannel2, //sweep by collision channel
		FCollisionShape::MakeSphere(Radius) //cast a shape type for sweeping
		);

	//if (HasHit)
	//{
	//	auto HitActor = HitResult.Actor->GetName();
	//	UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor);
	//}

	//	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	//	DrawDebugSphere(
	//		GetWorld(), //world context
	//		GlobalLocation, //location to draw
	//		Radius, //size of sphere
	//		12, //number of side-line to draw for sphere
	//		ResultColor, //color to draw
	//		true, //persistance
	//		100 //lifetime duration
	//	);

	return !HasHit;
}



// spawn, scale, rotate and position using value from SpawnPosition
AActor* ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
		AActor* SpawnedActor = GetWorld()->SpawnActor(ToSpawn);
		if (SpawnedActor == nullptr) 
		{ 
			UE_LOG(LogTemp, Warning, TEXT("[%s] has no actor: %s"), *(this->GetName()), *ToSpawn->GetName());
			return nullptr; 
		}

		SpawnedActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		SpawnedActor->SetActorRelativeLocation(SpawnPosition.Location);
		SpawnedActor->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
		SpawnedActor->SetActorScale3D(FVector(SpawnPosition.Scale));
		AttachActors.Add(SpawnedActor);
		return SpawnedActor;
}



void ATile::TileConquered()
{
	if (!HasBeenConquered)
	{
		AInfiniteTerrainGM* CurrentGM = Cast<AInfiniteTerrainGM>(GetWorld()->GetAuthGameMode());
		CurrentGM->AddToScore();
		UE_LOG(LogTemp, Warning, TEXT("Score: %d"), CurrentGM->GetScore());
		HasBeenConquered = true;
	}
}


// Garbage cleaning
void ATile::CleanTile()
{
	if (AttachActors.Num() < 1)
		return;

	for (AActor* Actor: AttachActors)
		Actor->Destroy();
}

//END Config
//====================================================

