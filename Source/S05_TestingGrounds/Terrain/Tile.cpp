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

void ATile::SetPool(UActorPool * ActorPool)
{
	Pool = ActorPool;
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->CheckOut();
	if (!NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough NavMeshVolumes"));
		return;
	}
	
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavMeshBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Pool && NavMeshBoundsVolume)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: EndPlay"), *GetName())
		Pool->Return(NavMeshBoundsVolume);
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	RandomlyActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	RandomlyActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}

template<class T>
inline void ATile::RandomlyActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	int ItemsToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (int i = 0; i < ItemsToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		if (FindEmptyLocation(SpawnPosition.Location, Radius))
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
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
	return !HasHit;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition &SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPosition.Location, Rotation);
	if (Spawned)
	{
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
		ActorsArray.Add(Spawned);
	}	
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition & SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location , Rotation);
	if (Spawned)
	{
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Enemy"));
		ActorsArray.Add(Spawned);
	}
}

void ATile::DestroyActorsOnTile()
{
	for (AActor* FoundActor : ActorsArray)
	{
		FoundActor->Destroy();
	}
	
}