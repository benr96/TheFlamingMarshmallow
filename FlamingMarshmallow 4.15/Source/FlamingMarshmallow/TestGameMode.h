// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameModeBase.h"
#include "Item.h"
#include "ItemSpawnLoc.h"
#include "mallow.h"
#include "AI.h"
#include "TestGameMode.generated.h"

UCLASS()
class FLAMINGMARSHMALLOW_API ATestGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATestGameMode();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	APawn* SpawnDefaultPawnFor();

	TArray<AActor*> ItemSpawnLocations;

	FCoreItemData Cone;
	FCoreItemData Cube;
	FCoreItemData Sphere;

	void SpawnAndAddAI(float);

	TArray<AAI*> enemies;
	Amallow* mainChar;
};
