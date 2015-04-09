// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MouseController.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class PROJECTTAP_API AMouseController : public APlayerController
{
	GENERATED_BODY()
public:
  AMouseController(const FObjectInitializer& initializer);
};