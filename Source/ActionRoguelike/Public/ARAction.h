// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ARAction.generated.h"

class UWorld;
class UARActionComponent;

/**
 * Must mark as blueprintable otherwise we cannot make child classes from UARAction... As a blueprint
 */
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARAction : public UObject
{
	GENERATED_BODY()

	protected:

	bool bIsRunning;

	/*tags added to owning actor when activated, removed when action stops*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/*action can only start if OwningActor has none of these tags applied*/
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	UARActionComponent* GetOwningComponent() const;

	public:

	UFUNCTION(BlueprintCallable, Category = Action)
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	//Mark so it has a native implementation so blueprint can override the C++ behavior and extend it.
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
	
	//TODO Refactor to be FGameplayTags
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UWorld* GetWorld() const override;

};
