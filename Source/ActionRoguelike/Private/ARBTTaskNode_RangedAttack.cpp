// Fill out your copyright notice in the Description page of Project Settings.


#include "ARBTTaskNode_RangedAttack.h"
#include "AI/ARAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ARAIController.h"
#include "ARAttributeComponent.h"

UARBTTaskNode_RangedAttack::UARBTTaskNode_RangedAttack()
{
	MaxBulletPitchSpread = 4.0f;
	MaxBulletYawSpread = 4.0f;
}

EBTNodeResult::Type UARBTTaskNode_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackboardComp))
	{
		AARAIController* MyAIController = Cast<AARAIController>(OwnerComp.GetAIOwner());
		AARAICharacter* MyCharacter = nullptr;
		AActor* TargetActor = nullptr;

		if (ensure(MyAIController))
		{
			MyCharacter = Cast<AARAICharacter>(MyAIController->GetPawn());
			TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
		}

		if (ensure(MyCharacter && TargetActor) && UARAttributeComponent::IsActorAlive(TargetActor))
		{
			FVector MuzzleLoc = MyCharacter->GetMesh()->GetSocketLocation("Muzzle_01");

			FVector Direction = TargetActor->GetActorLocation() - MuzzleLoc;
			FRotator MuzzleRotation = Direction.Rotation();

			//Add random weapon spread
			MuzzleRotation.Pitch += FMath::RandRange(0.f, MaxBulletPitchSpread);
			MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletYawSpread, MaxBulletPitchSpread);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = MyCharacter;
			SpawnParams.Instigator = MyCharacter;

			AActor* ProjectileFired = GetWorld()->SpawnActor<AActor>(ProjectileBP, MuzzleLoc, MuzzleRotation, SpawnParams);

			return ProjectileFired ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
		}

	}

	return EBTNodeResult::Failed;
}
