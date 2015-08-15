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
	FVector lastLockPosition;
	bool lockX = false;
	bool lockY = false;
	bool lockZ = true;
public:
	UConstrainingSpringArmComponent();
	void SetTargetOffsetCustom(const FVector& newTargetOffset);
	void SetLockPosition(const FVector& lastLockPosition);
	UFUNCTION(BlueprintCallable, Category = Lock)
	void SetLockX(bool isXLock);
	UFUNCTION(BlueprintCallable, Category = Lock)
	void SetLockY(bool isYLock);
	UFUNCTION(BlueprintCallable, Category = Lock)
	void SetLockZ(bool isZLock);
protected:
	/** Updates the desired arm location, calling BlendLocations to do the actual blending if a trace is done */
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime);
};
