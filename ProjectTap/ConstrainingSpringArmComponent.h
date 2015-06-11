// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "ConstrainingSpringArmComponent.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API UConstrainingSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

	protected:
	FVector lockPosition;
	  /** Updates the desired arm location, calling BlendLocations to do the actual blending if a trace is done */
	  virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime);
public:

	void SetLockPosition(const FVector& lockPosition);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Constraint)
	bool lockX = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Constraint)
	bool lockY = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Constraint)
	bool lockZ = true;
};
