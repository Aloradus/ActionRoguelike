// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARTargetDummy.generated.h"

class UARAttributeComponent;
class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AARTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARTargetDummy();

protected:

UPROPERTY(VisibleAnywhere)
UARAttributeComponent* AttributeComp;

UPROPERTY(VisibleAnywhere)
UStaticMeshComponent* MeshComp;

UFUNCTION()
void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Delta);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
