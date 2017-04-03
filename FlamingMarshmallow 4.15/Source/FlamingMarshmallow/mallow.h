// Fill out your copyright notice in the Description page of Project Settings.
//stamina for dashing
//face where camera is looking, maybe only for when locked on
//limit camera movement so you don't glitch through mallow
#pragma once

#include "GameFramework/Character.h"
#include "AI.h"
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
		UStaticMeshComponent* LeftEyeVis;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* RightEyeVis;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* Flames;

	UTextRenderComponent* Speed;


public:
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float TurnRate;
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float LookRate;
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float MaxSpeed;
	UPROPERTY(VisibleAnywhere, Category = Movement)
		float JumpVelocity;

	// Variables for targeting system
	UFUNCTION()
		void CameraPitch(float fAmount);
	UFUNCTION()
		void CameraYaw(float fAmount);
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float fMouseSensitivity;
	bool bLockOn = false;


	bool midJump;

	bool right;
	bool left;
	bool forward;
	bool back;

	//functions for movement
	void MoveForward();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void StopForward();
	void StopBack();
	void StopLeft();
	void StopRight();
	void IncFTime();
	void IncBTime();
	void IncRTime();
	void IncLTime();
	void ToggleFire();

	FTimerHandle ForwardTimer;
	float forwardTime;
	FTimerHandle BackTimer;
	float backTime;
	FTimerHandle LeftTimer;
	float leftTime;
	FTimerHandle RightTimer;
	float rightTime;
	float originalTime;
	bool doubleMove;

	FVector CheckDirection(FString Axis);
	void movementControl();
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void StartRun();
	void StopRun();
	void jump();
	void dashOn(float acc = 20000000000, float speed = 2500);
	void dashOff(FTimerHandle handle, float *time);

	void LockOnEnemy();
	void LockRightEnemy();
	void LockLeftEnemy();
	void TargetEnemy();
	void FindRight();
	void FindLeft();
	void SortEnemies();

	TArray<AAI*> TestAI;
	AAI* ai1;
	FVector AILocation;
	float mousePitch;
	float mouseYaw;
	int next;
	int highest;
	struct AI_Struct
	{
		float distanceFromChar;
		FRotator rotationFromCenter;
		AAI* sAI;
	};
	TArray<AI_Struct*> enemy_Array;
	bool sortOrNah;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
