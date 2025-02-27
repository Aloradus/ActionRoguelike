// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/ARAIController.h"

void UARBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Check distance between ai pawn and target actor

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));

		if (TargetActor)
		{
			AARAIController* MyController = Cast<AARAIController>(OwnerComp.GetAIOwner());

			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < 2000.f;

					bool bHasLOS = false;

					if (bWithinRange)
					{
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}

					BlackboardComp->SetValueAsBool(bWithinAttackRangeKey.SelectedKeyName, (bHasLOS && bWithinRange));
				}
			}
		}
	}
}
