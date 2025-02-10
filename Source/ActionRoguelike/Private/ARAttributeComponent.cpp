// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/Pawn.h"




// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

UARAttributeComponent* UARAttributeComponent::GetAttributesComp(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UARAttributeComponent>();
	}

	return nullptr;
}

bool UARAttributeComponent::IsActorAlive(AActor* Actor)
{
	UARAttributeComponent* AttributesComp = GetAttributesComp(Actor);
	if (AttributesComp)
	{
		return AttributesComp->IsAlive();
	}

	//Default behavior - no component, so must be dead
	return false;
}

// Called when the game starts
void UARAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


bool UARAttributeComponent::ApplyHealthChange(AActor* InstigatingActor, float Delta)
{

	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	float OldHealth = Health;
	Health = FMath::Clamp(Health += Delta, 0.f, MaxHealth);
	float ActualDelta = OldHealth - Health;

	if (ActualDelta != 0)
	{
		//AActor*, InstigatorActor, UARAttributeComponent*, OwningComp, float, NewHealth, float, MaxHealth, float, Delta
		OnHealthChanged.Broadcast(InstigatingActor, this, Health, MaxHealth, ActualDelta);

		return true;
	}
	return false;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool UARAttributeComponent::IsHealthLow() const
{
	return Health <= FlagLowHealthAt;
}

void UARAttributeComponent::Initalize(bool AIControlled, APawn* ControllingPawn)
{
	bAIControlled = AIControlled;
	OwningPawn = ControllingPawn;
}

// Called every frame
void UARAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UARAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}

