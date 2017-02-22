// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "mallow.h"

// Sets default values
Amallow::Amallow()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//size of capsule
	GetCapsuleComponent()->InitCapsuleSize(46.0f, 96.0f);

	//creating static mesh
	MallowVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallowVisual"));
	MallowVisual->SetupAttachment(RootComponent);

	//find the asset we want to use
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallowVisualAsset(TEXT("/Game/MarshmallowV2"));

	//if it found the asset position it correctly
	if (MallowVisualAsset.Succeeded())
	{
		MallowVisual->SetStaticMesh(MallowVisualAsset.Object);
		MallowVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		MallowVisual->SetWorldScale3D(FVector(0.8f));
	}

	//input rates
	TurnRate = 60.0f;
	LookRate = 60.0f;

	//so it doesn't rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	//move character in direction of input
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//setting rotation rate
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//setting jump velocity
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	//setting force in air
	GetCharacterMovement()->AirControl = 0.5f;


	//CAMERA
	//Init camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	//Attach camera to the root component
	CameraBoom->SetupAttachment(RootComponent);

	//Set camera boom length
	CameraBoom->TargetArmLength = 300.0f;

	//make camera rotate based on the controller
	CameraBoom->bUsePawnControlRotation = true;

	//init actual camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	//attach follow camera to socket on boom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//stop camera rotating with controller, so it just rotates with boom
	FollowCamera->bUsePawnControlRotation = false;

	//Particle System
	MallowParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MallowParticleSystem"));
	MallowParticleSystem->SetupAttachment(MallowVisual);
	MallowParticleSystem->bAutoActivate = true;//change to false when we add button to toggle the PS
	MallowParticleSystem->SetRelativeLocation(FVector(0, 0, 0));
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MallowAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));

	if (MallowAsset.Succeeded())
	{
		MallowParticleSystem->SetTemplate(MallowAsset.Object);
	}
}

// Called when the game starts or when spawned
void Amallow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Amallow::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
}

// Called to bind functionality to input
void Amallow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	//jumping
	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &ACharacter::Jump);

	//start running/stop running
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &Amallow::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &Amallow::StopRun);


	//moving forward and right
	PlayerInputComponent->BindAxis("MoveForward", this, &Amallow::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Amallow::MoveRight);

	//turning and camera control
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Amallow::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Amallow::LookUpAtRate);
}

void Amallow::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200;
}

void Amallow::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void Amallow::MoveForward(float Value)
{
	if (Controller != NULL && Value != 0.0f)
	{
		//check controller to see what the character rotation is
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//use the rotation found above to get the forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//move forward at the value input
		AddMovementInput(Direction, Value);
	}
}

void Amallow::MoveRight(float Value)
{
	if (Controller != NULL && Value != 0.0f)
	{
		//check controller to get character rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//use above rotation to find right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//move in that direction
		AddMovementInput(Direction, Value);
		
	}
}

void Amallow::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * LookRate * GetWorld()->GetDeltaSeconds());
}

void Amallow::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());
}


