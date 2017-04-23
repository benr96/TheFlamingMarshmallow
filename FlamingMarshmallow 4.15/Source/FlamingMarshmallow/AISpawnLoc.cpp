// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "AISpawnLoc.h"


// Sets default values
AAISpawnLoc::AAISpawnLoc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spawnLoc = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = spawnLoc;

	bOccupied = false;
}

// Called when the game starts or when spawned
void AAISpawnLoc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAISpawnLoc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	location = GetActorLocation();
}

