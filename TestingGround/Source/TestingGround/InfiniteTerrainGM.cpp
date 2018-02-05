// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGM.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "EngineUtils.h"
#include "PoolActorComponent.h"
#include "TestingGroundHUD.h"
#include "UObject/ConstructorHelpers.h"


AInfiniteTerrainGM::AInfiniteTerrainGM()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/Behaviour/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestingGroundHUD::StaticClass();

	PoolActor = CreateDefaultSubobject<UPoolActorComponent>(TEXT("PoolActorComponent"));
}




//Responsible for populate the PoolActorComponent with NavMeshBoundsVolume in the world on BeginPlay

void AInfiniteTerrainGM::PopulateBoundsVolume()
{
	TActorIterator<ANavMeshBoundsVolume> VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (VolumeIterator)
	{
		AddToPool(*VolumeIterator);
		++VolumeIterator;
	}
}


void AInfiniteTerrainGM::AddToPool(ANavMeshBoundsVolume *VolumeToAdd)
{
	PoolActor->AddActorToPool(VolumeToAdd);
}



void AInfiniteTerrainGM::AddToScore()
{
	Score++;
}

int32 AInfiniteTerrainGM::GetScore() const
{
	return Score;
}
