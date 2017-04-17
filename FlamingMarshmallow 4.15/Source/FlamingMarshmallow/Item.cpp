// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "Item.h"
#include "mallow.h"
#include "UI_Controller.h"

Amallow *mainChar;
/*
	KNOWN BUGs: 
	
	1.when item is respawned trigger enter and trigger enter 1 are both called, the trigger exits are not called.
	this happnes when you are not in the tigger area. due to exit not being called you can pick up the item from a distance.
	This is related to setting actor collision to false when pickup is called, and then true when respawned. Without this change
	it does not happen, however then there is the problem of having an invisible obstacle.
	
	**Possibly to do with the items being close together actually, triggering each other functions

	2. When items are dropped on top of each other the text is drawn over each other being unreadable, condsider making it just say
	how many items there are
	
	3. When items are dropped on top of each other pressing e only pickups up 1, you then need to leave the trigger zone
	and reenter to pickup the next one. consider making it so if an item is already there drop it 50cm to the right or something.
	until it finds an empty space
*/

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Sphere for pickup
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetWorldScale3D(FVector(2,2,2));
	Sphere->SetRelativeLocation(FVector(0, 0, 0));

	Sphere->bGenerateOverlapEvents = true;

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::TriggerEnter);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::TriggerExit);
	RootComponent = Sphere;



	//Sphere for label
	SphereOuter = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOuter"));
	SphereOuter->SetupAttachment(RootComponent);
	SphereOuter->SetWorldScale3D(FVector(7, 7, 7));
	SphereOuter->SetRelativeLocation(FVector(0, 0, 0));

	SphereOuter->bGenerateOverlapEvents = true;

	SphereOuter->OnComponentBeginOverlap.AddDynamic(this, &AItem::TriggerEnter1);
	SphereOuter->OnComponentEndOverlap.AddDynamic(this, &AItem::TriggerExit1);

	

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);	

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetupAttachment(RootComponent);
	
	Text->SetVisibility(false);
	Text->SetWorldSize(10);
}

//used when recreating an item from a saved struct
void AItem::Initializer(FString name, UStaticMesh *Mesh, UTexture *InvImage, FVector scale, FVector offset)
{
	AUI_Controller *PC = (AUI_Controller*)GetWorld()->GetFirstPlayerController();

	mainChar = PC->mainChar;

	ItemName = name;
	this->Mesh->SetStaticMesh(Mesh);
	this->InvImage = InvImage;
	RespawnTimer = 0;

	FVector Location = mainChar->GetCharacterMovement()->GetActorFeetLocation();
	SetActorLocation(Location);

	this->Mesh->SetWorldScale3D(scale);
	this->Mesh->SetRelativeLocation(offset);

}


// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	AUI_Controller *PC = (AUI_Controller*)GetWorld()->GetFirstPlayerController();

	mainChar = PC->mainChar;

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	scale = Mesh->GetComponentScale();

	if (Text->IsVisible() == true)
	{
		drawText();
	}

	if (mainChar != NULL && bHidden == false)
	{
		if (mainChar->bPickup == true && bItemIsWithinRange == true)
		{
			Pickup();
			mainChar->bPickup = false;
		}
	}
}

void AItem::TriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHidden == false)
	{
		bItemIsWithinRange = true;
		mainChar->HUD->bPickupPrompt = true;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, "TRIGGER ENTER");
	}
}

void AItem::TriggerExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bHidden == false)
	{
		bItemIsWithinRange = false;
		mainChar->HUD->bPickupPrompt = false;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, "TRIGGER EXIT");
	}
}

void AItem::TriggerEnter1(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHidden == false)
	{
		Text->SetVisibility(true);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, "TRIGGER ENTER1");
	}
}

void AItem::TriggerExit1(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bHidden == false)
	{
		Text->SetVisibility(false);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, "TRIGGER EXIT 1");

	}
}

void AItem::Pickup()
{
	bFullCheck = true;

	for (int i = 0; i < mainChar->HUD->Slots.Num(); i++)
	{
		if (mainChar->HUD->Slots[i].Active == false)
		{
			bFullCheck = false;
			mainChar->HUD->Slots[i].Name = ItemName;
			mainChar->HUD->Slots[i].Mesh = Mesh->GetStaticMesh();
			mainChar->HUD->Slots[i].InvImage = InvImage;
			mainChar->HUD->Slots[i].Active = true;
			mainChar->HUD->Slots[i].scale = scale;
			mainChar->HUD->Slots[i].offset = Mesh->RelativeLocation;
			break;
		}
	}

	//if full display message stating such
	if (bFullCheck == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, "Inventory Full");
	}
	else
	{
		if (RespawnTimer == 0)
		{
			Destroy();
		}
		else
		{
			bHidden = true;
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AItem::Respawn, RespawnTimer, true);
			bItemIsWithinRange = false;
			mainChar->HUD->bPickupPrompt = false;
			Text->SetVisibility(false);
		}

		mainChar->HUD->used++;
	}


}

void AItem::Respawn()
{
	bHidden = false;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	bItemIsWithinRange = false;
	mainChar->HUD->bPickupPrompt = false;
	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
}

void AItem::drawText()
{
	Text->SetText(FText::FromString(ItemName));
	Text->SetHorizontalAlignment(EHTA_Center);
	Text->SetRelativeLocation(FVector(0, 0, 25));

	FVector TextLoc = Text->GetComponentLocation();
	FVector PlayerLoc = mainChar->FollowCamera->GetComponentLocation();

	FVector diff = TextLoc - PlayerLoc;

	FRotator fullRot = diff.Rotation();
	FRotator XRot = FRotator(0, fullRot.Yaw - 180, 0);
	Text->SetRelativeRotation(XRot);
}
