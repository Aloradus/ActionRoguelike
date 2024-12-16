// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "ARInteractionComponent.h"
#include "Animation/AnimMontage.h"

// Sets default values
AARCharacter::AARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UARInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
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
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//AddMovementInput(GetActorForwardVector(), Value.Get<FInputActionValue::Axis1D>());
	AddMovementInput(ControlRot.Vector(), Value.Get<FInputActionValue::Axis1D>());
}

void AARCharacter::MoveRight(const FInputActionValue& Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// x is always forward (Red)
	// y is right (Green)
	// z is up (Blue)
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value.Get<FInputActionValue::Axis1D>());
}


void AARCharacter::MoveLook(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<FVector2D>().Y);
	AddControllerYawInput(Value.Get<FVector2D>().X);
}

void AARCharacter::PrimaryAttack(const FInputActionValue& Value)
{
	PlayAnimMontage(AttackAnim);

	//Eventually use Animation Events
	//Set time call attack elapsed function what time reached
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AARCharacter::PrimaryAttack_TimeElapsed, 0.2f);

}

void AARCharacter::MoveJump(const FInputActionValue& Value)
{
	Jump();
}

void AARCharacter::PrimaryInteract(const FInputActionValue& Value)
{
	InteractionComp->PrimaryInteract();
}

void AARCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

// Called every frame
void AARCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/** Rotation Visuals **/
	const float drawScale = 100.0f;
	const float thickness = 5.0f;

	FVector lineStart = GetActorLocation();
	lineStart += GetActorRightVector() * 100.0f;
	FVector actorDirectionLineEnd = lineStart + (GetActorForwardVector() * 100.0f);
	DrawDebugDirectionalArrow(GetWorld(), lineStart, actorDirectionLineEnd, drawScale, FColor::Yellow, false, 0.0f, 0, thickness);

	FVector controllerDirectionLineEnd = lineStart + (GetControlRotation().Vector() * 100.0f);
	controllerDirectionLineEnd.Z = lineStart.Z;

	DrawDebugDirectionalArrow(GetWorld(), lineStart, controllerDirectionLineEnd, drawScale, FColor::Green, false, 0.0f, 0, thickness);
}


// Called to bind functionality to input
void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Bind movements
		EnhancedInputComponent->BindAction(IAMoveForward, ETriggerEvent::Triggered, this, &AARCharacter::MoveForward);
		EnhancedInputComponent->BindAction(IAMoveRight, ETriggerEvent::Triggered, this, &AARCharacter::MoveRight);
		EnhancedInputComponent->BindAction(IAYawnAndPitchInput, ETriggerEvent::Triggered, this, &AARCharacter::MoveLook);
		EnhancedInputComponent->BindAction(IAPrimaryAttack, ETriggerEvent::Triggered, this, &AARCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Triggered, this, &AARCharacter::MoveJump);
		EnhancedInputComponent->BindAction(IAInteract, ETriggerEvent::Triggered, this, &AARCharacter::PrimaryInteract);
	}

	//PlayerInputComponent->BindAxis("MoveForward", this, &AARCharacter::MoveForward);

	//PlayerInputComponent->BindAxis("Turn", this, &AARCharacter::AddControllerYawInput);
}

