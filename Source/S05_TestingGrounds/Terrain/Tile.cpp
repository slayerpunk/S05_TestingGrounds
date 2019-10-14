// Fill out your copyright notice in the Description page of Project Settings.

#include "S05_TestingGrounds.h"
#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn)
{
	FVector Min = FVector(0.f, -2000.f, 0.f);
	FVector Max = FVector(4000.f, 2000.f, 0.f);
	FBox Bounds(Min, Max);
	
	int ItemsToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);	
	for (int i = 0; i < ItemsToSpawn; i++)
	{
		float Rotation = FMath::RandRange(-180.f, 180.f);
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		SpawnRotation.Yaw = Rotation;
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnPoint, SpawnRotation);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	}

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

