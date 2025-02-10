// Fill out your copyright notice in the Description page of Project Settings.


#include "ARWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UARWorldUserWidget::InitializeWidget(FVector OwningActorWorldOffset, AActor* OwningActor)
{
	AttachedActor = OwningActor;
	WorldOffset = OwningActorWorldOffset;
}

void UARWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));

		return;
	}

	FVector2D ScreenPosition;

	if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition, false))
	{
		if (ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}


	//ABOVE METHOD ACCOUNDS FOR SCALER
	////GetOwningPlayer() is the player the screen is being drawn for.
	//if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	//{
	//	float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

	//	ScreenPosition /= Scale;

	//	if (ParentSizeBox)
	//	{
	//		ParentSizeBox->SetRenderTranslation(ScreenPosition);
	//	}
	//}

}
