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
	bool bEdible;
	float regen;
	bool Active;

};

UCLASS()
class FLAMINGMARSHMALLOW_API AMHUD : public AHUD
{
	GENERATED_BODY()

public:
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


	FCoreItemData selected;
	int selectedIndex = -1;
	FCoreItemData initializer;

	TArray<FCoreItemData> Slots;

	virtual void DrawHUD() override;
	void CheckHitboxes();
	void drawInv();
	void dropItem();
	void useItem();
	void drawPickupPrompt();

	virtual void BeginPlay() override;
};
