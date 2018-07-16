// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "../ActorPool.h"
#include "WorldCollision.h"
#include "DrawDebugHelpers.h"
#include "AI/Navigation/NavigationSystem.h"


// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoundsMin = FVector(0, -2000, 0);
	BoundsMax = FVector(4000, 2000, 0);

	NavigationBoundsOffset = FVector(2000, 0, 0);
}

void ATile::SetPool(UActorPool* InPool)
{
	Pool = InPool;

	NavMeshBoundsVolume = Pool->Checkout();
	if (!NavMeshBoundsVolume) {
		UE_LOG(LogTemp, Error, TEXT("Not enough actors in pool."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	FVector TileCenter = GetActorLocation() + NavigationBoundsOffset;
	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting NavMeshBoundsVolume location to: {%s}"), *GetName(), *TileCenter.ToString());
	NavMeshBoundsVolume->SetActorLocation(TileCenter);
	GetWorld()->GetNavigationSystem()->Build();
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Pool->Return(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(TSubclassOf<AActor> Spawnable, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions = GenerateRandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceActor(Spawnable, SpawnPosition);
	}
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> Spawnable, int MinSpawn, int MaxSpawn, float Radius)
{
	TArray<FSpawnPosition> SpawnPositions = GenerateRandomSpawnPositions(MinSpawn, MaxSpawn, Radius, 1, 1);
	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceAIPawn(Spawnable, SpawnPosition);
	}
}

TArray<FSpawnPosition> ATile::GenerateRandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions;
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool bFound = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (bFound) {
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			SpawnPositions.Add(SpawnPosition);
		}
	}
	return SpawnPositions;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds = FBox(BoundsMin, BoundsMax);
	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector PossibleSpawnPoint = FMath::RandPointInBox(Bounds);
		bool bSpawnPointCollision = CastSphere(PossibleSpawnPoint, Radius);
		if (!bSpawnPointCollision) {
			OutLocation = PossibleSpawnPoint;
			return true;
		}
	}
	return false;
}

void ATile::PlaceActor(TSubclassOf<AActor> Spawnable, FSpawnPosition SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(Spawnable);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	return;
}

void ATile::PlaceAIPawn(TSubclassOf<APawn> Spawnable, FSpawnPosition SpawnPosition)
{
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(Spawnable);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SpawnDefaultController();
	Spawned->Tags.Add(FName("Enemy"));
	return;
}

bool ATile::CastSphere(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	//FColor DebugSphereColor = bHit ? FColor::Red : FColor::Green;
	//DrawDebugCapsule(
	//	GetWorld(),
	//	GlobalLocation,
	//	0,
	//	Radius,
	//	FQuat::Identity,
	//	DebugSphereColor,
	//	true,
	//	100
	//);
	return bHit;
}

