// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UActorPool;

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Generate")
	FVector BoundsMin;

	UPROPERTY(EditDefaultsOnly, Category = "Generate")
	FVector BoundsMax;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Generate")
	void PlaceActors(TSubclassOf<AActor> Spawnable, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable, Category = "Generate")
	void PlaceAIPawns(TSubclassOf<APawn> Spawnable, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);
	
private:
	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> Spawnable, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500, float MinScale = 1, float MaxScale = 1);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	void PlaceActor(TSubclassOf<AActor> Spawnable, FSpawnPosition SpawnPosition);

	void PlaceActor(TSubclassOf<APawn> Spawnable, FSpawnPosition SpawnPosition);

	bool CastSphere(FVector Location, float Radius);

	UPROPERTY()
	UActorPool* Pool;

	UPROPERTY()
	AActor* NavMeshBoundsVolume;
};
