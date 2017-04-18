// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "ItemSpawnLoc.h"


// Sets default values
AItemSpawnLoc::AItemSpawnLoc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bOccupied = false;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = sphere;

}

// Called when the game starts or when spawned
void AItemSpawnLoc::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemSpawnLoc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	location = GetActorLocation();
}

