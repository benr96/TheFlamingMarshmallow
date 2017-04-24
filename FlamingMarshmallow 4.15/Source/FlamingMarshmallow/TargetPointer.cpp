// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "TargetPointer.h"
#include "mallow.h"

Amallow* player;
AAI* TargetedAI;

// Sets default values
ATargetPointer::ATargetPointer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating static mesh
	pointer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	pointer->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> pointerAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone"));
	//if asset loading succeeded put it on static mesh
	if (pointerAsset.Succeeded())
	{
		pointer->SetStaticMesh(pointerAsset.Object);
	}

	pointer->SetWorldScale3D(FVector(.2f, .2f, .2f));
	FRotator newRotation;
	newRotation.Roll += 180.f;
	pointer->SetWorldRotation(newRotation);
}

// Called when the game starts or when spawned
void ATargetPointer::BeginPlay()
{
	Super::BeginPlay();

	player = (Amallow*)(GetWorld()->GetFirstPlayerController()->GetPawn());

	//GetWorld()->SpawnActor<ATargetPointer>(ATargetPointer::StaticClass());
	location = player->GetActorLocation();
	SetActorLocation(location);
	
}

// Called every frame
void ATargetPointer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (player->bLockOn)
	{
		location = player->TestAI[player->next]->GetActorLocation();
		location.Z += 180.f;
		SetActorLocation(location);
	}
	else
	{
		location = player->GetActorLocation();
		location.Z -= 1000.f;
		SetActorLocation(location);
	}
}

