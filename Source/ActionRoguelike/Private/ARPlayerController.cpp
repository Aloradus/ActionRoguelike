// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AARPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		UGameplayStatics::SetGamePaused(this, false);
		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		//High Z order so infront of everything.
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		UGameplayStatics::SetGamePaused(this, true);
	}
}

void AARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}
