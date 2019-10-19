// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "S05_TestingGroundsGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */

class UActorPool;

UCLASS()
class S05_TESTINGGROUNDS_API AInfiniteTerrainGameMode : public AS05_TestingGroundsGameMode
{
	GENERATED_BODY()
	
public:
	AInfiniteTerrainGameMode();
	
	UFUNCTION(BlueprintCallable, Category = "Actors pool")
	void PopulateBoundsVolumePool();

	UFUNCTION(BlueprintCallable, Category = "Game Stats")
	void TileConquered();

	UActorPool*	GetNavMeshPool() const;
private:
	void AddToPool(class ANavMeshBoundsVolume *VolumeToAdd);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	UActorPool *NavMeshPool = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Game Stats")
	int32 Score = 0;

};
