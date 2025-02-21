// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAction_ProjectileAttack.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

UARAction_ProjectileAttack::UARAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = .2f;

}


void UARAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);


	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		//Do note derive from AActor so cannot use GetWorldTimerManager
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}



}

void UARAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (!InstigatorCharacter)
	{
		return;

	}
	//SpawnLocation
	FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

	//Calc Projectile's Spawn Rotation
	APlayerController* PC = Cast<APlayerController>(InstigatorCharacter->GetController());

	if (PC)
	{
		FVector TraceStart = PC->PlayerCameraManager->GetCameraLocation();
		FVector AimLocation = TraceStart + (PC->PlayerCameraManager->GetCameraRotation().Vector() * 5000.0f);

		FHitResult Hit;
		FCollisionObjectQueryParams ObjectQueryParams;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(InstigatorCharacter);

		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, AimLocation, ObjectQueryParams, CollisionQueryParams);

		//If an object is in the player's cross hairs, aim at impact point.
		if (bBlockingHit)
		{
			AimLocation = Hit.ImpactPoint;
		}

		FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, AimLocation);


		FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);

		//Spawn params
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = InstigatorCharacter;
		SpawnParams.Instigator = InstigatorCharacter;

		//Spawn projectile
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}

