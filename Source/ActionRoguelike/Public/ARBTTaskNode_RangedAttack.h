// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARBTTaskNode_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARBTTaskNode_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	public:
	UARBTTaskNode_RangedAttack();

	protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletPitchSpread;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletYawSpread;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileBP;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
