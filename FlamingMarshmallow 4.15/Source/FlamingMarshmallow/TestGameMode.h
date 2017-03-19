// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "TestGameMode.generated.h"

/**
 *
 */
UCLASS()
class FLAMINGMARSHMALLOW_API ATestGameMode : public AGameModeBase
{
	GENERATED_BODY()

	void BeginPlay();
	APawn* SpawnDefaultPawnFor();
};
