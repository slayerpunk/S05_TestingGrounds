// Fill out your copyright notice in the Description page of Project Settings.

#include "S05_TestingGrounds.h"
#include "DrawDebugHelpers.h"
#include "InfiniteTerrainGameMode.h"
#include "ActorPool.h"
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
void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: EndPlay"), *GetName())
	Pool->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetPool(UActorPool * ActorPool)
{
	
	Pool = ActorPool;

	PositionNavMeshBoundsVolume();

}

void ATile::PositionNavMeshBoundsVolume()
{
	UE_LOG(LogTemp, Warning, TEXT("PositionNavMesh Called by %s"), *GetName())
	NavMeshBoundsVolume = Pool->CheckOut();
	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough NavMeshVolumes"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s get NavMesh %s and set location %s"),*GetName(),*NavMeshBoundsVolume->GetName(), *GetActorLocation().ToString());
	
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavMeshBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);

	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
}


TArray<FSpawnPosition> ATile::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions;
	int ItemsToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int i = 0; i < ItemsToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		if (FindEmptyLocation(SpawnPosition.Location, Radius))
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
			SpawnPositions.Add(SpawnPosition);
		}
	}
		
	return SpawnPositions;
}

bool ATile::FindEmptyLocation(FVector &SpawnLocation, float Radius)
{
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 25;
	for (int i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (IsCanSpawnAtLocation(CandidatePoint, Radius))
		{
			SpawnLocation = CandidatePoint;			
			return true;
		}		
	}
	return false;
}
void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition &SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	
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

