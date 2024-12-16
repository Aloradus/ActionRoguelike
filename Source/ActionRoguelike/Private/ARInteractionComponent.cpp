// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "ARInteractiveInterface.h"

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UARInteractionComponent::PrimaryInteract()
{
	AActor* MyOwner = GetOwner();

	FRotator EyeRotation;
	FVector EyeLocation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector StartLocation = EyeLocation;
	FVector EndLocation = EyeLocation + (EyeRotation.Vector() * 200.0f); 

	FHitResult Hit;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, StartLocation, EndLocation, ObjectQueryParams);
	FColor FDebugColor = bBlockingHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FDebugColor, false, 2.0f, 0, 2.0f);
	//UE_LOG(LogTemp, Warning, TEXT("Primary Interact Triggered"));

	//if (AActor* HitActor = Hit.GetActor())
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Hit an Actor"));


	//	if (HitActor->Implements<UARInteractiveInterface>())
	//	{
	//		APawn* MyPawn = Cast<APawn>(GetOwner());
	//		IARInteractiveInterface::Execute_Interact(HitActor, MyPawn);
	//	}
	//}


	//The above searches by a small line.
	//Below sweeps by an object type and returns an array of results.
	TArray<FHitResult> HitResults;
	FCollisionObjectQueryParams QueryParams;
	FCollisionShape Shape;
	Shape.SetSphere(30.0f);

	GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, EndLocation, FQuat::Identity, QueryParams,Shape);

	for (FHitResult HitResult : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit an Actor"));


			if (HitActor->Implements<UARInteractiveInterface>())
			{
				APawn* MyPawn = Cast<APawn>(GetOwner());
				IARInteractiveInterface::Execute_Interact(HitActor, MyPawn);
				break; // we don't want multiple objects hit and opened
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 15.0f, 16, FDebugColor, false, 2.0f, 0, 1.0f);
	}
}