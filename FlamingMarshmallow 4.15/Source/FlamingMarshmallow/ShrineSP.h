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

	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *base;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *stand;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *torus;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *pillar1;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *pillar2;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *pillar3;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *pillar4;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *pillar1Top;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *pillar2Top;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *pillar3Top;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UStaticMeshComponent *pillar4Top;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UParticleSystemComponent *torusPS;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UParticleSystemComponent *pillar1PS;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UParticleSystemComponent *pillar2PS;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UParticleSystemComponent *pillar3PS;
	UPROPERTY(VisibleAnywhere, Category = Visual)
	UParticleSystemComponent *pillar4PS;

	float TimeRunning;
	
	
};
