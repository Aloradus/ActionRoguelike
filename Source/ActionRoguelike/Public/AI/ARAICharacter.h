// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARAICharacter.generated.h"

class UPawnSensingComponent;
class UARAttributeComponent;
class AARAIController;
class UARWorldUserWidget;
class UARActionComponent;

UCLASS()
class ACTIONROGUELIKE_API AARAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARAICharacter();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, category = "UI")
	TSubclassOf<UARWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, category = "UI")
	FVector HealthBarWorldOffset;

	UPROPERTY(BlueprintReadWrite, category = "UI")
	UARWorldUserWidget* ActiveHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	UARAttributeComponent* AttributesComp;

	//Pawn sensing is deprecated. AI Perception should be used instead.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable)
	void OnPawnSeen(APawn* PawnSeen);

	UFUNCTION(BlueprintCallable)
	void OnHealthChange(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Delta);

	void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	AARAIController* AIController;

};
