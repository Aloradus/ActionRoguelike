// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAction.h"
#include "ARActionComponent.h"




bool UARAction::CanStart_Implementation(AActor* Instigator)
{
	//Check to ensure the action isn't already running
	if (bIsRunning)
	{
		return false;
	}


	UARActionComponent* Comp = GetOwningComponent();

	//Check if our action component has any of this actions blocked blocked tags.
	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void UARAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running %s"), *GetNameSafe(this));

	UARActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;

}

void UARAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	UARActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UARAction::GetWorld() const
{
	//Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());

	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

UARActionComponent* UARAction::GetOwningComponent() const
{
	//We know it will be this, because when we add these in our UARActionComponent (w/ NewObject) we set the outer to our component
	return Cast<UARActionComponent>(GetOuter());
}

bool UARAction::IsRunning() const
{
	return bIsRunning;
}