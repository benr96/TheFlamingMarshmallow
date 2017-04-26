// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "EnemyCharacter.h"
#include "EnemyAI.h"
#include "mallow.h"
#include "TestGameMode.h"
#include "Kismet/KismetMathLibrary.h"

/* AI Include */
#include "Perception/PawnSensingComponent.h"

Amallow* mallow;

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
		aiMesh->SetRelativeLocation(FVector(.5f, .5f, -50.f));
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

	//Initializing the pawn sensing component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	//Set the peripheral vision angle to 90 degrees
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

	left = 1;
	health = 100;
	damage = 5;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	mallow = (Amallow*)(GetWorld()->GetFirstPlayerController()->GetPawn());

	//Register the function that is going to fire when the character sees a Pawn
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePawn);
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
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
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AEnemyCharacter::OnSeePawn(APawn* Pawn)
{
	AEnemyAI* AIController = Cast<AEnemyAI>(GetController());
	//Set the seen target on the blackboard
	if (AIController)
	{
		AIController->SetSeenTarget(Pawn);
	}
}

void AEnemyCharacter::SpawnAI(FVector Location)
{
	FRotator Rotation = FRotator(0.f, 0.f, 0.f);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	//AEnemyCharacter *Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(BP_EnemyCharacter, Location, Rotation, SpawnParams);
}

void AEnemyCharacter::moveAI()
{
}

void AEnemyCharacter::followMallow()
{
}

void AEnemyCharacter::Attack()
{
	mallow->health -= damage;
	if (bKnock)
	{
		KnockBack();
	}
	UE_LOG(LogTemp, Warning, TEXT("Ouch!"));
	bfirstTime = true;
}

void AEnemyCharacter::CheckRangeToChar()
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

void AEnemyCharacter::KnockBack()
{
	FVector newPosition;
	PlayerToThis = GetActorLocation() - mallow->GetActorLocation();
	float LaunchForce = PlayerToThis.Normalize() * 1.001f;
	UE_LOG(LogTemp, Warning, TEXT("FORCE: %f"), LaunchForce);
	newPosition = mallow->GetActorLocation()*LaunchForce;

	if (mallow->playerYaw >= -45.f && mallow->playerYaw <= 45.f)
	{
		newPosition.X -= 15.f;
	}
	else if (mallow->playerYaw <= 135.f && mallow->playerYaw > 45.f)
	{
		newPosition.Y -= 15.f;
	}
	else if (mallow->playerYaw >= -135.f && mallow->playerYaw < -45.f)
	{
		newPosition.Y += 15.f;
	}
	else
	{
		newPosition.X += 15.f;
	}
	UE_LOG(LogTemp, Warning, TEXT("VECTOR: %f %f %f"), mallow->GetActorLocation().X, mallow->GetActorLocation().Y, mallow->GetActorLocation().Z);
	mallow->SetActorLocation(newPosition);
	bKnock = false;
}

/*void AEnemyCharacter::KnockBackSelf(FVector amount)
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

void AEnemyCharacter::FlamesCheck()
{
	if (bFlameOn == true && Flames->IsActive() == false)
	{
		Flames->ToggleActive();
	}
}
