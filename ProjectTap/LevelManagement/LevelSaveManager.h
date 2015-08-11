// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "LevelSaveManager.generated.h"

UCLASS()
class PROJECTTAP_API ULevelSaveManager : public USaveGame
{
	GENERATED_BODY()

public:	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 playerEpisode;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int32 playerLevel;

	// Sets default values for this actor's properties
	ULevelSaveManager(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	int32 GetLastEpisode();
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	int32 GetLastLevel();

	UFUNCTION(BlueprintCallable, Category = "Save Data")
	int32 GetCurrentEpisode(FName buttonText);
	UFUNCTION(BlueprintCallable, Category = "Save Data")
	int32 GetCurrentLevel(FName buttonText);\
};
