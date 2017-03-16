// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "TestGameMode.h"
#include "mallow.h"


void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();
	DefaultPawnClass = Amallow::StaticClass();

	Amallow* NewMallow = GetWorld()->SpawnActor<Amallow>(Amallow::StaticClass());
}

APawn* ATestGameMode::SpawnDefaultPawnFor()
{
	return NULL;
}


