// Fill out your copyright notice in the Description page of Project Settings.
//TODO allow some Yaw movement (make smoother)
//TODO sort array everytime enemy is added/deleted
//TODO Fix bug when looking form behind while targeting

#include "FlamingMarshmallow.h"
#include "MHUD.h"
#include "mallow.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values

Amallow::Amallow()
{
	next = 0;
	highest = 0;

	// Variables for targeting system
	fMouseSensitivity = 60.0f;
	mousePitch = 0.0f;
	mouseYaw = 0.0f;

	//combat
	health = 100;
	damage = 5;
	BaseDamage = damage;

	//movement
	right = false;
	left = false;
	forward = false;
	back = false;
	doubleMove = false;
	midJump = false;

	originalTime = 0.05;
	TurnRate = 60.0f;
	LookRate = 60.0f;
	MaxSpeed = 400.0f;
	BaseMaxSpeed = MaxSpeed;
	JumpVelocity = 400.0f;
	dashState = 0.5;

	//Hud/menus
	bMenuShow = false;
	bInvShow = false;
	bAcceptInput = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//size of capsule
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 35.0f);
	RootComponent = GetCapsuleComponent();
	
	//creating static mesh
	MallowVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallowVisual"));
	MallowVisual->SetupAttachment(RootComponent);

	//find the asset we want to use
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallowVisualAsset(TEXT("/Game/MallowParts/marshmallowV5_Marshmallow_Body"));

	//if it found the asset position it correctly
	if (MallowVisualAsset.Succeeded())
	{
		MallowVisual->SetStaticMesh(MallowVisualAsset.Object);
		MallowVisual->SetRelativeLocation(FVector(5.0f, 0.0f, -12.0f));
	}

	// Creates an audio component
	jumpAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("JumpAudioComp"));
	attackAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackAudioComp"));

	// Stopping the sound from playing immediately
	jumpAudioComponent->bAutoActivate = false;
	attackAudioComponent->bAutoActivate = false;

	// Attaching the sound to the pawn
	jumpAudioComponent->SetupAttachment(RootComponent);
	attackAudioComponent->SetupAttachment(RootComponent);

	// Sound comes from slightly in front of the pawn
	jumpAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	attackAudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	/*
	MallowVisual = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MallowVisual"));
	MallowVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MallowVisualAsset(TEXT("/Game/MallowParts/marshmallowV5_Body_Skeleton.uasset"));
	if (MallowVisualAsset.Succeeded())
	{
		MallowVisual->SetSkeletalMesh(MallowVisualAsset.Object);
		MallowVisual->SetRelativeLocation(FVector(5.0f, 0.0f, -12.0f));
	}
	*/
	LeftEyeVis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftEyeVis"));
	RightEyeVis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightEyeVis"));

	LeftEyeVis->SetupAttachment(MallowVisual);
	RightEyeVis->SetupAttachment(MallowVisual);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LeftEyeAsset(TEXT("/Game/MallowParts/marshmallowV5_left_eye"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RightEyeAsset(TEXT("/Game/MallowParts/marshmallowV5_right_eye"));

	if (LeftEyeAsset.Succeeded())
	{
		LeftEyeVis->SetStaticMesh(LeftEyeAsset.Object);
	}

	if (RightEyeAsset.Succeeded())
	{
		RightEyeVis->SetStaticMesh(RightEyeAsset.Object);
	}

	//so it doesn't rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;//move character in direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);//setting rotation rate
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;//setting jump velocity
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;//setting max walking speed
	GetCharacterMovement()->AirControl = 10.0f;//setting force in air

	//CAMERA
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);//Attach camera to the root component
	CameraBoom->TargetArmLength = 300.0f;//Set camera boom length
	CameraBoom->bUsePawnControlRotation = true;//make camera rotate based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));//init actual camera
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);//attach follow camera to socket on boom
	FollowCamera->bUsePawnControlRotation = false;//stop camera rotating with controller, so it just rotates with boom

	//Particle System
	Flames = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flames"));
	Flames->SetupAttachment(MallowVisual);
	Flames->bAutoActivate = false;
	Flames->SetRelativeLocation(FVector(0, 0, 0));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlamesAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));

	if (FlamesAsset.Succeeded())
	{
		Flames->SetTemplate(FlamesAsset.Object);
	}

	FlameCharge = 0.99;
}

// Called when the game starts or when spawned
void Amallow::BeginPlay()
{
	Super::BeginPlay();
	HUD = (AMHUD*)(GetWorld()->GetFirstPlayerController()->GetHUD());
	PC = (AUI_Controller*)(GetWorld()->GetFirstPlayerController());

	PC->mainChar = this;

	GetWorldTimerManager().SetTimer(FTFlameCharger, this, &Amallow::FlameCharger, 0.01, true);
}

// Called every frame
void Amallow::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	movementControl();

	playerYaw = GetActorRotation().Yaw;
	GEngine->AddOnScreenDebugMessage(-10, 1.f, FColor::Yellow, FString::Printf(TEXT("%f %f %f"), GetActorRotation().Pitch, GetActorRotation().Roll, GetActorRotation().Yaw));
	if (bLockOn)
	{
		TargetEnemy();
	}
	if (health <= 0)
	{
		Destroy();
		bDeathMenu = true;
	}

	FHitResult result;

	FVector start = GetCharacterMovement()->GetActorFeetLocation();
	FVector end = FVector(start.X, start.Y, start.Z - 5000);

	GetWorld()->LineTraceSingleByChannel(result, start, end, ECollisionChannel::ECC_Visibility);

	FloorLoc = result.Location;
	
	if (PC->bMainMenu == true)
	{
		PC->MainMenu->SetVisibility(ESlateVisibility::Visible);
		PC->Menu->SetVisibility(ESlateVisibility::Hidden);
		PC->HUD->SetVisibility(ESlateVisibility::Hidden);
		PC->DeathMenu->SetVisibility(ESlateVisibility::Hidden);
		PC->bShowMouseCursor = true;
		PC->SetPause(true);
		bAcceptInput = false;
	}

	if (bDeathMenu == true)
	{
		PC->DeathMenu->SetVisibility(ESlateVisibility::Visible);
		PC->MainMenu->SetVisibility(ESlateVisibility::Hidden);
		PC->Menu->SetVisibility(ESlateVisibility::Hidden);
		PC->HUD->SetVisibility(ESlateVisibility::Hidden);
		PC->bShowMouseCursor = true;
		PC->SetPause(true);
		bAcceptInput = false;
	}

	if (FlameCharge <= 0)
	{
		Flames->SetActive(false);
		bUsingFlame = false;
	}

	if (Flames->IsActive() == true)
	{
		damage = BaseDamage * 3;
	}
	else if(GetWorldTimerManager().IsTimerActive(HUD->FTDamage) == true)
	{
		damage = BaseDamage + dif;
	}
	else
	{
		damage = BaseDamage;
	}
}

// Called to bind functionality to input
void Amallow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	//jumping
	PlayerInputComponent->BindAction("Jump",IE_Pressed, this,&Amallow::jump );

	//start running/stop running
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &Amallow::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &Amallow::StopRun);


	//moving forward and right
	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &Amallow::MoveForward);
	PlayerInputComponent->BindAction("MoveBack", IE_Pressed, this, &Amallow::MoveBack);


	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &Amallow::MoveRight);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &Amallow::MoveLeft);

	PlayerInputComponent->BindAction("MoveForward", IE_Released, this, &Amallow::StopForward);
	PlayerInputComponent->BindAction("MoveBack", IE_Released, this, &Amallow::StopBack);


	PlayerInputComponent->BindAction("MoveRight", IE_Released, this, &Amallow::StopRight);
	PlayerInputComponent->BindAction("MoveLeft", IE_Released, this, &Amallow::StopLeft);

	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &Amallow::ToggleMenu);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &Amallow::ToggleInv).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &Amallow::Pickup);
	PlayerInputComponent->BindAction("Pickup", IE_Released, this, &Amallow::StopPickup);
	

	
	//turning and camera control
	PlayerInputComponent->BindAxis("Turn", this, &Amallow::CameraYaw);
	PlayerInputComponent->BindAxis("TurnRate", this, &Amallow::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &Amallow::CameraPitch);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Amallow::LookUpAtRate);


	//control for targetting enemies
	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &Amallow::LockOnEnemy);
	PlayerInputComponent->BindAction("LockRight", IE_Pressed, this, &Amallow::LockRightEnemy);
	PlayerInputComponent->BindAction("LockLeft", IE_Pressed, this, &Amallow::LockLeftEnemy);


	//control for attacking
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &Amallow::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &Amallow::LMouseClicked).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &Amallow::LMouseReleased).bExecuteWhenPaused = true;


	PlayerInputComponent->BindAction("ToggleFire", IE_Pressed, this, &Amallow::ToggleFire);
}

void Amallow::ToggleFire()
{
	if (bAcceptInput == true)
	{
		if(FlameCharge >= 1)
		{
			Flames->ToggleActive();
			bUsingFlame = true;
		}
	}
}

void Amallow::movementControl()
{
	//if in the air after dashing divide velocity by 10 so you don't get an insane boost
	if (GetCharacterMovement()->Velocity.Z != 0 && doubleMove == true)
	{
		GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity/10;
	}

	//input functions change boolean variable and handle dashing/dodging, this functions controls normal movement
	if (left == true)
	{
		FVector Direction = CheckDirection("Y");

		AddMovementInput(Direction, -1);
	}

	if (right == true)
	{
		FVector Direction = CheckDirection("Y");

		AddMovementInput(Direction, 1);
	}

	if (forward == true)
	{
		FVector Direction = CheckDirection("X");

		AddMovementInput(Direction, 1);
	}

	if (back == true)
	{
		FVector Direction = CheckDirection("X");

		AddMovementInput(Direction, -1);
	}
}

//the move functions check if timer is active, if so, increase max speed/acc for dashing/dodging
void Amallow::MoveForward()
{
	if (bAcceptInput == true)
	{
		if (GetWorldTimerManager().IsTimerActive(ForwardTimer) == true && GetCharacterMovement()->Velocity.Z == 0)
		{
			dashOn(20000000000, 5000);
		}

		forward = true;
	}
}

//the timers are started when the button is released, after release you have 0.05 seconds to press it again to initiate dash
void Amallow::StopForward()
{
	if (bAcceptInput == true)
	{
		if (GetCharacterMovement()->Velocity.Z == 0)
		{
			GetWorldTimerManager().SetTimer(ForwardTimer, this, &Amallow::IncFTime, 0.05f, true);
		}
		forward = false;
	}

}

void Amallow::IncFTime()
{
	forwardTime -= 0.05;

	if (forwardTime < 0)
	{
		dashOff(ForwardTimer, &forwardTime);
	}
}

void Amallow::MoveBack()
{
	if (bAcceptInput == true)
	{
		if (GetWorldTimerManager().IsTimerActive(BackTimer) == true && GetCharacterMovement()->Velocity.Z == 0)
		{
			dashOn();
		}

		back = true;
	}
}

void Amallow::StopBack()
{
	if (bAcceptInput == true)
	{
		if (GetCharacterMovement()->Velocity.Z == 0)
		{
			GetWorldTimerManager().SetTimer(BackTimer, this, &Amallow::IncBTime, 0.05f, true);
		}
		back = false;
	}

}

void Amallow::IncBTime()
{
	backTime -= 0.05;

	if (backTime < 0)
	{
		dashOff(BackTimer, &backTime);
	}
}

void Amallow::MoveLeft()
{
	if (bAcceptInput == true)
	{
		if (GetWorldTimerManager().IsTimerActive(LeftTimer) == true && GetCharacterMovement()->Velocity.Z == 0)
		{
			dashOn();
		}

		left = true;
	}
}

void Amallow::StopLeft()
{
	if (bAcceptInput == true)
	{
		if (GetCharacterMovement()->Velocity.Z == 0)
		{
			GetWorldTimerManager().SetTimer(LeftTimer, this, &Amallow::IncLTime, 0.05f, true);
		}
		left = false;
	}
}

void Amallow::IncLTime()
{
	leftTime -= 0.05;

	if (leftTime < 0)
	{
		dashOff(LeftTimer, &leftTime);
	}
}

void Amallow::MoveRight()
{
	if (bAcceptInput == true)
	{
		if (GetWorldTimerManager().IsTimerActive(RightTimer) == true && GetCharacterMovement()->Velocity.Z == 0)
		{
			dashOn();
		}

		right = true;
	}
}

void Amallow::StopRight()
{
	if (bAcceptInput == true)
	{
		if (GetCharacterMovement()->Velocity.Z == 0)
		{
			GetWorldTimerManager().SetTimer(RightTimer, this, &Amallow::IncRTime, 0.05f, true);
		}
		right = false;
	}
}

void Amallow::IncRTime()
{
	rightTime -= 0.05;

	if (rightTime < 0)
	{
		dashOff(RightTimer, &rightTime);
	}
}

void Amallow::jump()
{
	if (bAcceptInput == true)
	{
		float Z = GetCharacterMovement()->Velocity.Z;
		float changeZ = 500;//make this relative to current Z so it always has a similar increase(currently double jumping at different levels of normal jump changes overall jump height quite a lot)

		if (midJump == true)
		{
			GetCharacterMovement()->Velocity += FVector(0, 0, changeZ);//maybe adding straight to velocity isn't the best way to do this
			midJump = false;
			jumpAudioComponent->Activate(true);
			jumpAudioComponent->Play();

		}

		if (Z == 0)
		{
			Jump();
			midJump = true;
			jumpAudioComponent->Activate(true);
			jumpAudioComponent->Play();

		}
	}
}

void Amallow::StartRun()
{
	if (bAcceptInput == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeed * 1.5;
	}
}

void Amallow::StopRun()
{
	if (bAcceptInput == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	}
}

void Amallow::LookUpAtRate(float Rate)
{
	if (bAcceptInput == true)
	{
		AddControllerPitchInput(Rate * LookRate * GetWorld()->GetDeltaSeconds());
	}
}

void Amallow::TurnAtRate(float Rate)
{
	if (bAcceptInput == true)
	{
		AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void Amallow::LockOnEnemy()
{
	if (bAcceptInput == true)
	{
		if (bLockOn) 
		{
			bLockOn = false;
		}
		else 
		{
			next = closest;
			bFirstLock = true;
			bLockOn = true;
		}

		// Test messages
		if (bLockOn) { UE_LOG(LogTemp, Warning, TEXT("Locked On")); }
		else { UE_LOG(LogTemp, Warning, TEXT("Not Locked On")); }
	}
}

void Amallow::LockRightEnemy()
{
	if (bLockOn)
	{
		FindRight();
		UE_LOG(LogTemp, Warning, TEXT("Switched to right target."));
	}
}

void Amallow::LockLeftEnemy()
{
	if (bLockOn)
	{
		FindLeft();
		UE_LOG(LogTemp, Warning, TEXT("Switched to left target."));
	}
}

void Amallow::TargetEnemy()
{
	if (bFirstLock)
	{
		FindClosest();
		next = closest;
		bFirstLock = false;
	}
	if (AllAI.Num() > 0)
	{
		if (AllAI[next]->bInTargetRange)
		{
			FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AllAI[next]->GetActorLocation());
			rotation.Pitch = mousePitch;
			rotation.Yaw += 10 * mouseYaw;
			Controller->SetControlRotation(rotation); 
		}
		else
		{
			//bLockOn = false;
			//bIsTargetting = false;
			next = previous;
			GEngine->AddOnScreenDebugMessage(-50, 1.f, FColor::Yellow, FString::Printf(TEXT("No targets in range.")));
		}
	}
}

void Amallow::FindClosest()
{
	closest = 0;
	for (int i = 0; i < AllAI.Num(); i++)
	{
		if (AllAI[closest]->distToPlayer > AllAI[i]->distToPlayer)
		{
			closest = i;
		}
	}
}

void Amallow::FindRight()
{
	SortEnemies();
	if (next < AllAI.Num()-1)
	{
		previous = next;
		next++;
	}
}

void Amallow::FindLeft()
{
	SortEnemies();
	if (next > 0)
	{
		previous = next;
		next--;
	}
}

void Amallow::SortEnemies()
{
	AAI* temp;
	for (int i = 0; i < AllAI.Num(); i++)
	{
		int swaps = 0;
		for (int j = 0; j < AllAI.Num()-i-1; j++)
		{
			if (AllAI[j]->rotationFromChar.Yaw > AllAI[j + 1]->rotationFromChar.Yaw)
			{
				temp = AllAI[j];
				AllAI[j] = AllAI[j + 1];
				AllAI[j + 1] = temp;
				swaps++;
			}
		}
		if (swaps == 0)
		{
			break;
		}
	}
}

void Amallow::CameraPitch(float fAmount)
{
	// Will disable camera movement by player when 'TAB' has been pressed
	AddControllerPitchInput(fMouseSensitivity * fAmount * GetWorld()->GetDeltaSeconds());
	if (bLockOn)
	{
		mousePitch -= fMouseSensitivity * fAmount * GetWorld()->GetDeltaSeconds();
	}
}

void Amallow::CameraYaw(float fAmount)
{
	if (bAcceptInput == true)
	{
		// Will disable camera movement by player when 'TAB' has been pressed
		if (!bLockOn)
		{
			AddControllerYawInput(fMouseSensitivity * fAmount * GetWorld()->GetDeltaSeconds());
		}
		else
		{
			//AddControllerYawInput(0);
			AddControllerYawInput(fMouseSensitivity * fAmount * GetWorld()->GetDeltaSeconds());
			mouseYaw = fMouseSensitivity * fAmount * GetWorld()->GetDeltaSeconds();
		}
	}
}

void Amallow::Attack()
{
	if (bLockOn == true && AllAI[next]->bInAttackRange == true && attackTime == 0 && !GetWorldTimerManager().IsTimerActive(attackHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("DIE!"));
		AllAI[next]->health -= damage;
		attackAudioComponent->Activate(true);
		attackAudioComponent->Play();
		KnockBack();
		if (bUsingFlame == true)
		{
			AllAI[next]->bFlameOn = true;
		}

		if (AllAI[next]->health <= 0)
		{
			AllAI.RemoveAt(next);
			SortEnemies();
			FindClosest();
			next = closest;
			UE_LOG(LogTemp, Warning, TEXT("%d"), AllAI.Num());
			bLockOn = false;
		}
		GetWorldTimerManager().SetTimer(attackHandle, this, &Amallow::DelayAttack, .4f, true);
	}
}

void Amallow::DelayAttack()
{
	if (attackTime < 1)
	{
		attackTime++;
	}
	else
	{
		attackTime = 0;
		GetWorldTimerManager().ClearTimer(attackHandle);
	}
}

void Amallow::KnockBack()
{
	FVector newPosition;
	FRotator rotation;

	rotation = GetActorRotation();
	PlayerToEnemy = GetActorLocation() - AllAI[next]->GetActorLocation();
	float LaunchForce = PlayerToEnemy.Normalize() * 1.002f;
	UE_LOG(LogTemp, Warning, TEXT("FORCE: %f"), LaunchForce);
	newPosition = AllAI[next]->GetActorLocation()*LaunchForce;

	if (rotation.Yaw >= -45.f && rotation.Yaw <= 45.f)
	{
		newPosition.Y -= 15.f;
	}
	else if (rotation.Yaw <= 135.f && rotation.Yaw > 45.f)
	{
		newPosition.X -= 15.f;
	}
	else if (rotation.Yaw >= -135.f && rotation.Yaw < -45.f)
	{
		newPosition.X += 15.f;
	}
	else
	{
		newPosition.Y += 15.f;
	}

	AllAI[next]->SetActorLocation(newPosition);
}

FVector Amallow::CheckDirection(FString Axis)
{
	FVector Direction;

	FRotator Yaw(0, Controller->GetControlRotation().Yaw, 0);

	if (Axis == "X")
	{
		Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	}
	else if (Axis == "Y")
	{
		Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	}

	return Direction;
}

void Amallow::dashOff(FTimerHandle handle, float *time)
{
	GetWorldTimerManager().ClearTimer(handle);
	*time = originalTime;
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	GetCharacterMovement()->MaxAcceleration = 2048;
	GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity / 10;
	doubleMove = false;
	
}

void Amallow::dashOn(float acc, float speed)
{
	if (dashState >= 0.33)
	{
		GetCharacterMovement()->MaxAcceleration = acc;
		GetCharacterMovement()->MaxWalkSpeed = speed;
		doubleMove = true;
		dashState -= 0.33;
	}
}

void Amallow::ToggleMenu()
{
	if (bMenuShow == false)
	{
		PC->Menu->SetVisibility(ESlateVisibility::Visible);
		bMenuShow = true;
		Pause();
	}
}

void Amallow::ToggleInv()
{
	if (bInvShow == false)
	{
		bInvShow = true;
		HUD->bDrawInv = true;
		Pause();
	}
	else
	{
		PC->Resume();
	}
}

void Amallow::Pause()
{
	bAcceptInput = false;
	PC->bShowMouseCursor = true;
	PC->SetPause(true);
}

void Amallow::Pickup()
{
	bPickup = true;
}

void Amallow::StopPickup()
{
	bPickup = false;
}

void Amallow::LMouseClicked()
{
	HUD->bLMouseClicked = true;
}

void Amallow::LMouseReleased()
{
	HUD->bLMouseClicked = false;
}

void Amallow::FlameCharger()
{
	if (bUsingFlame == true)
	{
		if (FlameCharge >= 0)
		{
			FlameCharge -= 0.001;
		}
	}
	else
	{
		if (FlameCharge <= 1)
		{
			FlameCharge += 0.001;
		}
	}
}