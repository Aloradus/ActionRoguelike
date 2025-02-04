// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AI/ARAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "ARAttributeComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributesComp = CreateDefaultSubobject<UARAttributeComponent>("AttributesComp");
	AttributesComp->Initalize(true);
}

//Pawn sensing is deprecated. AI Perception should be used instead.
void AARAICharacter::OnPawnSeen(APawn* PawnSeen)
{
	AARAIController* AIC = Cast<AARAIController>(GetController());

	if (ensure(AIC))
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor", PawnSeen);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);

	}

}

void AARAICharacter::OnHealthChange(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Delta)
{
	
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AARAICharacter::OnPawnSeen);
	AttributesComp->OnHealthChanged.AddDynamic(this, &AARAICharacter::OnHealthChange);
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

