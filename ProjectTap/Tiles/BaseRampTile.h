// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tiles/Tile.h"
#include "Pawns/BallPawn.h"
#include "BaseRampTile.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API ABaseRampTile : public ATile
{
	GENERATED_BODY()

	static const FName BASE_RAMP_CURVE_PATH;

protected:
	FScriptDelegate pawnIn;
	FScriptDelegate pawnOut;
	float time = 0;

	friend class APawnCastingTrigger;
	ABallPawn* ball = nullptr;
	USceneComponent* offset;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	UCurveFloat* rotationSequence;

	ABaseRampTile();

	virtual void Tick( float DeltaTime ) override;

	virtual void Highlight(bool litTile = true, bool litEdge = true) override;

	virtual void turnOffHighlight() override{}

	virtual void activate() override;
};
