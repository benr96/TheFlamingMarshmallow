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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;

	//visual components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MallowVisual;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* MallowParticleSystem;

public:
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float TurnRate;
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float LookRate;
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float MaxSpeed;
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float JumpVelocity;


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
