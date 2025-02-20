// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARAction.generated.h"

/**
 * Must mark as blueprintable otherwise we cannot make child classes from UARAction... As a blueprint
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARAction : public UObject
{
	GENERATED_BODY()

	public:

	//Mark so it has a native implementation so blueprint can override the C++ behavior and extend it.
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

};
