// Fill out your copyright notice in the Description page of Project Settings.


#include "ARProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AssertionMacros.h"
#include "UObject/FastReferenceCollector.h"
#include "ARAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AARProjectileBase::AARProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 2000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}

void AARProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &AARProjectileBase::OnActorHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AARProjectileBase::OnActorOverlap);
}

void AARProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AARProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UARAttributeComponent* AttributesComp = Cast<UARAttributeComponent>(OtherActor->GetComponentByClass(UARAttributeComponent::StaticClass()));
		if (AttributesComp)
		{
			AttributesComp->ApplyHealthChange(Damage);
			Destroy();
		}
	}
}

void AARProjectileBase::Explode_Implementation()
{
	if (ImpactVFX && ImpactSoundCue)
	{
		AudioComp->Stop();
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundCue, GetActorLocation());
	}
	
	Destroy();
}

// Called when the game starts or when spawned
void AARProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(FlightSoundCue))
	{
		AudioComp->SetSound(FlightSoundCue);
		AudioComp->Play();
	}

	UGameplayStatics::SpawnEmitterAtLocation(this, LaunchVFX, GetActorLocation(), GetActorRotation());

}

// Called every frame
void AARProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AudioComp->SetWorldLocation(GetActorLocation());

}

