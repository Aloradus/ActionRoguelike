// Fill out your copyright notice in the Description page of Project Settings.


#include "ARActionComponent.h"
#include "ARAction.h"

void UARActionComponent::AddAction(TSubclassOf<UARAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	 }

	UARAction* NewAction = NewObject<UARAction>(this, ActionClass);

	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UARActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UARActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

// Sets default values for this component's properties
UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UARActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

