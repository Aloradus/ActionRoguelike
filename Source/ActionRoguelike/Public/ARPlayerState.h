// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
                       
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreditChange, float, Delta, float, CreditsHeld);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	public:

	AARPlayerState();

	UPROPERTY(BlueprintAssignable)
	FOnCreditChange OnCreditChage;

	//Not necessary - Read more on Replicated Properties
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_Credits();

	UFUNCTION(BlueprintCallable, Category = "Credits")
    void SetCredits(float NewValue);

	UFUNCTION(BlueprintGetter)
	float GetCredits();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(float CreditsToAdd);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(float CreditsToRemove);

	virtual void PostInitializeComponents() override;

	protected:
	

	private:

	UPROPERTY(ReplicatedUsing = OnRep_Credits, Category = PlayerState, BlueprintGetter = GetCredits)
    float Credits;

};
