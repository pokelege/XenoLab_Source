// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "CheckpointSave.h"
#include "Kismet/GameplayStatics.h"


UCheckpointSave::UCheckpointSave(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SaveSlotName = TEXT("CHECKPOINT_DATA");
	UserIndex = 0;
	Enabled = false;
}