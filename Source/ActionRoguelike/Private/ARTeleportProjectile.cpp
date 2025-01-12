// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTeleportProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AARTeleportProjectile::AARTeleportProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	InitialEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("InitialEffectComp");
	InitialEffectComp ->SetupAttachment(SphereComp);

	SecondaryEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("SecondaryEffectComp");
	SecondaryEffectComp->SetupAttachment(SphereComp);
	SecondaryEffectComp->Activate(false);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AARTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_TeleportTimer, this, &AARTeleportProjectile::TeleportPhaseOne, .3f);
}

void AARTeleportProjectile::TeleportPhaseOne()
{
	MovementComp->StopMovementImmediately();
	InitialEffectComp->Activate(false);
	SecondaryEffectComp->Activate(true);
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportTimer, this, &AARTeleportProjectile::TeleportPhaseTwo, .2f);
}

void AARTeleportProjectile::TeleportPhaseTwo()
{
	AActor* MyInstigator = GetOwner();


	if (MyInstigator)
	{
		UE_LOG(LogTemp, Warning, TEXT("MyInstigator was %s"), *MyInstigator->GetName());
		MyInstigator->TeleportTo(this->GetActorLocation(), GetInstigator()->GetActorRotation());
	}
	Destroy();
}

// Called every frame
void AARTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

