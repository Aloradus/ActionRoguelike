// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerState.h"
#include "UObject/CoreNet.h"
#include "Net/UnrealNetwork.h"

AARPlayerState::AARPlayerState()
{
	Credits = 0.0f;
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the float
	DOREPLIFETIME(AARPlayerState, Credits);
}

void AARPlayerState::OnRep_Credits()
{
	UE_LOG(LogTemp, Warning, TEXT("MyReplicatedFloat updated: %f"), Credits);
}

void AARPlayerState::SetCredits(float NewValue)
{
	if (GetLocalRole() == ROLE_Authority) // Only the server can change replicated variables
	{
		Credits = NewValue;
		OnRep_Credits(); // Call the RepNotify function on the server immediately.
	}
}

float AARPlayerState::GetCredits()
{
	return Credits;
}

void AARPlayerState::AddCredits(float CreditsToAdd)
{
	SetCredits(Credits + CreditsToAdd);
}

//adjust to bool so we return true if this is succesful
void AARPlayerState::RemoveCredits(float CreditsToAdd)
{
	SetCredits(Credits - CreditsToAdd);
}

void AARPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
