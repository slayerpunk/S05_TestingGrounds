// Fill out your copyright notice in the Description page of Project Settings.

#include "S05_TestingGrounds.h"
#include "../Weapons/Gun.h"
#include "GameFramework/InputSettings.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Mannequin.h"



// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnNoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(3.f, 0.f, 64.f);		// Position the camera
	FirstPersonCameraComponent->RelativeRotation = FRotator(0.f, 0.f, 10.f);	// Position the camera
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

	if (GunBlueprint == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Gun blueprint missing."));
		return;
	}
	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	if (IsPlayerControlled())
	{
		Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	}
	else
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_0")); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	}
	Gun->AnimInstanceFP = Mesh1P->GetAnimInstance();
 	Gun->AnimInstanceTP = GetMesh()->GetAnimInstance();
	if (InputComponent != nullptr)
	{
		//InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
	}
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	Mesh1P->DestroyComponent(true);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_0")); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::PullTrigger(AActor* WhoShooted)
{
	Gun->OnFire(WhoShooted, this);
}

void AMannequin::ReportNoise(USoundBase * SoundToPlay, float Volume)
{
	//If we have a valid sound to play, play the sound and
	//report it to our game
	if (SoundToPlay)
	{
		//Play the actual sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation(), Volume);

		//Report that we've played a sound with a certain volume in a specific location
		MakeNoise(Volume, this, GetActorLocation());
	}
}
