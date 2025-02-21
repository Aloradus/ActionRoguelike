// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameplayFunctionLibrary.h"
#include "ARAttributeComponent.h"

bool UARGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UARAttributeComponent* AttributesComp = UARAttributeComponent::GetAttributesComp(TargetActor);

	if (AttributesComp)
	{
		return AttributesComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

//Pass HitResult as a const so it stays an input pin in blueprints
bool UARGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();

		//If we don't pass the BoneName it will check the root of the component / mesh hit
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
		//Invert vector because the hit result points to the direcion it was hit from.
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 100000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}
