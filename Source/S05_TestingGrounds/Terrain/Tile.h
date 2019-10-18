// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"
 
USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

class UActorPool;
UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.f, float MinScale = 1.f, float MaxScale = 1.f);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent = FVector(0.f, -2000.f, 0.f);
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent = FVector(4000.f, 2000.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector NavMeshBoundsOffset = FVector(2000.f, 0.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector AIOffset = FVector(0.f, 0.f, 100.f);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* ActorPool);

	

private:

	void PositionNavMeshBoundsVolume();
	
	TArray<FSpawnPosition> RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale);

	bool FindEmptyLocation(FVector &SpawnLocation, float Radius);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition &SpawnPosition);
	
	bool IsCanSpawnAtLocation(FVector Location, float Radius);
	
	void PlaceAI(TSubclassOf<APawn> ToSpawn, FSpawnPosition &SpawnPosition);

	UActorPool* Pool = nullptr;
	
	AActor* NavMeshBoundsVolume = nullptr;
	
};
