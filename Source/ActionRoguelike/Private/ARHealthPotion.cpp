// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPotion.h"

// Sets default values
AARHealthPotion::AARHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

}

// Called when the game starts or when spawned
void AARHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARHealthPotion::HideAndCoolDown()
{

}

void AARHealthPotion::SetPotionState(bool bIsActive)
{

}

void AARHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	throw std::logic_error("The method or operation is not implemented.");
}

