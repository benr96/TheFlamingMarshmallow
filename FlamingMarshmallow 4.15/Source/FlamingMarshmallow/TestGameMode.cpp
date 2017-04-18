// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "TestGameMode.h"
#include "mallow.h"
#include "ShrineSP.h"
#include "AI.h"

ATestGameMode::ATestGameMode()
{
	static ConstructorHelpers::FClassFinder<AHUD> MHUDClass(TEXT("/Game/MHUD"));

	if (MHUDClass.Class != NULL)
	{
		HUDClass = MHUDClass.Class;
	}	
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();
	DefaultPawnClass = Amallow::StaticClass();

	mainChar = GetWorld()->SpawnActor<Amallow>(Amallow::StaticClass());
	mainChar->SetActorRelativeLocation(FVector(-600, 0, 100));
	
	float i = 1;

	while (enemies.Num() < 5)
	{
		SpawnAndAddAI(i);
		i++;
	}
}

void ATestGameMode::SpawnAndAddAI(float i)
{
	AAI* testAI = GetWorld()->SpawnActor<AAI>(AAI::StaticClass());
	testAI->SetActorLocation(FVector(i*60.f, i*10.f, 50.f));
	if (enemies.Num() % 2 == 0)
	{
		testAI->left *= -1;
	}
	enemies.Add(testAI);
	mainChar->TestAI.Add(testAI);
	UE_LOG(LogTemp, Warning, TEXT("%d"), enemies.Num());
}

// Called every frame
void ATestGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

APawn* ATestGameMode::SpawnDefaultPawnFor()
{
	return NULL;
}
