// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FlamingMarshmallow.h"
#include "MHUD.h"
#include "UI_Controller.h"
#include "Item.h"
#include "mallow.h"

/*
	KNOWN BUGS: 

	1. Mouse focus is an issue, when closing inventory you have to click to give focus back to the game
	it causes a limit to turning

	2.food is allowed to be eaten when health is already full, fix this
*/

void AMHUD::BeginPlay()
{
	initializer.Name = "";
	initializer.InvImage = NULL;
	initializer.Mesh = NULL;
	initializer.Active = false;

	Slots.Init(initializer, 30);
	
	selected = initializer;

	const FVector2D ScreenSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	width = ScreenSize.X;
	height = ScreenSize.Y;

	InvX = width * 0.15;
	InvY = height * 0.15;
	SlotWH = (width * 0.25 / 6);

	InvHeight = height * 0.75;
	border = width * 0.25*0.05;
	InvWidth = SlotWH * 5 + border * 6;

	resumeX = InvX + border;
	resumeY = InvY + InvHeight - SlotWH - border;

	selectedBoxX = InvX + InvWidth + border;
	selectedBoxY = InvY;
	selectedBoxWidth = InvWidth / 2;
	selectedBoxHeight = InvHeight / 3;

	dropX = selectedBoxX + border;
	dropWidth = (selectedBoxWidth - border * 3)/2;
	dropHeight = SlotWH * 0.75;
	dropY = selectedBoxY + selectedBoxHeight - border - dropHeight;

	useX = selectedBoxX + (border * 2) + dropWidth;
	useY = dropY;
	useWidth = dropWidth;
	useHeight = dropHeight;

	boostBoxWidth = width / 7;
	boostBoxHeight = height / 10;
	boostBoxX = width - boostBoxWidth - border;
	boostBoxY = border;

	rows = Slots.Num() / 5;

	for (int row = 0; row < rows; row++)
	{
		int count = 0;
		int first = row * 5;

		for (int j = first; j < first + 5; j++)
		{
			float x = (InvX + (border*(count + 1))) + SlotWH*count;
			float y = InvY +SlotWH + (SlotWH*row) + (border*(row + 1));

			FString n = FString::FromInt((j));
			FName name = FName(*n);

			AddHitBox(FVector2D(x, y), FVector2D(SlotWH, SlotWH), name, true, j);

			count++;
		}
	}

	AddHitBox(FVector2D(resumeX, resumeY), FVector2D(SlotWH * 2, SlotWH), TEXT("30"), true, 100);
	AddHitBox(FVector2D(dropX, dropY), FVector2D(dropWidth, dropHeight), TEXT("31"), true, 100);
	AddHitBox(FVector2D(useX, useY), FVector2D(useWidth, useHeight), TEXT("32"), true, 100);

	used = 0;
	capacity = Slots.Num();
}

void AMHUD::DrawHUD()
{
	if (bDrawInv == true)
	{
		drawInv();
	}

	if (bPickupPrompt == true)
	{
		drawPickupPrompt();
	}

	CheckHitboxes();
	drawBoosts();
}

void AMHUD::drawInv()
{
	float textWidth;
	float textHeight;

	//Inventory Box
	DrawRect(FLinearColor(0.1, 0.1, 0.1, 0.75), InvX, InvY, InvWidth, InvHeight);

	//Resume Button
	DrawRect(FLinearColor(0.01, 0.01, 0.01, 0.75), resumeX, resumeY, SlotWH * 2, SlotWH);

	GetTextSize("Resume", textWidth, textHeight,0,1.25);
	DrawText("Resume", FLinearColor(1, 1, 1), resumeX + SlotWH - textWidth/2 , resumeY+SlotWH/2 - textHeight/2,0,1.25);

	GetTextSize("INVENTORY", textWidth, textHeight, 0, 1.25);
	DrawText("INVENTORY", FLinearColor(1, 1, 1), InvX+InvWidth/2 - textWidth/2, InvY+textHeight,0,1.25);

	FString cap = FString::FromInt(used) + "/" + FString::FromInt(capacity);

	DrawText(cap, FLinearColor(1, 1, 1), resumeX+SlotWH * 3, resumeY,0,1.25);


	if (bAlreadyActive == true)
	{
		DrawText(msg, FLinearColor(1, 0, 0), resumeX + SlotWH * 3, resumeY + textHeight, 0, 1.25);
	}

	for (int row = 0; row < rows; row++)
	{
		int count = 0;
		int first = row * 5;
	
		for (int j = first; j < first + 5; j++)
		{
			float x = (InvX + (border*(count + 1))) + SlotWH*count;
			float y = InvY +SlotWH + (SlotWH*row) + (border*(row + 1));


			if (Slots[j].Active == true)
			{
				DrawTexture(Slots[j].InvImage, x, y, SlotWH, SlotWH, 60, 60, 1, 1);
			}
			else
			{
				DrawRect(FLinearColor(0.3, 0.3, 0.3, 0.75), x, y, SlotWH, SlotWH);
			}

			if (selectedIndex == j)
			{
				DrawLine(x, y, x + SlotWH, y, FLinearColor(1, 0, 0));
				DrawLine(x + SlotWH, y, x + SlotWH, y + SlotWH, FLinearColor(1, 0, 0));
				DrawLine(x + SlotWH, y + SlotWH, x, y + SlotWH, FLinearColor(1, 0, 0));
				DrawLine(x, y + SlotWH, x, y, FLinearColor(1, 0, 0));

			}

			count++;
		}
	}

	//if an item is selected
	if (selected.Active == true)
	{
		FString Line1;
		FString Line2;
		GetTextSize(selected.Name, textWidth, textHeight, 0, 1.25);

		DrawRect(FLinearColor(0.1, 0.1, 0.1, 0.75), selectedBoxX, selectedBoxY, selectedBoxWidth, selectedBoxHeight);
		DrawText(selected.Name, FLinearColor(1, 1, 1), selectedBoxX + selectedBoxWidth/2 - textWidth/2, selectedBoxY+textHeight,0,1.25);

		//Drop Button
		GetTextSize("Drop", textWidth, textHeight, 0, 1.25);
		DrawRect(FLinearColor(0.01, 0.01, 0.01, 0.75), dropX, dropY, dropWidth, dropHeight);
		DrawText("Drop", FLinearColor(1, 1, 1), dropX + dropWidth/2 - textWidth/2, dropY + dropHeight/2 - textHeight/2,0,1.25);

		//Use Button
		GetTextSize("Use", textWidth, textHeight, 0, 1.25);
		DrawRect(FLinearColor(0.01, 0.01, 0.01, 0.75), useX, useY, useWidth, useHeight);
		DrawText("Use", FLinearColor(1, 1, 1), useX + useWidth/2 - textWidth/2, useY + useHeight/2 - textHeight/2,0,1.25);

		if (selected.bFood == true)
		{
			Line1 = "Health: +" + FString::SanitizeFloat(selected.Health);
			Line2 = "";
		}

		if (selected.bSpeed == true)
		{
			Line1 = "Speed: +" + FString::SanitizeFloat(selected.Speed) + "%";
			Line2 = "Duration: " + FString::SanitizeFloat(selected.SpeedTime) + "s";
		}

		if (selected.bDamage == true)
		{
			Line1 = "Damage: +" + FString::SanitizeFloat(selected.Damage) + "%";
			Line2 = "Duration: " + FString::SanitizeFloat(selected.DamageTime) + "s";
		}


		DrawText(Line1, FLinearColor(1, 1, 1), selectedBoxX + border, selectedBoxY + border + textHeight*2,0,1.25);
		DrawText(Line2, FLinearColor(1, 1, 1), selectedBoxX + border, selectedBoxY + border + textHeight * 3,0,1.25);
	}


}

void AMHUD::CheckHitboxes()
{
	if (bLMouseClicked == true && bDrawInv == true)
	{
		for (int i = 0; i < HitBoxHits.Num(); i++)
		{
			FString name = HitBoxHits[i]->GetName().ToString();//make name string

			int index = FCString::Atoi(*name);// make name int


			//if hit box is one of the inv slots
			if (index >= 0 && index <= 29)
			{
				FString ItemName = Slots[index].Name;//get the item name

				selected = Slots[index];
				selectedIndex = index;
			}

			//if hit box is the resume button
			if (HitBoxHits[i]->GetName() == TEXT("30"))
			{
				//reset selected
				selected = initializer;
				selectedIndex = -1;

				AUI_Controller *PC = (AUI_Controller*)(GetWorld()->GetFirstPlayerController());
				PC->Resume();//run resume function
			}

			//if hitbox is drop and something is selected
			if (HitBoxHits[i]->GetName() == TEXT("31") && selected.Active == true)
			{
				dropItem();
			}

			//if hit box is use and something is selected
			if (HitBoxHits[i]->GetName() == TEXT("32") && selected.Active == true)
			{
				useItem();
			}
		}
	}
}

void AMHUD::dropItem()
{
	AUI_Controller *PC = (AUI_Controller*)GetWorld()->GetFirstPlayerController();
	Amallow  *mainChar = PC->mainChar;

	bool bValidDropLocation = false;
	bool bCheck = true;
	
	FVector location = FVector(mainChar->FloorLoc.X + 50, mainChar->FloorLoc.Y, mainChar->FloorLoc.Z);

	//set items struct location to be directly below the player
	selected.Location = location;
	selected.respawnTime = 0;

	//spawn and initialze the item
	AItem *dropped = GetWorld()->SpawnActor<AItem>(AItem::StaticClass());
	dropped->Initializer(&selected);

	//remove from inv
	Slots[selectedIndex].Active = false;
	selected.Active = false;
	selectedIndex = -1;
	used--;
}

void AMHUD::useItem()
{
	AUI_Controller *PC = (AUI_Controller*)GetWorld()->GetFirstPlayerController();
	Amallow *mainChar = PC->mainChar;

	//if item is edible apply changes
	if (selected.bFood == true && mainChar->health < 100)
	{
		bAlreadyActive = false;
		mainChar->health += selected.Health;

		if (mainChar->health > 100)
		{
			mainChar->health = 100;
		}

		Slots[selectedIndex].Active = false;
		selected.Active = false;
		selectedIndex = -1;
		used--;
	}
	else
	{
		bAlreadyActive = true;
		msg = "Health Already Full";
	}

	if (selected.bSpeed == true)
	{
		if (GetWorldTimerManager().IsTimerActive(FTSpeed) == true)
		{
			bAlreadyActive = true;
			msg = "Speed Boost Already Active";
		}
		else
		{
			bAlreadyActive = false;

			float dif = mainChar->MaxSpeed * selected.Speed/100;

			mainChar->MaxSpeed += dif;
			mainChar->GetCharacterMovement()->MaxWalkSpeed = mainChar->MaxSpeed;

			Slots[selectedIndex].Active = false;
			selected.Active = false;
			selectedIndex = -1;
			used--;

			SpeedTime = selected.SpeedTime;
			Speed = selected.Speed;
			GetWorldTimerManager().SetTimer(FTSpeed, this, &AMHUD::SpeedTimer, 1, true);
		}
	}

	if (selected.bDamage == true)
	{
		if (GetWorldTimerManager().IsTimerActive(FTDamage) == true)
		{
			bAlreadyActive = true;
			msg = "Damage Boost Already Active";
		}
		else
		{
			bAlreadyActive = false;
			float dif = mainChar->damage *selected.Damage / 100;
			mainChar->damage += dif;
			mainChar->dif = dif;

			Slots[selectedIndex].Active = false;
			selected.Active = false;
			selectedIndex = -1;
			used--;

			DamageTime = selected.DamageTime;
			Damage = selected.Damage;
			GetWorldTimerManager().SetTimer(FTDamage, this, &AMHUD::DamageTimer, 1, true);
		}
	}
}

void AMHUD::drawPickupPrompt()
{
	DrawText("Press X to pickup", FLinearColor(1, 1, 0), width/10, height/20,0,1.5);
}


void AMHUD::SpeedTimer()
{
	AUI_Controller *PC = (AUI_Controller*)GetWorld()->GetFirstPlayerController();
	Amallow *mainChar = PC->mainChar;

	SpeedTime -= 1;

	if (SpeedTime < 0)
	{
		GetWorldTimerManager().ClearTimer(FTSpeed);
		mainChar->MaxSpeed = mainChar->BaseMaxSpeed;
		mainChar->GetCharacterMovement()->MaxWalkSpeed = mainChar->MaxSpeed;
	}
}

void AMHUD::DamageTimer()
{
	AUI_Controller *PC = (AUI_Controller*)GetWorld()->GetFirstPlayerController();
	Amallow *mainChar = PC->mainChar;

	DamageTime -= 1;

	if (DamageTime < 0)
	{
		GetWorldTimerManager().ClearTimer(FTDamage);
		mainChar->damage = mainChar->BaseDamage;
	}
}

void AMHUD::drawBoosts()
{
	float textWidth;
	float textHeight;

	bool bActive = false;

	DrawRect(FLinearColor(0.1, 0.1, 0.1, 0.75), boostBoxX, boostBoxY, boostBoxWidth, boostBoxHeight);
	DrawLine(boostBoxX, boostBoxY, boostBoxX + boostBoxWidth, boostBoxY, FLinearColor(1, 0, 0), 2);
	DrawLine(boostBoxX + boostBoxWidth, boostBoxY,boostBoxX+boostBoxWidth,boostBoxY+boostBoxHeight,FLinearColor(1, 0, 0), 2);
	DrawLine(boostBoxX + boostBoxWidth, boostBoxY + boostBoxHeight, boostBoxX, boostBoxY + boostBoxHeight, FLinearColor(1, 0, 0), 2);
	DrawLine(boostBoxX, boostBoxY + boostBoxHeight, boostBoxX, boostBoxY, FLinearColor(1, 0, 0), 2);

	if (GetWorldTimerManager().IsTimerActive(FTSpeed) == true)
	{
		FString msg = "Speed +" + FString::SanitizeFloat(Speed) + "% : " + FString::SanitizeFloat(SpeedTime);
		DrawText(msg, FLinearColor(1, 1, 1,0.75), boostBoxX+border,boostBoxY + border / 2,0,1.25);

		bActive = true;
	}

	if (GetWorldTimerManager().IsTimerActive(FTDamage) == true)
	{
		FString msg = "Damage +" + FString::SanitizeFloat(Damage) + "% : " + FString::SanitizeFloat(DamageTime);
		GetTextSize(msg, textWidth, textHeight, 0, 1.25);
		DrawText(msg, FLinearColor(1, 1, 1,0.75), boostBoxX + border, boostBoxY + textHeight*2,0,1.25);

		bActive = true;
	}

	if (bActive == false)
	{
		FString no = "No Active";
		FString boost = "Boosts";

		GetTextSize(no, textWidth, textHeight,0, 1.25);
		DrawText(no, FLinearColor(1, 1, 1, 0.75), boostBoxX + boostBoxWidth / 2 - textWidth / 2, boostBoxY + border, 0, 1.25);

		GetTextSize(boost, textWidth, textHeight, 0, 1.25);
		DrawText(boost, FLinearColor(1, 1, 1, 0.75), boostBoxX + boostBoxWidth / 2 - textWidth / 2, boostBoxY + border + textHeight, 0, 1.25);
	}
}