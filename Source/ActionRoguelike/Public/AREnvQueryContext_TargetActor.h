// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "AREnvQueryContext_TargetActor.generated.h"

/**
 * Custom Context Query that returns Player 0's Actor for a query searches center
 */
UCLASS()
class ACTIONROGUELIKE_API UAREnvQueryContext_TargetActor : public UEnvQueryContext
{
	GENERATED_BODY()
	
	public: 
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
