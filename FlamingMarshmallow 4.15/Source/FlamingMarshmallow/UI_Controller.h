// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "UI_Controller.generated.h"

/**
 * 
 */

class Amallow;

UCLASS()
class FLAMINGMARSHMALLOW_API AUI_Controller : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wHUD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wMenu;

	UUserWidget *HUD;
	UUserWidget *Menu;

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void Settings();
	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void Quit();
	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void Resume();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	
	Amallow *mainChar;
	FTimerHandle dashRegenTimer;
	void dashRegen();
};
