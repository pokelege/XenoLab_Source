// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
/// <summary>
/// The base tile
/// </summary>
class PROJECTTAP_API ATile : public AActor
{
	GENERATED_BODY()
protected:
  bool activated = false;
public:	
	UStaticMeshComponent* TileMesh;
	
	// Sets default values for this actor's properties
	ATile( const FObjectInitializer& initializer );
	
	void activate();

	void deactivate();
	
	virtual void BeginPlay();
	virtual void Tick( float DeltaTime );
};