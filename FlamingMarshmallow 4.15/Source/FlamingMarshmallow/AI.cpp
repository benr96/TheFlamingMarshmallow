// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "AI.h"
#include "mallow.h"
#include "TestGameMode.h"
#include "Kismet/KismetMathLibrary.h"

Amallow* mallow;

// Sets default values
AAI::AAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating static mesh
	aiMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	aiMesh->SetupAttachment(RootComponent);

	//find the asset we want to use
	static ConstructorHelpers::FObjectFinder<UStaticMesh> aiMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	
	//if it found the asset position it correctly
	if (aiMeshAsset.Succeeded())
	{
		aiMesh->SetStaticMesh(aiMeshAsset.Object);
		aiMesh->SetWorldScale3D(FVector(0.1f, 0.7f, 1.5f));
		//aiMesh->SetRelativeLocation(FVector(FMath::RandRange(-1100.f, 1100.f), FMath::RandRange(-1000.f, 1000.f), 50.f));
		aiMesh->SetRelativeLocation(FVector(98.0, 470.f, 160.f));
	}

	left = 1;
	health = 100;
}

// Called when the game starts or when spawned
void AAI::BeginPlay()
{
	Super::BeginPlay();
	mallow = (Amallow*)(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void AAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FString AIname = GetName();
	moveAI();
	CheckRangeToChar();

	if (health <= 0)
	{
		Destroy();
	}
	if (!mallow->IsPendingKill())
	{
		if (bInAttackRange)
		{
			if (firstTime)
			{
				lastTimeInRange = GetWorld()->GetTimeSeconds();
				firstTime = false;
			}

			if (GetWorld()->GetTimeSeconds() - lastTimeInRange >= delayForAttack)
			{
				mallow->health -= 20;
				UE_LOG(LogTemp, Warning, TEXT("Ouch! %s"), *AIname);
				firstTime = true;
			}
		}
	}
}

// Called to bind functionality to input
void AAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAI::moveAI()
{
	SetActorLocation(FVector(GetActorLocation().X, left*yPos, 50.f));

	if (yPos <= 0 || yPos >= 300)
	{
		bHitLimit = true;
	}
	else
	{
		bHitLimit = false;
	}

	if (bHitLimit)
	{
		inc *= -1;
	}

	//yPos += inc;
}

void AAI::followMallow()
{
}

void AAI::Attack()
{
	mallow->health -= 10;
	UE_LOG(LogTemp, Warning, TEXT("Ouch!"));
}

void AAI::CheckRangeToChar()
{
	distToPlayer = GetDistanceTo(mallow);
	if (distToPlayer <= 750)
	{
		bInTargetRange = true;
		if (distToPlayer <= 100)
		{
			bInAttackRange = true;
			if (distToPlayer <= attackRange)
			{
				bCanAttack = true;
			}
			else
			{
				bCanAttack = false;
			}
		}
		else
		{
			bInAttackRange = false;
		}
	}
	else
	{
		bInTargetRange = false;
	}
	rotationFromChar = UKismetMathLibrary::FindLookAtRotation(mallow->GetActorLocation(), this->GetActorLocation());
}