// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_HealSelf.h"
#include "AI/ARAIController.h"
#include "AI/ARAICharacter.h"
#include "ARAttributeComponent.h"

EBTNodeResult::Type UBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AARAIController* OwningController = Cast<AARAIController>(OwnerComp.GetAIOwner());

	if (OwningController)
	{
		APawn* MyPawn = OwningController->GetPawn();

		if (ensure(MyPawn))
		{
			UARAttributeComponent* AttributesComp = MyPawn->GetComponentByClass<UARAttributeComponent>();

			if (AttributesComp)
			{
				AttributesComp->ApplyHealthChange(MyPawn, -HealAmount);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
