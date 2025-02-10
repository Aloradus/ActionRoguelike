// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARWorldUserWidget.generated.h"

class USizeBox;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget(FVector OwningActorWorldOffset, AActor* OwningActor);

	protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
