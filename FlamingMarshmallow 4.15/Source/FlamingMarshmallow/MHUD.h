// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MHUD.generated.h"


USTRUCT()
struct FCoreItemData
{
	GENERATED_BODY()

	FString Name;
	UStaticMesh *Mesh;
	UTexture *InvImage;
	FVector scale;
	FVector offset;
	FVector Location;
	float respawnTime;

	bool bFood = false;
	float Health = 0;

	bool bSpeed = false;
	float Speed = 0;
	float SpeedTime = 0;

	bool bDamage = false;
	float Damage = 0;
	float DamageTime = 0;

	bool Active;
};

UCLASS()
class FLAMINGMARSHMALLOW_API AMHUD : public AHUD
{
	GENERATED_BODY()

public:
	//positioning
	float width;
	float height;

	float InvX;
	float InvY;
	float SlotWH;

	float InvHeight;
	float border;
	float InvWidth;

	float resumeX;
	float resumeY;

	float dropX;
	float dropY;
	float dropWidth;
	float dropHeight;

	float useX;
	float useY;
	float useWidth;
	float useHeight;

	float selectedBoxX;
	float selectedBoxY;
	float selectedBoxWidth;
	float selectedBoxHeight;

	int rows;
	int capacity;
	int used;

	bool bDrawInv = false;
	bool bLMouseClicked = false;
	bool bPickupPrompt = false;

	//selected item
	FCoreItemData selected;
	int selectedIndex = -1;
	FCoreItemData initializer;

	//items
	TArray<FCoreItemData> Slots;

	FTimerHandle FTSpeed;
	FTimerHandle  FTDamage;

	float SpeedTime;
	float DamageTime;

	void SpeedTimer();
	void DamageTimer();

	virtual void DrawHUD() override;
	void CheckHitboxes();
	void drawInv();
	void dropItem();
	void useItem();
	void drawPickupPrompt();

	virtual void BeginPlay() override;
};
