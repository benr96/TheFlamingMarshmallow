// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameModeBase.h"
#include "Item.h"
#include "ItemSpawnLoc.h"
#include "TestGameMode.generated.h"

UCLASS()
class FLAMINGMARSHMALLOW_API ATestGameMode : public AGameModeBase
{
	GENERATED_BODY()
	ATestGameMode();
	void BeginPlay();
	APawn* SpawnDefaultPawnFor();

	TArray<AActor*> ItemSpawnLocations;

	FCoreItemData Cone;
	FCoreItemData Cube;
	FCoreItemData Sphere;
};
