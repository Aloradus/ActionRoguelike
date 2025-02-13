// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCoinStash.h"
#include "ARPlayerState.h"

AARCoinStash::AARCoinStash()
{
	CreditValue = 50.0f;
	ResetTimeInSeconds = 30.0f;
}

void AARCoinStash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	FRotator RotationChange(0.0f, 100.0f * DeltaTime, 0.0f); // Rotate 100 degrees per second around Z axis
	FRotator NewRotation = CurrentRotation + RotationChange;

	SetActorRotation(NewRotation);
}

void AARCoinStash::ShowCoins()
{
	SetCoinState(true);
}

void AARCoinStash::HideAndCoolDown()
{
	SetCoinState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RefreshTimer, this, &AARCoinStash::ShowCoins, ResetTimeInSeconds);
}

void AARCoinStash::SetCoinState(bool bIsActive)
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void AARCoinStash::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		AController* PlayerController = InstigatorPawn->GetController();

		if (PlayerController)
		{
			AARPlayerState* PlayerState = PlayerController->GetPlayerState<AARPlayerState>();
			if (PlayerState)
			{
				PlayerState->AddCredits(CreditValue);
			}
			HideAndCoolDown();
		}
	}
}
