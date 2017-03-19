// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "ShrineSP.h"


//Sets default values
AShrineSP::AShrineSP()
{
	 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//init static meshes comps
	base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("base"));
	stand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("stand"));
	torus = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("torus"));

	pillar1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar1"));
	pillar1Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar1Top"));
	pillar1PS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Pillar1PS"));

	pillar2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar2"));
	pillar2Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar2Top"));
	pillar2PS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Pillar2PS"));


	pillar3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar3"));
	pillar3Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar3Top"));
	pillar3PS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Pillar3PS"));


	pillar4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar4"));
	pillar4Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pillar4Top"));
	pillar4PS = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Pillar4PS"));

	
	//load the assets
	static ConstructorHelpers::FObjectFinder<UStaticMesh> baseAsset(TEXT("/Game/Shrine_Cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> standAsset(TEXT("/Game/Shrine_Cube"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> torusAsset(TEXT("/Game/Shrine_Torus"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> torusPSAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> pillar1Asset(TEXT("/Game/Shrine_Cylinder_001"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> pillar1TopAsset(TEXT("/Game/Shrine_Cylinder_002"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> pillar1PSAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> pillar2Asset(TEXT("/Game/Shrine_Cylinder_003"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> pillar2TopAsset(TEXT("/Game/Shrine_Cylinder_004"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> pillar2PSAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> pillar3Asset(TEXT("/Game/Shrine_Cylinder_005"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> pillar3TopAsset(TEXT("/Game/Shrine_Cylinder_006"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> pillar3PSAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> pillar4Asset(TEXT("/Game/Shrine_Cylinder_007"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> pillar4TopAsset(TEXT("/Game/Shrine_Cylinder_008"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> pillar4PSAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	
	//if asset loading succeeded put it on static mesh
	if (baseAsset.Succeeded())
	{
		base->SetStaticMesh(baseAsset.Object);
	}

	if (standAsset.Succeeded())
	{
		stand->SetStaticMesh(standAsset.Object);
	}

	if (torusAsset.Succeeded())
	{
		torus->SetStaticMesh(torusAsset.Object);
	}
	
	/*
	if (torusPSAsset.Succeeded())
	{
		torusPS->SetTemplate(torusPSAsset.Object);
	}

	*/
	if (pillar1Asset.Succeeded())
	{
		pillar1->SetStaticMesh(pillar1Asset.Object);
	}

	if (pillar1TopAsset.Succeeded())
	{
		pillar1Top->SetStaticMesh(pillar1TopAsset.Object);
	}

	
	if (pillar1PSAsset.Succeeded())
	{
		pillar1PS->SetTemplate(pillar1PSAsset.Object);
	}
	

	if (pillar2Asset.Succeeded())
	{
		pillar2->SetStaticMesh(pillar2Asset.Object);
	}

	if (pillar2TopAsset.Succeeded())
	{
		pillar2Top->SetStaticMesh(pillar2TopAsset.Object);
	}
	
	if (pillar2PSAsset.Succeeded())
	{
		pillar2PS->SetTemplate(pillar2PSAsset.Object);
	}
	

	if (pillar3Asset.Succeeded())
	{
		pillar3->SetStaticMesh(pillar3Asset.Object);
	}

	if (pillar3TopAsset.Succeeded())
	{
		pillar3Top->SetStaticMesh(pillar3TopAsset.Object);
	}
	
	if (pillar3PSAsset.Succeeded())
	{
		pillar3PS->SetTemplate(pillar3PSAsset.Object);
	}
	

	if (pillar4Asset.Succeeded())
	{
		pillar4->SetStaticMesh(pillar4Asset.Object);
	}

	if (pillar4TopAsset.Succeeded())
	{
		pillar4Top->SetStaticMesh(pillar4TopAsset.Object);
	}

	
	if (pillar4PSAsset.Succeeded())
	{
		pillar4PS->SetTemplate(pillar4PSAsset.Object);
	}
	
	//set root comp
	RootComponent = base;

	//set attachments
	stand->SetupAttachment(base);

	torus->SetupAttachment(stand);
	//torusPS->SetupAttachment(torus);

	pillar1->SetupAttachment(base);
	pillar1Top->SetupAttachment(pillar1);
	pillar1PS->SetupAttachment(pillar1);

	pillar2->SetupAttachment(base);
	pillar2Top->SetupAttachment(pillar2);
	pillar2PS->SetupAttachment(pillar2);

	pillar3->SetupAttachment(base);
	pillar3Top->SetupAttachment(pillar3);
	pillar3PS->SetupAttachment(pillar3);

	pillar4->SetupAttachment(base);
	pillar4Top->SetupAttachment(pillar4);
	pillar4PS->SetupAttachment(pillar4);

	//make any adjustments to defaults required
	pillar1->SetRelativeLocation(FVector(0, 0, -20));
	pillar2->SetRelativeLocation(FVector(0, 0, -20));
	pillar3->SetRelativeLocation(FVector(0, 0, -20));
	pillar4->SetRelativeLocation(FVector(0, 0, -20));

	pillar1PS->SetRelativeLocation(FVector(740, 750, 300));
	pillar2PS->SetRelativeLocation(FVector(739, -728, 300));
	pillar3PS->SetRelativeLocation(FVector(-747, 766, 300));
	pillar4PS->SetRelativeLocation(FVector(-730, -739, 300));

	
	pillar1PS->bAutoActivate = true;
	pillar2PS->bAutoActivate = true;
	pillar3PS->bAutoActivate = true;
	pillar4PS->bAutoActivate = true;

	//torusPS->bAutoActivate = true;
	
}


// Called when the game starts or when spawned
void AShrineSP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShrineSP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPos = torus->RelativeLocation;
	
	float DeltaHeight = (FMath::Sin(TimeRunning + DeltaTime) - FMath::Sin(TimeRunning));
	NextPos.Z += DeltaHeight * 20.0f;
	TimeRunning += DeltaTime;
	
	torus->SetRelativeLocation(NextPos);


	//make torus hover and maybe rotate

	//make pillar tops hover and add flames to each one

}

