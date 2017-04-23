// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "UI_Controller.h"
#include "Blueprint/UserWidget.h"
#include "mallow.h"

void AUI_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (wHUD)
	{
		HUD = CreateWidget<UUserWidget>(this, wHUD);

		if (HUD)
		{
			HUD->AddToViewport();
			HUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (wMenu)
	{
		Menu = CreateWidget<UUserWidget>(this, wMenu);

		if (Menu)
		{
			Menu->AddToViewport();
			Menu->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (wMainMenu)
	{
		MainMenu = CreateWidget<UUserWidget>(this, wMainMenu);

			if (MainMenu)
			{
				MainMenu->AddToViewport();
				MainMenu->SetVisibility(ESlateVisibility::Visible);
			}
	}

	if (wDeathMenu)
	{
		DeathMenu = CreateWidget<UUserWidget>(this, wDeathMenu);

		if (DeathMenu)
		{
			DeathMenu->AddToViewport();
			DeathMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}


	GetWorldTimerManager().SetTimer(dashRegenTimer, this, &AUI_Controller::dashRegen, 0.01f, true);
}

void AUI_Controller::Tick(float DeltaSeconds)
{

}

void AUI_Controller::Settings()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Will Open Settings When Clicked"));
}

void AUI_Controller::Quit()
{
	ConsoleCommand("Exit");
}

void AUI_Controller::Resume()
{
	bMainMenu = false;
	mainChar->bAcceptInput = true;
	mainChar->bMenuShow = false;
	mainChar->bInvShow = false;
	mainChar->HUD->bDrawInv = false;
	mainChar->HUD->bAlreadyActive = false;
	bShowMouseCursor = false;
	HUD->SetVisibility(ESlateVisibility::Visible);
	Menu->SetVisibility(ESlateVisibility::Hidden);
	MainMenu->SetVisibility(ESlateVisibility::Hidden);
	SetPause(false);
}

void AUI_Controller::Restart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AUI_Controller::dashRegen()
{
	if (mainChar->dashState <= 1)
	{
		mainChar->dashState += 0.001;
	}
}




