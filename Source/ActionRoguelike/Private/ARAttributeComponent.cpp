// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UARAttributeComponent::ApplyHealthChange(float Delta)
{
	Health = FMath::Clamp(Health += Delta, 0.f, MaxHealth);
	//AActor*, InstigatorActor, UARAttributeComponent*, OwningComp, float, NewHealth, float, MaxHealth, float, Delta
	OnHealthChanged.Broadcast(nullptr, this, Health, MaxHealth, Delta);

}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

// Called every frame
void UARAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

