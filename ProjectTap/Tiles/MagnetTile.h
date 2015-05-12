// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "MagnetTile.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AMagnetTile : public ATile
{
	GENERATED_BODY()

	class ABallPawn* FindBallPawn();
	void PullBall(class ABallPawn* ball, float DeltaTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float length = 99999.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnet)
	float targetVelocity = 1000.0f;

	AMagnetTile();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;


	virtual void ReceiveHit
	(
		class UPrimitiveComponent * MyComp,
		AActor * Other,
		class UPrimitiveComponent * OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult & Hit
	);

	virtual void deactivate() override;
};
