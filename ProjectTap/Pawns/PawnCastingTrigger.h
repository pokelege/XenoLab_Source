// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PawnCastingTrigger.generated.h"

UCLASS()
class PROJECTTAP_API APawnCastingTrigger : public AActor
{
	GENERATED_BODY()

	friend class APortalTile;
	class ABallPawn* currentPawn = nullptr;

private:
	friend class ABallPawn;
	void SetBallPawn(ABallPawn* currentPawn);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
		UBoxComponent* tileOverlapCollision;

	// Sets default values for this actor's properties
	APawnCastingTrigger();

	UFUNCTION()
		void OnBeginTriggerOverlap(AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndTriggerOverlap(AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

};
