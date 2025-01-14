// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARInteractiveInterface.h"
#include "ARHealthPotion.generated.h"

class UStaticMeshComponent;
class UPrimitiveComponent;

UCLASS()
class ACTIONROGUELIKE_API AARHealthPotion : public AActor, public IARInteractiveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARHealthPotion();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	float HealAmount;

	UPROPERTY(EditDefaultsOnly)
	float ResetTimeInSeconds;

	FTimerHandle TimerHandle_RefreshTimer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	void Interact_Implementation(APawn* InstigatorPawn) override;

	private:

	void HideAndCoolDown();

	void SetPotionState(bool bIsActive);

};
