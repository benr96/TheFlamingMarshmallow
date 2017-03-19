// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ShrineSP.generated.h"

UCLASS()
class FLAMINGMARSHMALLOW_API AShrineSP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShrineSP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	UParticleSystemComponent *torusPS;
	UParticleSystemComponent *pillar1PS;
	UParticleSystemComponent *pillar2PS;
	UParticleSystemComponent *pillar3PS;
	UParticleSystemComponent *pillar4PS;

	float TimeRunning;
	
	
};
