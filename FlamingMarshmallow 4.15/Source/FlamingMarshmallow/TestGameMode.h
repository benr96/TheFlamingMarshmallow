// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameModeBase.h"
#include "Item.h"
#include "ItemSpawnLoc.h"
#include "mallow.h"
#include "AI.h"
#include "EnemyCharacter.h"
#include "AISpawnLoc.h"
#include "UI_Controller.h"
#include "MHUD.h"
#include "TargetPointer.h"
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
	void SpawnItems();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyCharacter> BP_EnemyCharacter;

	TArray<AActor*> bp_enemies;

	TArray<AActor*> ItemSpawnLocations;//where items will be randomly spawned
	TArray<AActor*> mallows;
	TArray<FCoreItemData> ItemTemplates;//where the structs that are used to spawn items will be kept
	TArray<AItem*> Items;

	FCoreItemData appleRed;
	FCoreItemData appleBlue;
	FCoreItemData appleGreen;

	void SpawnAndAddAI();

	//TArray<AAI*> enemies;
	TArray<AEnemyCharacter*> enemies;
	TArray<AActor*> AISpawnLocations;
	Amallow* mainChar;
	//ATargetPointer* pointer;
	int maxEnemies;
};

