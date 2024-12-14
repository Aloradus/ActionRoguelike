// Fill out your copyright notice in the Description page of Project Settings.


#include "ARExplodingBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARExplodingBarrel::AARExplodingBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionProfileName("PhysicsActor");
	RootComponent = MeshComp;
	MeshComp->OnComponentHit.AddDynamic(this, &AARExplodingBarrel::OnComponentHit);


	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->Radius = 450.0f;
	RadialForceComp->bImpulseVelChange = true;
}

// Called when the game starts or when spawned
void AARExplodingBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AARExplodingBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AARExplodingBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();
}

