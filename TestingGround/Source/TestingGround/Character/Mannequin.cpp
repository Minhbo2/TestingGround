// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "../Weapon/Gun.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"



// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}


// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();
	

	if (BP_Gun == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Gun blueprint missing."));
		return;
	}

	Gun = GetWorld()->SpawnActor<AGun>(BP_Gun);
	// IsPlayerController is a function deprived from APawn class to check if this character is being controlled by the player controller. 
	if (IsPlayerControlled())
	{
		Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	}
	else
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_0"));
	}
	Gun->AnimInstance1P = Mesh1P->GetAnimInstance();
	Gun->AnimInstance3P = GetMesh()->GetAnimInstance();



	// We do binding here because SetupPlayerInputComponent runs before beginplay, and we dont have a gun 
	// InputComponent is inherited from ACharacter class
	if(InputComponent != NULL)
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_0"));
	
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(
		Timer, 
		this,
		&AMannequin::OnTimerExpire,
		DestroyOnDelay,
		false
	);
}


void AMannequin::OnTimerExpire()
{
	Destroy();
}


// Garbage cleaning here because OnTimerExpire only work for Player Pawn.
void AMannequin::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	// Tile only destroy AI Pawn but not their gun, so need to let the Pawn to do garbage cleaning as well
	Gun->Destroy();
}



// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}


void AMannequin::PullTrigger()
{
	Gun->OnFire();
}

