// Fill out your copyright notice in the Description page of Project Settings.

#include "FlamingMarshmallow.h"
#include "Shrine.h"


// Sets default values for this component's properties
UShrine::UShrine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	




	
}


// Called when the game starts
void UShrine::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShrine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

