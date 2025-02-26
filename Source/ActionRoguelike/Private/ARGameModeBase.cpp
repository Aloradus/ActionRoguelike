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
#include "ARPlayerState.h"
#include "ARSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "ARInteractiveInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


//debugging console variable
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


AARGameModeBase::AARGameModeBase()
{
	PlayerStateClass = AARPlayerState::StaticClass();
	SpawnTimerInterval = 2.0f;
	SlotName = "SaveGame01";
}

void AARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	LoadSaveGame();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

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
		
		//TODO the below is ineffecient. YOu can pull the playerstate from APawn->GetPlayerState
		//Update players Credits from this kill
		Player = Cast<AARCharacter>(Killer);
		UARAttributeComponent* VictomAttributesComp = UARAttributeComponent::GetAttributesComp(VictimActor);

		if (Player && VictomAttributesComp)
		{
			AController* MyController = Player->GetController(); // Get the controller from the actor
			if (MyController)
			{
				AARPlayerState* MyPlayerState = MyController->GetPlayerState<AARPlayerState>();
				if (MyPlayerState)
				{
					MyPlayerState->AddCredits(VictomAttributesComp->KillCreditValue);
					// Use MyPlayerState
				}
			}
		}


		UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

}

void AARGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	AARPlayerState* PS = NewPlayer->GetPlayerState<AARPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
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

void AARGameModeBase::WriteSaveGame()
{

	CurrentSaveGame->SavedActors.Empty();


	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AARPlayerState* PS = Cast<AARPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
		}
	}

	//Save valid AActor Transforms
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		//only interested in gameplay actors
		if (!Actor->Implements<UARInteractiveInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();


		//Serialize example & Proper save method
		// 
		//Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		//Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;

		//Converts Actor's SaveGame UPROPERTIES into a binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);

		UE_LOG(LogTemp, Warning, TEXT("Saved position of actor %s."), *ActorData.ActorName);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
	UE_LOG(LogTemp, Warning, TEXT("Game saved."));
}

void AARGameModeBase::LoadSaveGame()
{
	//Part of the load logic for our player is handled w/ HandleStartingNewPlayer_Implementation
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame Data"));
	}
	else
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::CreateSaveGameObject(UARSaveGame::StaticClass()));

		UE_LOG(LogTemp, Warning, TEXT("Created new SaveGame Data"));
	}

	//Load saved AActor Transforms
	//This isn't efficient.. We loop through every saved actor
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		//only interested in gameplay actors
		if (!Actor->Implements<UARInteractiveInterface>())
		{
			continue;
		}

		for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
		{
			if (Actor->GetName() == ActorData.ActorName)
			{
				Actor->SetActorTransform(ActorData.Transform);


				//Serial load method from binary data
					//Pass the array to fill with data from Actor
				FMemoryReader MemReader(ActorData.ByteData);

				FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
				//Find only variables with UPROPERTY(SaveGame)
				Ar.ArIsSaveGame = true;

				//Convert binary array back into Actor's SaveGame UPROPERTIES variables 
				Actor->Serialize(Ar);

				IARInteractiveInterface::Execute_OnActorLoaded(Actor);


				UE_LOG(LogTemp, Warning, TEXT("Loading position of actor %s."), *ActorData.ActorName);
				break;
			}
		}
	}

}
