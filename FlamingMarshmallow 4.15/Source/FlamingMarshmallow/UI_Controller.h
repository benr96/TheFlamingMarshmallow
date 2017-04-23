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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wMainMenu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wDeathMenu;

	UUserWidget *HUD;
	UUserWidget *Menu;
	UUserWidget *MainMenu;
	UUserWidget *DeathMenu;

	bool bMainMenu = true;

	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void Settings();
	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void Quit();
	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void Resume();
	UFUNCTION(BlueprintCallable, Category = "C++ Functions")
		void Restart();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	
	Amallow *mainChar;
	FTimerHandle dashRegenTimer;
	void dashRegen();
};
