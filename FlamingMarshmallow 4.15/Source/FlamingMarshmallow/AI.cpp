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
		aiMesh->SetRelativeLocation(FVector(.5f, .5f, .5f));
	}

	Flames = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flames"));
	Flames->SetupAttachment(RootComponent);
	Flames->bAutoActivate = false;
	Flames->SetRelativeLocation(FVector(0, 0, 0));
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlamesAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	
	if (FlamesAsset.Succeeded())
	{
		Flames->SetTemplate(FlamesAsset.Object);
	}

	left = 1;
	health = 100;
	damage = 5;
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
	AIname = GetName();
	moveAI();
	CheckRangeToChar();
	FlamesCheck();

	if (health <= 0)
	{
		Destroy();
	}
	if (!mallow->IsPendingKill())
	{
		if (bCanAttack)
		{
			if (bfirstTime)
			{
				lastTimeInRange = GetWorld()->GetTimeSeconds();
				bfirstTime = false;
			}
			if (GetWorld()->GetTimeSeconds() - lastTimeInRange >= delayForAttack)
			{
				bfirstTime = true;
				Attack();
				bCanAttack = false;
				UE_LOG(LogTemp, Warning, TEXT("Ouch! %s"), *AIname);
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
}

void AAI::followMallow()
{
}

void AAI::Attack()
{
	mallow->health -= damage;
	if (bKnock)
	{
		KnockBack();
	}
	UE_LOG(LogTemp, Warning, TEXT("Ouch!"));
	bfirstTime = true;
}

void AAI::CheckRangeToChar()
{
	distToPlayer = GetDistanceTo(mallow);
	if (distToPlayer <= 750)
	{
		rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mallow->GetActorLocation());
		rotation.Pitch = 0.f;
		SetActorRotation(rotation);
		bInTargetRange = true;
		if (distToPlayer <= 100)
		{
			bInAttackRange = true;
			if (distToPlayer <= attackRange)
			{
				bCanAttack = true;
				bKnock = true;
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

void AAI::KnockBack()
{
	PlayerToThis = GetActorLocation() - mallow->GetActorLocation();
	float LaunchForce = PlayerToThis.Normalize() * 1.001f;
	UE_LOG(LogTemp, Warning, TEXT("FORCE: %f"), LaunchForce);
	mallow->SetActorLocation((mallow->GetActorLocation()*LaunchForce));
	bKnock = false;
}

/*void AAI::KnockBackSelf(FVector amount)
{
	FHitResult result;

	FVector start = GetActorLocation();
	//start += amount;
	//FVector amount = FVector(start.X, start.Y, start.Z - 1);
	amount.Z -= 5000;
	GetWorld()->LineTraceSingleByChannel(result, start, amount, ECollisionChannel::ECC_Visibility);

	FloorLoc = result.Location;
	SetActorLocation(FloorLoc);
}*/

void AAI::FlamesCheck()
{
	if (bFlameOn == true && Flames->IsActive() == false)
	{
		Flames->ToggleActive();
	}
}