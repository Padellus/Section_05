// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor* UActorPool::Checkout()
{
	if (Pool.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Trying to checkout from actor pool, but pool is empty."));
		return nullptr;
	}
	UE_LOG(LogTemp, Warning, TEXT("ActorPool Checkout"));
	return Pool.Pop(false);
}

void UActorPool::Return(AActor* ActorToReturn)
{
	Add(ActorToReturn);
}

void UActorPool::Add(AActor* ActorToAdd)
{
	if (!ActorToAdd) {
		UE_LOG(LogTemp, Error, TEXT("Trying to add null to actor pool."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("ActorPool Add"));
	Pool.Push(ActorToAdd);
}


