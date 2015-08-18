// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PawnCastingTrigger.h"
#include "BallPawn.generated.h"

DECLARE_DELEGATE(BallHitDuringJumpDelegate)

UCLASS()
class PROJECTTAP_API ABallPawn : public APawn
{
	GENERATED_BODY()

	//these two vectors are used when the ball
	//transits to the center of a tile
	FVector transitionNormal;
	FVector lastAnchorPosition;

	void SpawnCastingTrigger(BallCastType type);
private:
	friend class ADeflectiveTile;
	FVector currentNormal;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
		FVector initialVelocity = FVector(0.0f, 0.0f, 0.0f);
	UAudioComponent* dieSound = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ball)
	class UUserWidget* pauseMenuInstance = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ball)
	class UClass* pauseMenuBlueprint = nullptr;

	UCurveFloat* dieSequence;

	UCurveFloat* heartBeatSequence;

	UStaticMeshComponent* ballMesh;

	USphereComponent* ballCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ball)
	class UConstrainingSpringArmComponent* spring;

	BallHitDuringJumpDelegate ballHitDuringJumpDelegate;
private:
	class APawnCastingTrigger* rampTrigger = nullptr;
	class APawnCastingTrigger* blockingTrigger = nullptr;
	class UProjectTapCameraComponent* cameraComponent = nullptr;
	UMaterialInstanceDynamic* material = nullptr;
	float currentDieTime = 0;
	float currentHeartTime = 0;
public:
	float currentTransitionSpeed = 200.0f;
	float distanceTransitioned = .0f;
	float totalTransitionDistance = .0f;
	const float DEFUALT_TRANSITION_SPEED = 200.0f;
private:
	bool dying = false;
	bool bInvincible = false;
	bool bTransitioning = false;
	bool bTransitionFinishNextFrame = false;
	bool bCanGlow = false;
	bool bDeflectiveTransition = false;
	bool bDisableGravityWhenTransition = false;
	//i.e if the axis is (0,0,1), position's Z value will be applied to ball's Z value
	void PositionBallOnAxis(const FVector& position, 
						    const FVector& axis);

public:
	// Sets default values for this actor's properties
	ABallPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void UpdateResetTransition(float dt);

	void UpdateDeflectiveTransition(float dt);


	UFUNCTION(BlueprintCallable, Category = "Ball")
	void AddVelocity(const FVector& vel, bool clearForce = true);
	void AddVelocity(const FVector& vel, const FVector& resetPos, bool clearForce = true);

	//reset ball to the center of the tile when hit
	//*This function only works well with rampbase tiles
	//@param transitionSpeed: as the name says, however, it will be reset to default speed after next transition is finished,
	//it will not change the default speed
	void TransitionBallToProperLocationFromRamp(const FVector& resetPosition,
		const FVector& newVelDir,
		float transitionSpeed = 300.0f);

	//due to physics engine problem, please set transitionSpeed less than zero 
	//when deflective tile is vertical
	void TransitionBallToProperLocationFromDeflectiveTile(const FVector& toPos,
		const FVector& fromPos,
		const FVector& vel,
		float transitionSpeed = 300.0f,
		bool disableGravityWhenTransition = false);
	void ResetBallXYPosition(const FVector& position);

	bool IsTransitioningByDeflectiveTile();
	/// <summary>
	/// Saves checkpoint variables upon contact
	/// Variables will be saved out and reloaded on level reset when spawning pawn
	/// name: Name of Checkpoint in scene
	/// position: Position of respawn for checkpoint
	/// restartSpeed: Starting velocity in the same direction the checkpoint was reached
	/// </summary>
	void SaveCheckpointData(FString name, FVector position, float restartSpeed = 0.0f);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void FellOutOfWorld(const class UDamageType & dmgType) override;
	UFUNCTION(BlueprintCallable, Category = "Ball")
		void Kill();

	bool isDying();

	void setInvincibility(bool invincible);

	UFUNCTION(BlueprintCallable, Category = "Ball")
		void SetGlowForever(bool isGlowing);

	UFUNCTION(BlueprintCallable, Category = ToggleMenu)
		void togglePauseMenu();

	UFUNCTION()
		void OnHit(class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	void setCamera(class ABallPlayerStart* playerStart, const FVector* extraPosition = nullptr);
	UProjectTapCameraComponent* GetCamera();


};
