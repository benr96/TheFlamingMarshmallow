// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TargetPointer.generated.h"

UCLASS()
class FLAMINGMARSHMALLOW_API ATargetPointer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetPointer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StaticMeshAssets, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent *pointer;
	FVector location;
};
