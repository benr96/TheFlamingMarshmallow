// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "MHUD.h"


void AMHUD::DrawHUD()
{
	const FVector2D ScreenSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	width = ScreenSize.X;
	height = ScreenSize.Y;
	DrawDashStamina();
}

void AMHUD::DrawDashStamina()
{
	float border = width/20;
	float dashWidth = width/6;
	float dashHeight = height/15;
	float dashX = width - dashWidth - border;
	float dashY = height - dashHeight - border;

	DrawLine(dashX, dashY, dashX + dashWidth, dashY,FLinearColor(0,0,1),2);
	DrawLine(dashX + dashWidth, dashY, dashX + dashWidth, dashY + dashHeight, FLinearColor(0, 0, 1), 2);
	DrawLine(dashX + dashWidth, dashY + dashHeight, dashX, dashY + dashHeight, FLinearColor(0, 0, 1), 2);
	DrawLine(dashX, dashY + dashHeight, dashX, dashY, FLinearColor(0, 0, 1), 2);

	int dashState = 1;

	switch (dashState)
	{
		case 0://draw no bars;
		{
			break;
		}
		case 1://draw 1 bar
		{
			DrawRect(FLinearColor(0, 1, 0), dashX, dashY, dashWidth*0.33, dashHeight);
			break;
		}
		case 2://draw 2 bars
		{
			DrawRect(FLinearColor(0, 1, 0), dashX, dashY, dashWidth*0.66, dashHeight);
			break;
		}
		case 3://draw 3 bars
		{
			DrawRect(FLinearColor(0, 1, 0), dashX, dashY, dashWidth, dashHeight);
			break;
		}
	}
}