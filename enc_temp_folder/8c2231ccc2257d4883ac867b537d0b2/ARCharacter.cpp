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
#include "Kismet/KismetMathLibrary.h"
#include "ARAttributeComponent.h"
#include "Blueprint/UserWidget.h"
#include "ARActionComponent.h"

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

	AttributesComp = CreateDefaultSubobject<UARAttributeComponent>("AttributesComp");
	AttributesComp->Initalize(false, Cast<APawn>(this));

	ActionComp = CreateDefaultSubobject<UARActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

UCameraComponent* AARCharacter::GetActiveCamera()
{
	return CameraComp;
}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributesComp->OnHealthChanged.AddDynamic(this, &AARCharacter::OnHealthChange);
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
		EnhancedInputComponent->BindAction(IATeleportMove, ETriggerEvent::Triggered, this, &AARCharacter::TeleportMove);
		EnhancedInputComponent->BindAction(IASecondaryAttack, ETriggerEvent::Triggered, this, &AARCharacter::SecondaryAttack);
		EnhancedInputComponent->BindAction(IASprint, ETriggerEvent::Triggered, this, &AARCharacter::Sprint);
	}

	//PlayerInputComponent->BindAxis("MoveForward", this, &AARCharacter::MoveForward);

	//PlayerInputComponent->BindAxis("Turn", this, &AARCharacter::AddControllerYawInput);
}

void AARCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributesComp->ApplyHealthChange(this, Amount);
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

void AARCharacter::OnHealthChange(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Delta)
{
	//Flash the players material from a hit
	GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

	if (NewHealth <= 0.f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
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

	ActionComp->StartActionByName(this, "PrimaryAttack");
	//PlayAnimMontage(AttackAnim);

	////Eventually use Animation Events (Animation Notifies)
	////Set time call attack elapsed function what time reached
	//GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AARCharacter::PrimaryAttack_TimeElapsed, 0.2f);

}

void AARCharacter::TeleportMove(const FInputActionValue& Value)
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportMove, this, &AARCharacter::TeleportMove_TimeElapsed, .02f);
}

void AARCharacter::TeleportMove_TimeElapsed()
{
	//Calc spawn rotation & location
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	FRotator SpawnRotation = GetCrossHairRotation(HandLocation);
	FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);

	//Spawn params
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	//Spawn projectile
	GetWorld()->SpawnActor<AActor>(TeleportProjectile, SpawnTM, SpawnParams);
}

void AARCharacter::MoveJump(const FInputActionValue& Value)
{
	Jump();
}

void AARCharacter::PrimaryInteract(const FInputActionValue& Value)
{
	InteractionComp->PrimaryInteract();
}

void AARCharacter::Sprint(const FInputActionValue& Value)
{
	bool bSprintPressed = Value.Get<bool>();
	
	//UE_LOG(LogTemp, Warning, TEXT("Sprint pressed Value is: %s"), bSprintPressed ? TEXT("Pressed") : TEXT("Not Pressed"));

	if (bSprintPressed)
	{
		ActionComp->StartActionByName(this, "Sprint");
	}
	else
	{
		ActionComp->StopActionByName(this, "Sprint");
	}
}


void AARCharacter::PrimaryAttack_TimeElapsed()
{
	//Calc spawn rotation & location
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	FRotator SpawnRotation = GetCrossHairRotation(HandLocation);
	FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);

	//Spawn params
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	//Spawn projectile
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void AARCharacter::SecondaryAttack(const FInputActionValue& Value)
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this, &AARCharacter::SecondaryAttack_TimeElapsed, .2f);
}

void AARCharacter::SecondaryAttack_TimeElapsed()
{
	//Calc spawn rotation & location
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	FRotator SpawnRotation = GetCrossHairRotation(HandLocation);
	FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);

	//Spawn params
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	//Spawn projectile
	GetWorld()->SpawnActor<AActor>(SecondaryProjectile, SpawnTM, SpawnParams);
}

FRotator AARCharacter::GetCrossHairRotation(FVector FromLocation)
{
	//Calculate aim location
	FVector CameraLocation = CameraComp->GetComponentLocation();
	FVector AimLocation = CameraLocation + (CameraComp->GetComponentRotation().Vector() * 5000.0f);

	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, AimLocation, ObjectQueryParams, CollisionQueryParams);

	//If an object is in the player's cross hairs, aim at impact point.
	if (bBlockingHit)
	{
		AimLocation = Hit.ImpactPoint;
	}

	FRotator ReturnRotation = UKismetMathLibrary::FindLookAtRotation(FromLocation, AimLocation);

	return ReturnRotation;
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




