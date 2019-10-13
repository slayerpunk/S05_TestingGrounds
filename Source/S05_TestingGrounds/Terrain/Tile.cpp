// Fill out your copyright notice in the Description page of Project Settings.

#include "S05_TestingGrounds.h"
#include "Tile.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::PlaceActors()
{
	FVector Min = FVector(0.f, -2000.f, -119.f);
	FVector Max = FVector(4000.f, 2000.f, -119.f);
	FBox Bounds(Min, Max);
	FVector RandPoint;
	for (int i = 0; i < 20; i++)
	{
		RandPoint = FMath::RandPointInBox(Bounds);
		UE_LOG(LogTemp, Warning, TEXT("RandPoint is %s"), *RandPoint.ToString());
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

