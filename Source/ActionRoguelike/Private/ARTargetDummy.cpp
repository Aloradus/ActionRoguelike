// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTargetDummy.h"
#include "Components/StaticMeshComponent.h"
#include "ARAttributeComponent.h"

// Sets default values
AARTargetDummy::AARTargetDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>("AttributeComp");
}


void AARTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &AARTargetDummy::OnHealthChanged);
}

void AARTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("ARTarget Hit"));
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}

}

// Called when the game starts or when spawned
void AARTargetDummy::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AARTargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

