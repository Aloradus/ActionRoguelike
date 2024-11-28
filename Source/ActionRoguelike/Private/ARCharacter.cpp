// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


// Sets default values
AARCharacter::AARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void AARCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ARCharacterMappingContext, 0);
		}
	}
}

//Move character forward
void AARCharacter::MoveForward(const FInputActionValue& Value)
{
	//const float wValue = Value.Get<FInputActionValue::Axis1D>();
	//UE_LOG(LogTemp, Warning, TEXT("IA MOVE TRIGGERED"));
	AddMovementInput(GetActorForwardVector(), Value.Get<FInputActionValue::Axis1D>());
}

//Move character backwards
void AARCharacter::MoveBackward(const FInputActionValue& Value)
{
	AddMovementInput(GetActorForwardVector(), Value.Get<FInputActionValue::Axis1D>());
}

void AARCharacter::AddYawnInput(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<FVector2D>().X);
}

// Called every frame
void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Bind movement
		EnhancedInputComponent->BindAction(IAMoveForward, ETriggerEvent::Triggered, this, &AARCharacter::MoveForward);
		EnhancedInputComponent->BindAction(IAMoveBackward, ETriggerEvent::Triggered, this, &AARCharacter::MoveBackward);
		EnhancedInputComponent->BindAction(IAYawnInput, ETriggerEvent::Triggered, this, &AARCharacter::AddYawnInput);

	}

	//PlayerInputComponent->BindAxis("MoveForward", this, &AARCharacter::MoveForward);

	//PlayerInputComponent->BindAxis("Turn", this, &AARCharacter::AddControllerYawInput);
}

