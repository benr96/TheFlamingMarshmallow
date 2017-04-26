// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyAI.generated.h"

/**
 * 
 */
UCLASS()
class FLAMINGMARSHMALLOW_API AEnemyAI : public AAIController
{
	GENERATED_BODY()
	
	/*Behavior tree comp ref*/
	class UBehaviorTreeComponent *BehaviorComp;

	/*----------Blackboard----------*/

	/*Blackboard comp ref*/
	class UBlackboardComponent *BlackboardComp;

	/*Blackboard keys*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName BlackboardKey = "Target";

	/*I use the AActor class as the Template class so I can use the GetAllActorsOfClass function.
	However, in this collection I'm going to store all the Available ABotTargetPoint instances.*/
	TArray<AActor*> BotTargetPoints;

	/*Posses is executed when the character we want to control is spawned.
	Inside this function, we initialize the blackboard and start the behavior tree*/
	virtual void Possess(APawn* Pawn) override;


public:

	/*----------Constructor----------*/
	AEnemyAI();

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return BotTargetPoints; }
	
	void SetSeenTarget(APawn* Pawn);
	
};
