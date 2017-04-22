// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "MHUD.h"
#include "Item.generated.h"

UCLASS()
class FLAMINGMARSHMALLOW_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	//kinds of items
	bool bFood = false;
	bool bSpeed = false;
	bool bDamage = false;

	//amount to increase by
	float Health = 0;
	float Speed = 0;
	float Damage = 0;

	bool bHidden = false;
	bool bItemIsWithinRange = false;
	bool bPickupPrompt = false;
	bool bFullCheck = false;

	FVector scale;
	
	FTimerHandle RespawnTimerHandle; 
	float  RespawnTimer = 10;//0 means no respawn, item will be destroyed in world
	void Respawn();

	USphereComponent *Sphere;
	USphereComponent *SphereOuter;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent *Mesh;

	UPROPERTY(EditAnywhere)
		UTexture *InvImage;

	UPROPERTY(EditAnywhere)
		FString ItemName;

	UTextRenderComponent *Text;

	void Pickup();
	UFUNCTION()
		void TriggerEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TriggerExit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void TriggerEnter1(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void TriggerExit1(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AItem();
	void Initializer(FCoreItemData *Spawner);

	void drawText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;



	
	
};
