// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "mallow.h"
#include "ShrineSP.h"
#include "AI.h"
#include "EngineUtils.h"
#include "TestGameMode.h"

ATestGameMode::ATestGameMode()
{
		HUDClass = AMHUD::StaticClass();

		//loading in assets for item spawning
		static ConstructorHelpers::FObjectFinder<UTexture> ConeImage(TEXT("/Game/WorldV3/Textures/Cone"));
		static ConstructorHelpers::FObjectFinder<UTexture> CubeImage(TEXT("/Game/WorldV3/Textures/Cube"));
		static ConstructorHelpers::FObjectFinder<UTexture> SphereImage(TEXT("/Game/WorldV3/Textures/sphere"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));


		//setting up the structs used to make each item from, so all you need to make an item is fill out one of these structs
		//spawn the item class and pass the struct in to its initializer class
		Cone.Name = "Cone";
		Cone.Mesh = ConeAsset.Object;
		Cone.InvImage = ConeImage.Object;
		Cone.scale = FVector(0.2, 0.2, 0.2);
		Cone.offset = FVector(0, 0, 0);
		Cone.Location = FVector(0, 0, 0);
		Cone.respawnTime = 10;
		Cone.Health = 20;//units
		Cone.bFood = true;

		Cube.Name = "Cube";
		Cube.Mesh = CubeAsset.Object;
		Cube.InvImage = CubeImage.Object;
		Cube.scale = FVector(0.2, 0.2, 0.2);
		Cube.offset = FVector(0, 0, 0);
		Cube.Location = FVector(0, 0, 0);
		Cube.respawnTime = 20;
		Cube.Speed = 0;
		Cube.bSpeed = true;
		Cube.Speed = 20;//%
		Cube.SpeedTime = 20;

		Sphere.Name = "Sphere";
		Sphere.Mesh = SphereAsset.Object;
		Sphere.InvImage = SphereImage.Object;
		Sphere.scale = FVector(0.2, 0.2, 0.2);
		Sphere.offset = FVector(0, 0, 0);
		Sphere.Location = FVector(0, 0, 0);
		Sphere.respawnTime = 5;
		Sphere.Damage = 0;
		Sphere.bDamage = true;
		Sphere.Damage = 20;//%
		Sphere.DamageTime = 20;

		ItemTemplates.Add(Cone);
		ItemTemplates.Add(Cube);
		ItemTemplates.Add(Sphere);

		maxEnemies = 10;
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetMallow();
	SpawnItems();
	
	float i = 1;

	SpawnAndAddAI();
}

void ATestGameMode::SpawnAndAddAI()
{

	/*AAI* testAI = GetWorld()->SpawnActor<AAI>(AAI::StaticClass());
	testAI->SetActorLocation(FVector(i*60.f, i*10.f, 50.f));
	if (enemies.Num() % 2 == 0)
	{
		testAI->left *= -1;
	}
	enemies.Add(testAI);
	mainChar->TestAI.Add(testAI);
	UE_LOG(LogTemp, Warning, TEXT("%d"), enemies.Num());
	*/
	//getting spawn locations for AI
	TSubclassOf<AAISpawnLoc> AISpawnLoc = AAISpawnLoc::StaticClass();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AISpawnLoc, AISpawnLocations);

	for (int i = 0; i < maxEnemies; i++)
	{
		float rand = FMath::RandRange(0, AISpawnLocations.Num() - 1);

		AAISpawnLoc *loc = (AAISpawnLoc*)AISpawnLocations[rand];
		//FVector location = loc->GetActorLocation();

		if(loc->bOccupied == false)
		{
			AAI *spawning = GetWorld()->SpawnActor<AAI>(AAI::StaticClass());
			loc->bOccupied = true;
			spawning->SetActorLocation(AISpawnLocations[rand]->GetActorLocation());
			enemies.Add(spawning);
			mainChar->TestAI.Add(spawning);
		}
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