// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPotion.h"
#include "ARAttributeComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
AARHealthPotion::AARHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;

	BoxComp = CreateDefaultSubobject <UBoxComponent>("BoxComp");
	BoxComp->SetupAttachment(RootComponent);

	HealAmount = 50.f;
	ResetTimeInSeconds = 10.0f;

}

// Called when the game starts or when spawned
void AARHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARHealthPotion::ShowPowerup()
{
	SetPotionState(true);
}

void AARHealthPotion::HideAndCoolDown()
{
	SetPotionState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RefreshTimer,this, &AARHealthPotion::ShowPowerup, ResetTimeInSeconds);
}

void AARHealthPotion::SetPotionState(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void AARHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		UARAttributeComponent* AttributesComp = InstigatorPawn->FindComponentByClass<UARAttributeComponent>();

		if (AttributesComp)
		{
			AttributesComp->ApplyHealthChange(HealAmount);
			HideAndCoolDown();
		}
	}
}

