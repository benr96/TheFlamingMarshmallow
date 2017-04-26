// Fill out your copyright notice in the Description page of Project Settings.
//TODO edit TArray enemies (Get rid of)
#include "FlamingMarshmallow.h"
#include "mallow.h"
#include "ShrineSP.h"
#include "AI.h"
#include "EnemyCharacter.h"
#include "EnemyAI.h"
#include "EngineUtils.h"
#include "TestGameMode.h"

ATestGameMode::ATestGameMode()
{
	HUDClass = AMHUD::StaticClass();

	//loading in assets for item spawning
	static ConstructorHelpers::FObjectFinder<UTexture> appleRedImage(TEXT("/Game/appleRed"));
	static ConstructorHelpers::FObjectFinder<UTexture> appleBlueImage(TEXT("/Game/appleBlue"));
	static ConstructorHelpers::FObjectFinder<UTexture> appleGreenImage(TEXT("/Game/appleGreen"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> appleBlueAsset(TEXT("/Game/WorldV3/Assets/demApplesBlue"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> appleRedAsset(TEXT("/Game/WorldV3/Assets/demApplesRed"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> appleGreenAsset(TEXT("/Game/WorldV3/Assets/demApplesGreen"));


	//setting up the structs used to make each item from, so all you need to make an item is fill out one of these structs
	//spawn the item class and pass the struct in to its initializer class
	appleRed.Name = "Healing item";
	appleRed.Mesh = appleRedAsset.Object;
	appleRed.InvImage = appleRedImage.Object;
	appleRed.scale = FVector(1, 1, 1);
	appleRed.offset = FVector(0, 0, -23);
	appleRed.Location = FVector(0, 0, 0);
	appleRed.respawnTime = 10;
	appleRed.Health = 20;//units
	appleRed.bFood = true;

	appleBlue.Name = "Speed boost";
	appleBlue.Mesh = appleBlueAsset.Object;
	appleBlue.InvImage = appleBlueImage.Object;
	appleBlue.scale = FVector(1, 1, 1);
	appleBlue.offset = FVector(0, 0, -30);
	appleBlue.Location = FVector(0, 0, 0);
	appleBlue.respawnTime = 20;
	appleBlue.Speed = 0;
	appleBlue.bSpeed = true;
	appleBlue.Speed = 20;//%
	appleBlue.SpeedTime = 20;

	appleGreen.Name = "Damage Boost";
	appleGreen.Mesh = appleGreenAsset.Object;
	appleGreen.InvImage = appleGreenImage.Object;
	appleGreen.scale = FVector(1, 1, 1);
	appleGreen.offset = FVector(0, 0, 0);
	appleGreen.Location = FVector(0, 0, 0);
	appleGreen.respawnTime = 5;
	appleGreen.Damage = 0;
	appleGreen.bDamage = true;
	appleGreen.Damage = 20;//%
	appleGreen.DamageTime = 20;

	ItemTemplates.Add(appleBlue);
	ItemTemplates.Add(appleRed);
	ItemTemplates.Add(appleGreen);
	
	maxEnemies = 10;

	//Linking the blueprint
	static ConstructorHelpers::FObjectFinder<UBlueprint> Enemy_BP(TEXT("/Game/BP_EnemyCharacter"));

	if (Enemy_BP.Object)
	{
		BP_EnemyCharacter = (UClass*)Enemy_BP.Object->GeneratedClass;
	}
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetMallow();
	SpawnItems();
	
	float i = 1;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_EnemyCharacter, bp_enemies);
	SpawnAndAddAI();
}

void ATestGameMode::SpawnAndAddAI()
{
	//getting spawn locations for AI
	TSubclassOf<AAISpawnLoc> AISpawnLoc = AAISpawnLoc::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AISpawnLoc, AISpawnLocations);               
	
	for (int i = 0; i < maxEnemies; i++)
	{
		float rand = FMath::RandRange(0, AISpawnLocations.Num() - 1);

		AAISpawnLoc *loc = (AAISpawnLoc*)AISpawnLocations[rand];
		FVector location = loc->GetActorLocation();
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if(loc->bOccupied == false)
		{
			//AAI *spawning = GetWorld()->SpawnActor<AAI>(AAI::StaticClass());
			AEnemyCharacter *spawning = GetWorld()->SpawnActor<AEnemyCharacter>(BP_EnemyCharacter);
			//AEnemyAI *spawning = GetWorld()->SpawnActor<AEnemyAI>(AEnemyAI::StaticClass());
			//loc->bOccupied = true;
			spawning->SetActorLocation(AISpawnLocations[rand]->GetActorLocation());
			//AEnemyCharacter::SpawnAI(AISpawnLocations[rand]->GetActorLocation());
			enemies.Add(spawning);
			mainChar->AllAI.Add(spawning);
		}
	}
	
	for (int i = 0; i < bp_enemies.Num(); i++)
	{
		mainChar->AllAI.Add((AEnemyCharacter*)bp_enemies[i]);
	}
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

void ATestGameMode::GetMallow()
{
	//getting mallow main character
	TSubclassOf<Amallow> mallow = Amallow::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), mallow, mallows);

	mainChar = (Amallow*)mallows[0];
	mainChar->AutoPossessPlayer = EAutoReceiveInput::Player0;
	mainChar->AutoReceiveInput = EAutoReceiveInput::Player0;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("POSSESSED"));

}

void ATestGameMode::SpawnItems()
{
	//getting spawn locations for items
	TSubclassOf<AItemSpawnLoc> ItemSpawnLoc = AItemSpawnLoc::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ItemSpawnLoc, ItemSpawnLocations);

	for (int i = 0; i < ItemSpawnLocations.Num(); i++)
	{
		//random loot spawning
		float rand = FMath::RandRange(0, ItemTemplates.Num() - 1);

		ItemTemplates[rand].Location = ItemSpawnLocations[i]->GetActorLocation();

		AItem *spawning = GetWorld()->SpawnActor<AItem>(AItem::StaticClass());
		spawning->Initializer(&ItemTemplates[rand]);

		Items.Add(spawning);
	}
}