// Fill out your copyright notice in the Description page of Project Settings.

#include "S05_TestingGrounds.h"
#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor * UActorPool::CheckOut()
{
	
	if (Pool.Num() <= 0) 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Try to Pop absent element"));
		return nullptr;
	}
	return Pool.Pop(false);
}


void UActorPool::Return(AActor * ActortoReturn)
{
	if (!ActortoReturn) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Try to return nullptr"));
		return; 
	}
	Pool.Push(ActortoReturn);
}

void UActorPool::Add(AActor * ActorToAdd)
{
	if (!ActorToAdd)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Try to add nullptr"));
		return;
	}
	Pool.Push(ActorToAdd);
}
