// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "TestGameMode.h"
#include "mallow.h"
#include "ShrineSP.h"
#include "AI.h"


#include "EngineUtils.h"
#include "MHUD.h"

ATestGameMode::ATestGameMode()
{
		HUDClass = AMHUD::StaticClass();

}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (TObjectIterator<ACharacter> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		FString name = Itr->GetName();

		if (name == "Main")
		{
			Itr->AutoPossessPlayer = EAutoReceiveInput::Player0;
			Itr->AutoReceiveInput = EAutoReceiveInput::Player0;
		}
	}
	

	

}

APawn* ATestGameMode::SpawnDefaultPawnFor()
{
	return NULL;
}
