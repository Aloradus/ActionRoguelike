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
class UARInteractionComponent;
class UAnimMontage;
class UARAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARCharacter();

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetActiveCamera();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName HandSocketName = "Muzzle_01";

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> SecondaryProjectile;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> TeleportProjectile;

	UPROPERTY(EditAnywhere, Category = "Attack");
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_TeleportMove;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UARInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UARAttributeComponent* AttributesComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* ARCharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAMoveForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAMoveRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAYawnAndPitchInput;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* IAPrimaryAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* IASecondaryAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* IATeleportMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input");
	UInputAction* IAJump;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IAInteract;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Delta);

	void MoveForward(const FInputActionValue& Value);

	void MoveRight(const FInputActionValue& Value);

	void MoveLook(const FInputActionValue& Value);

	void PrimaryAttack(const FInputActionValue& Value);

	void PrimaryAttack_TimeElapsed();

	void SecondaryAttack(const FInputActionValue& Value);

	void SecondaryAttack_TimeElapsed();

	void TeleportMove(const FInputActionValue& Value);

	void TeleportMove_TimeElapsed();

	void MoveJump(const FInputActionValue& Value);

	void PrimaryInteract(const FInputActionValue& Value);

	FRotator GetCrossHairRotation(FVector FromLocation);

	//void AddControllerYawInput(float Value);

	virtual void PostInitializeComponents() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
