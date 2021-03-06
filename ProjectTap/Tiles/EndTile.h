// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "Tiles/ICarriable.h"
#include "EndTile.generated.h"
/**
 *
 */
UCLASS()
class PROJECTTAP_API AEndTile : public ATile, public ICarriable
{
	GENERATED_BODY()

	static const FName END_MESH;
	TScriptDelegate<FWeakObjectPtr> delegate;
	FVector targetBallLastPosition;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Level )
		FName loadLevelName = FName( "MainMenu" );
private:
	TArray<USoundBase*> sounds;
	UAudioComponent* audioPlayer = nullptr;
	UCurveFloat* soundToPlayCurve = nullptr;
	UParticleSystemComponent* particleComponent = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Transport )
		UCurveVector* ballToSocketCurve = nullptr;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Transport )
		UCurveVector* transportScalingCurve = nullptr;
private:
	class ABallPawn* targetBall = nullptr;
	float currentAnimationTime = 0;
	float ballToSocketCurveDuration = 0;
	float transportScalingCurveDuration = 0;
	bool transporting = false;
	bool CanTransport = false;
public:

	AEndTile();
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void Tick( float DeltaTime ) override;
	virtual OffsetInfo getOffsetInfo() override;
	UFUNCTION()
	virtual void OnGameStateChanged( const CustomGameState gameState ) override;

	UFUNCTION()
		void EndLevel(class ABallPawn* ball);
private:
	void StartTransporting();
	void PlayTransport( const float& DeltaTime );
};
