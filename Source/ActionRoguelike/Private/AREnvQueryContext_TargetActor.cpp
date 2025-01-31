// Fill out your copyright notice in the Description page of Project Settings.


#include "AREnvQueryContext_TargetActor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"


void UAREnvQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	//AActor* OwningActor = Cast<AActor>(QueryInstance.Owner);

	//if (OwningActor)
	//{
	//}
	APlayerController* PlayerController = QueryInstance.World->GetFirstPlayerController();

	if (PlayerController)
	{
		AActor* PlayerActor = Cast<AActor>(PlayerController->GetPawn());

		ContextData.ValueType = UEnvQueryItemType_Actor::StaticClass();
		ContextData.NumValues = 1;
		ContextData.RawData.SetNumUninitialized(sizeof(FWeakObjectPtr));;
		UEnvQueryItemType_Actor::SetValue(ContextData.RawData.GetData(), PlayerActor);
	}
}