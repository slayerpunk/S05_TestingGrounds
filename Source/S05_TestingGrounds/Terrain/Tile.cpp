// Fill out your copyright notice in the Description page of Project Settings.

#include "S05_TestingGrounds.h"
#include "DrawDebugHelpers.h"
#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{

// 	
	int ItemsToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);	
	for (int i = 0; i < ItemsToSpawn; i++)
	{
		FVector SpawnLocation;		
		if (FindEmptyLocation(SpawnLocation, Radius))
		{
			float RandomRotation = FMath::RandRange(-180.f, 180.f);
			float RandomScale = FMath::RandRange(MinScale, MaxScale);
			PlaceActor(ToSpawn, SpawnLocation, RandomRotation, RandomScale);
		}		
 	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	 	FVector Min = FVector(0.f, -2000.f, 0.f);
	 	FVector Max = FVector(4000.f, 2000.f, 0.f);
	 	FBox Bounds(Min, Max);
		const int MAX_ATTEMPTS = 25;
		for (int i = 0; i < MAX_ATTEMPTS; i++)
		{
			FVector CandidatePoint = FMath::RandPointInBox(Bounds);
			if (IsCanSpawnAtLocation(CandidatePoint, Radius))
			{
				OutLocation = CandidatePoint;
				return true;
			}
		}
		return false;
}
void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnLocation, float Rotation, float Scale)
{
	 AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	 Spawned->SetActorRelativeLocation(SpawnLocation);
	 Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	 Spawned->SetActorRotation(FRotator(0, Rotation, 0));
	 Spawned->SetActorScale3D(FVector(Scale));
}



// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATile::IsCanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;

	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
// 	FColor Color;
// 
// 
// 		Color = HasHit ? FColor::Red : FColor::Green;
// 
// 	DrawDebugCapsule(
// 		GetWorld(), 
// 		GlobalLocation,
// 		0,
// 		Radius, 
// 		FQuat::Identity, 
// 		Color, 
// 		true, 
// 		-1,
// 		0,
// 		2
// 	);

	return !HasHit;
}

