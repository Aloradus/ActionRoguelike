// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ARCharacter.generated.h"

class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;

UCLASS()
class ACTIONROGUELIKE_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* ARCharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAMoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAMoveTurn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAYawnAndPitchInput;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(const FInputActionValue& Value);

	void MoveTurn(const FInputActionValue& Value);

	void MoveLook(const FInputActionValue& Value);

	//void AddControllerYawInput(float Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
