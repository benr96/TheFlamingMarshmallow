// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "mallow.h"
#include "ShrineSP.h"
#include "AI.h"
#include "EngineUtils.h"
#include "MHUD.h"
#include "TestGameMode.h"

ATestGameMode::ATestGameMode()
{
		HUDClass = AMHUD::StaticClass();

		static ConstructorHelpers::FObjectFinder<UTexture> ConeImage(TEXT("/Game/WorldV3/Textures/Cone"));
		static ConstructorHelpers::FObjectFinder<UTexture> CubeImage(TEXT("/Game/WorldV3/Textures/Cube"));
		static ConstructorHelpers::FObjectFinder<UTexture> SphereImage(TEXT("/Game/WorldV3/Textures/sphere"));

		static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));


		Cone.Name = "Cone";
		Cone.Mesh = ConeAsset.Object;
		Cone.InvImage = ConeImage.Object;
		Cone.scale = FVector(0.2, 0.2, 0.2);
		Cone.offset = FVector(0, 0, 0);
		Cone.Location = FVector(0, 0, 0);

		Cube.Name = "Cube";
		Cube.Mesh = CubeAsset.Object;
		Cube.InvImage = CubeImage.Object;
		Cube.scale = FVector(0.2, 0.2, 0.2);
		Cube.offset = FVector(0, 0, 0);
		Cube.Location = FVector(0, 0, 0);

		Sphere.Name = "Sphere";
		Sphere.Mesh = SphereAsset.Object;
		Sphere.InvImage = SphereImage.Object;
		Sphere.scale = FVector(0.2, 0.2, 0.2);
		Sphere.offset = FVector(0, 0, 0);
		Sphere.Location = FVector(0, 0, 0);
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



	TSubclassOf<AItemSpawnLoc> ClassToFind = AItemSpawnLoc::StaticClass();


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, ItemSpawnLocations);

	for (int i = 0; i < ItemSpawnLocations.Num(); i++)
	{
		AItemSpawnLoc *loc = (AItemSpawnLoc*)ItemSpawnLocations[i];
		FVector location = loc->location;

		Cube.Location = ItemSpawnLocations[i]->GetActorLocation();

		AItem *spawning = GetWorld()->SpawnActor<AItem>(AItem::StaticClass());
		spawning->Initializer(&Cube);
	}
	

	

}

APawn* ATestGameMode::SpawnDefaultPawnFor()
{
	return NULL;
}
