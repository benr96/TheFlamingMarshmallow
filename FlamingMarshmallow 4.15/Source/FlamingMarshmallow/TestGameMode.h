// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameModeBase.h"
#include "Item.h"
#include "ItemSpawnLoc.h"
#include "mallow.h"
#include "AI.h"
#include "UI_Controller.h"
#include "MHUD.h"
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
	void GetMallow();
	void GetItemSpawnLocations();
	void cleanItemSettings();

	TArray<AActor*> ItemSpawnLocations;//where items will be randomly spawned
	TArray<AActor*> mallows;
	TArray<FCoreItemData> ItemTemplates;//where the structs that are used to spawn items will be kept
	TArray<AItem*> Items;//actual item object spawned in the game

	FCoreItemData Cone;
	FCoreItemData Cube;
	FCoreItemData Sphere;

	void SpawnAndAddAI(float);

	TArray<AAI*> enemies;
	Amallow* mainChar;
};
