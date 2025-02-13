// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARHealthPotion.h"
#include "ARCoinStash.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARCoinStash : public AARHealthPotion
{
	GENERATED_BODY()

public: 

	AARCoinStash();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:

	UPROPERTY(EditDefaultsOnly)
	float CreditValue;

private:

	void ShowCoins();

	void HideAndCoolDown();

	void SetCoinState(bool bIsActive);
};
