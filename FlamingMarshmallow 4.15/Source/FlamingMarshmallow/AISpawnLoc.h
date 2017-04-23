// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AISpawnLoc.generated.h"

UCLASS()
class FLAMINGMARSHMALLOW_API AAISpawnLoc : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISpawnLoc();

	FVector location;
	bool bOccupied;
	UBoxComponent *spawnLoc;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
