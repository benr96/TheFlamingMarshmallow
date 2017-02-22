// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "mallow.generated.h"

UCLASS()
class FLAMINGMARSHMALLOW_API Amallow : public ACharacter
{
	GENERATED_BODY()

	// Sets default values for this character's properties
	Amallow();
	
	//camera components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	//visual components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual)
		UStaticMeshComponent* MallowVisual;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual)
		UParticleSystemComponent* MallowParticleSystem;

public:
	UPROPERTY()
		float TurnRate;
	UPROPERTY()
		float LookRate;

	//functions for movement
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void StartRun();
	void StopRun();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
