// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "CheckpointSave.generated.h"


UCLASS()
class PROJECTTAP_API UCheckpointSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		FString CheckpointName;

	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		FVector Position;

	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		FVector Direction;

	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		float Speed;

	UPROPERTY(VisibleAnywhere, Category = Checkpoint)
		bool Enabled;

	UCheckpointSave(const FObjectInitializer& ObjectInitializer);
};
