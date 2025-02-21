// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARAction.h"
#include "ARAction_ProjectileAttack.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARAction_ProjectileAttack : public UARAction
{
	GENERATED_BODY()
	
	protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;
	
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);


	public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	UARAction_ProjectileAttack();

};