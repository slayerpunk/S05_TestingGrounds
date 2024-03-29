// Fill out your copyright notice in the Description page of Project Settings.

#include "S05_TestingGrounds.h"
#include "EngineUtils.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "ActorPool.h"
#include "InfiniteTerrainGameMode.h"

AInfiniteTerrainGameMode::AInfiniteTerrainGameMode()
{
	NavMeshPool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Volume Bounds Pool"));
}


void AInfiniteTerrainGameMode::PopulateBoundsVolumePool()
{
	
	
	TActorIterator<ANavMeshBoundsVolume> NavMeshIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (NavMeshIterator)
	{
		AddToPool(*NavMeshIterator);
		++NavMeshIterator;

	}
}

void AInfiniteTerrainGameMode::TileConquered()
{
	Score++;
}

UActorPool * AInfiniteTerrainGameMode::GetNavMeshPool() const
{
	return NavMeshPool;
}

void AInfiniteTerrainGameMode::AddToPool(ANavMeshBoundsVolume * VolumeToAdd)
{
	if (!NavMeshPool)
	{ 
		return; 
	}
	NavMeshPool->Add(VolumeToAdd);
}
