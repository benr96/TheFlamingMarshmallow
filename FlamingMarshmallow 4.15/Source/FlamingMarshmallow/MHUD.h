// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MHUD.generated.h"

/**
 * 
 */
UCLASS()
class FLAMINGMARSHMALLOW_API AMHUD : public AHUD
{
	GENERATED_BODY()

	float width;
	float height;

	virtual void DrawHUD() override;
	void DrawDashStamina();

	
	
};
