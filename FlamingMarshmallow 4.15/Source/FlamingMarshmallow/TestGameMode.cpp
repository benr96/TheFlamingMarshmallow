// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "TestGameMode.h"
#include "mallow.h"
#include "ShrineSP.h"

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();
	DefaultPawnClass = Amallow::StaticClass();

	Amallow* mainChar = GetWorld()->SpawnActor<Amallow>(Amallow::StaticClass());
	mainChar->SetActorRelativeLocation(FVector(-600, 0, 100));
}

APawn* ATestGameMode::SpawnDefaultPawnFor()
{
	return NULL;
}
