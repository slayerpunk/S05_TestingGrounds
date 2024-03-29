// Fill out your copyright notice in the Description page of Project Settings.

#include "S05_TestingGrounds.h"
#include "Gun.h"
#include "BallProjectile.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Animation/AnimInstance.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnNoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	RootComponent = FP_Gun;

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGun::OnFire(AActor* WhoShooted, ACharacter* Shooter)
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

		UWorld* const World = GetWorld();
		if (World != NULL)
		{			
			// spawn the projectile at the muzzle
			ABallProjectile* Projectile = World->SpawnActor<ABallProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			Projectile->WhoShooted = WhoShooted;
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{		
		ReportNoise(FireSound, FireSoundVolume, Shooter);
	}

	
	// try and play a firing animation if specified 
	if (FireAnimationTP != nullptr && AnimInstanceTP != nullptr)
	{
			AnimInstanceTP->Montage_Play(FireAnimationTP, 1.f);
	}
	if (FireAnimationFP != nullptr && AnimInstanceFP != nullptr)
	{
			AnimInstanceFP->Montage_Play(FireAnimationFP, 1.f);
	}
}

void AGun::ReportNoise(USoundBase * SoundToPlay, float Volume, ACharacter* Shooter)
{
	//If we have a valid sound to play, play the sound and
	//report it to our game
	if (SoundToPlay)
	{
		//Play the actual sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation(), Volume);

		//Report that we've played a sound with a certain volume in a specific location
		MakeNoise(Volume, Shooter, GetActorLocation());
	}
}