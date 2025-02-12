// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "ARAttributeComponent.h"
#include "AI/ARAICharacter.h"
#include "EngineUtils.h"
#include "Curves/CurveFloat.h"
#include "ARCharacter.h"

//debugging console variable
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

void AARGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{

	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();

	if (SpawnLocations.Num() > 0)
	{
		DrawDebugSphere(GetWorld(),SpawnLocations[0], 50.f, 20, FColor::Blue, false, 30.0f);

		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}


void AARGameModeBase::SpawnBotTimerElapsed()
{
	//if this is set to false int he console for debugging return
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar CVarSpawnBots"));
		return;
	}

	//Check if were at capacity before running spawn logic

	int32 NumOfAliveBots = 0;

	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;

		UARAttributeComponent* AttributesComp = UARAttributeComponent::GetAttributesComp(Bot);

		if (AttributesComp && AttributesComp->IsAlive())
		{
			NumOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i bots alive."), NumOfAliveBots);

	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping spawn"));
		return;
	}

	//Find spawn points w/ EVQ search, at completion spawn
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARGameModeBase::OnQueryCompleted);
	}
}

void AARGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		//Detatch dead pawn, so we spawn a new one
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

AARGameModeBase::AARGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void AARGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AARCharacter* Player =Cast<AARCharacter>(VictimActor);

		//Only proceed if type cast is Player
		if (Player)
		{
			//handle is not saved but prvents it from overriting itself if another player dies at the same time

			FTimerHandle TimerHandle_RespawnDelay;
			FTimerDelegate Delegate;

			float RespawnDelay = 2.0f;
			Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
			GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
		}

		UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

}

void AARGameModeBase::KillAll()
{
	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;

		UARAttributeComponent* AttributesComp = UARAttributeComponent::GetAttributesComp(Bot);

		if (AttributesComp && AttributesComp->IsAlive())
		{
			AttributesComp->Kill(this); // @fixme: pass in player for kill credit
		}
	}
}
