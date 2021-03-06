// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "ICarriable.h"
#include "CustomColor.h"
#include "PortalTile.generated.h"

#define ECC_Portal ECollisionChannel::ECC_EngineTraceChannel1
/**
 * blue portal faces positive X axis
 */
UCLASS()
class PROJECTTAP_API APortalTile : public ATile , public ICarriable
{
	GENERATED_BODY()
private:
	static const GroundableInfo groundableInfo;
	TScriptDelegate<FWeakObjectPtr> beginBlueOverlap;
	TScriptDelegate<FWeakObjectPtr> endBlueOverlap;
	TScriptDelegate<FWeakObjectPtr> beginOrangeOverlap;
	TScriptDelegate<FWeakObjectPtr> endOrangeOverlap;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Portal)
	FLinearColor customColor;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		APortalTile* otherPortal = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		UBoxComponent* bluePortalTrigger = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		UBoxComponent* orangePortalTrigger = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
		UAudioComponent* teleportSound = nullptr;

	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		float velocityMultiplier = 1.0f;
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Portal )
		CustomColor color = CustomColor::Tomato;

private:
	bool leftBluePortal = false;
	bool leftOrangePortal = false;
	bool enteredPortal = false;
public:
	
	APortalTile();

	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	OffsetInfo getOffsetInfo() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override;
	virtual void EditorKeyPressed(FKey Key,
		EInputEvent Event) override;
#endif


	UFUNCTION()
	void OnBlueBeginTriggerOverlap(class AActor* OtherActor,
									class UPrimitiveComponent* OtherComp,
									int32 OtherBodyIndex,
									bool bFromSweep,
									const FHitResult & SweepResult);

	UFUNCTION()
	void OnBlueEndTriggerOverlap(class AActor* OtherActor,
										 class UPrimitiveComponent* OtherComp,
										 int32 OtherBodyIndex,
										 bool bFromSweep,
										 const FHitResult & SweepResult);


	UFUNCTION()
	void OnOrangeBeginTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnOrangeEndTriggerOverlap(AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult);

	void Enable() override;

	virtual const struct GroundableInfo* GetGroundableInfo() const override;
private:
	void SetColor();
	void TransportBallToOrange(class ABallPawn* pawn);
	void TransportBallToBlue(ABallPawn* pawn);
	
	void AdjustOrientationAndTriggerBoxes();

	void SetMeshCollisionProperty( UBoxComponent* box );
	void GeneratePortalCollision();
	void ProcessBallEndfOverlap(AActor* actor);

	friend class ALaser;
	friend class AMagnetTile;
	void GetLaserPortalTransportedLocation(UPrimitiveComponent* hitPportalTrigger, FVector& newDir, FVector& newPos);
	void GetMagnetPortalTransportedLocation(UPrimitiveComponent* hitPportalTrigger, FVector& newDir, FVector& newPos);

};
