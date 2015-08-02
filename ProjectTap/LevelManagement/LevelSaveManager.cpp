// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Engine/GameInstance.h"
#include "LevelSaveManager.h"

#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::White, text);

// Sets default values
ULevelSaveManager::ULevelSaveManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

int32 ULevelSaveManager::GetLastEpisode()
{
	// load in previous data
	ULevelSaveManager* LoadGameManager = Cast<ULevelSaveManager>(UGameplayStatics::CreateSaveGameObject(ULevelSaveManager::StaticClass()));
	LoadGameManager = Cast<ULevelSaveManager>(UGameplayStatics::LoadGameFromSlot("LEVEL_DATA", 0));
	if (LoadGameManager == nullptr)
	{
		return 0;
	}
	return LoadGameManager->playerEpisode;
}

int32 ULevelSaveManager::GetLastLevel()
{
	// load in previous dataLoadGameManager = Cast<ULevelSaveManager>(UGameplayStatics::CreateSaveGameObject(ULevelSaveManager::StaticClass()));
	ULevelSaveManager* LoadGameManager = Cast<ULevelSaveManager>(UGameplayStatics::CreateSaveGameObject(ULevelSaveManager::StaticClass()));
	LoadGameManager = Cast<ULevelSaveManager>(UGameplayStatics::LoadGameFromSlot("LEVEL_DATA", 0));
	if (LoadGameManager == nullptr)
	{
		return 0;
	}
	return LoadGameManager->playerLevel;
}

int32 ULevelSaveManager::GetCurrentEpisode(FName buttonText)
{
	FString episode = "";
	FString buttonString = buttonText.ToString();

	printonscreen(buttonString);

	for (int32 i = 0; i < buttonString.Len(); i++)
	{
		FString temp = "";
		temp += buttonString[i];
		if (temp.IsNumeric())
		{
			episode += buttonString[i];
		}
	}
	if (episode.Len() > 0)
	{
		return FCString::Atoi(*episode);
	}
	return INT_MAX;
}

int32 ULevelSaveManager::GetCurrentLevel(FName buttonText)
{
	FString level = "";
	FString buttonString = buttonText.ToString();
	for (int32 i = 0; i < buttonString.Len(); i++)
	{
		FString temp = "";
		temp += buttonString[i];
		if (temp.IsNumeric())
		{
			level += buttonString[i];
		}
	}
	if (level.Len() > 0)
	{
		return FCString::Atoi(*level);
	}
	return INT_MAX;
}