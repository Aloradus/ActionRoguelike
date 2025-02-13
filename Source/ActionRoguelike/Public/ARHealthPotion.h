// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARInteractiveInterface.h"
#include "ARHealthPotion.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ACTIONROGUELIKE_API AARHealthPotion : public AActor, public IARInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARHealthPotion();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly)
	float CreditCost;

	UPROPERTY(EditDefaultsOnly)
	float HealAmount;

	UPROPERTY(EditDefaultsOnly)
	float ResetTimeInSeconds;

	FTimerHandle TimerHandle_RefreshTimer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void ShowPowerup();

	void HideAndCoolDown();

	void SetPotionState(bool bIsActive);

};
