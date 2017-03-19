// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Shrine.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLAMINGMARSHMALLOW_API UShrine : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShrine();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UStaticMeshComponent *base;
	UStaticMeshComponent *stand;
	UStaticMeshComponent *torus;
	UStaticMeshComponent *pillar1;
	UStaticMeshComponent *pillar2;
	UStaticMeshComponent *pillar3;
	UStaticMeshComponent *pillar4;
	UStaticMeshComponent *pillar1Top;
	UStaticMeshComponent *pillar2Top;
	UStaticMeshComponent *pillar3Top;
	UStaticMeshComponent *pillar4Top;


		
	
};
