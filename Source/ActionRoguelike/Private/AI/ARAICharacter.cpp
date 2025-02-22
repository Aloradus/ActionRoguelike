// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "ARAttributeComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BrainComponent.h"
#include "ARWorldUserWidget.h"
#include "ARActionComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributesComp = CreateDefaultSubobject<UARAttributeComponent>("AttributesComp");
	AttributesComp->Initalize(true, Cast<APawn>(this));

	ActionComp = CreateDefaultSubobject<UARActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}
	

void AARAICharacter::SetTargetActor(AActor* NewTarget)
{
	if (ensure(AIController))
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor", NewTarget);
	}
}

//Pawn sensing is deprecated. AI Perception should be used instead.
void AARAICharacter::OnPawnSeen(APawn* PawnSeen)
{
	SetTargetActor(PawnSeen);

	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
}

void AARAICharacter::OnHealthChange(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Delta)
{

	if (InstigatorActor != this)
	{
		DrawDebugString(GetWorld(), GetActorLocation(), "NEW TARGET AQUIRED FROM DAMAGE", nullptr, FColor::Red, 4.0f, true);
		SetTargetActor(InstigatorActor);
	}


	//If damages, show health bar
	if (ActiveHealthBar == nullptr)
	{
		ActiveHealthBar = CreateWidget<UARWorldUserWidget>(GetWorld(), HealthBarWidgetClass);

		if (ActiveHealthBar)
		{
			ActiveHealthBar->InitializeWidget(HealthBarWorldOffset, this);
			ActiveHealthBar->AddToViewport();
		}
	}	


	//Flash the character's material from a hit
	GetMesh()->SetScalarParameterValueOnMaterials("HitFlashTime", GetWorld()->TimeSeconds);

	if (AttributesComp->IsHealthLow())
	{
		if (ensure(AIController))
		{
			UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
			BBComp->SetValueAsBool("bLowHealth", true);
		}
	}
	else
	{
		if (ensure(AIController))
		{
			UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
			BBComp->SetValueAsBool("bLowHealth", false);
		}
	}

	if (NewHealth <= 0.f)
	{
		//Stop Behavior Tree
		if (AIController)
		{
			AIController->GetBrainComponent()->StopLogic("Killed");
		}

		//Ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		GetCharacterMovement()->DisableMovement();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//Set lifespan
		SetLifeSpan(10.0f);

	}
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AARAICharacter::OnPawnSeen);
	AttributesComp->OnHealthChanged.AddDynamic(this, &AARAICharacter::OnHealthChange);
	AIController = Cast<AARAIController>(GetController());
}

// Called when the game starts or when spawned
void AARAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AARAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

