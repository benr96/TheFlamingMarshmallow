// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "GameFramework/Pawn.h"
#include "AI.generated.h"

UCLASS()
class FLAMINGMARSHMALLOW_API AAI : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMeshAssets, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* aiMesh;

	void moveAI();
	void followMallow();

	//Combat variables and functions
	float health;
	float damage;
	float delayForAttack = 1.5f;
	float lastTimeInRange;
	float attackRange = 75.f;
	bool bfirstTime = true;
	bool bCanAttack;

	void Attack();
	void CheckRangeToChar();

	//Targeting variables
	float yPos = 300;
	float inc = 1.0f;
	float xPos;
	float left;
	bool bHitLimit = false;
	bool bInAttackRange = false;
	bool bInTargetRange = false;
	float distToPlayer;
	FRotator rotationFromChar;

	FString AIname;
};
