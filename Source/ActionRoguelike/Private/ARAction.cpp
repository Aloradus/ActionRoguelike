// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAction.h"

void UARAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running %s"), *GetNameSafe(this));
}

void UARAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopping %s"), *GetNameSafe(this));
}
