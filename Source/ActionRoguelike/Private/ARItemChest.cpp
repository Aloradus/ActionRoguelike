// Fill out your copyright notice in the Description page of Project Settings.


#include "ARItemChest.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AARItemChest::AARItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;

	bLidOpen = false;
}

void AARItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.0f , 0.0f));
}

// Called when the game starts or when spawned
void AARItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}


void AARItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpen();
}

void AARItemChest::OnRep_LidOpen()
{
	float CurrPitch = bLidOpen ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0.f, 0.f));
}
// Called every frame
void AARItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

