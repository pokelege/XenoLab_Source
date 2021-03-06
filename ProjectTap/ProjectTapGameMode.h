// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameMode.h"
#include "CustomGameState.h"
#include "LevelManagement/LevelSaveManager.h"
#include "General/CheckpointSave.h"
#include "ProjectTapGameMode.generated.h"

/**
 *
 */
UCLASS()
class PROJECTTAP_API AProjectTapGameMode : public AGameMode
{
	GENERATED_BODY()

	FDelegateHandle OnCameraFadeInDelegateHandle;
	FDelegateHandle OnCameraFadeOutDelegateHandle;
	FDelegateHandle OnCameraFadeUpdateDelegateHandle;
	FDelegateHandle OnGameStateChangedDelegateHandle;
	FDelegateHandle OnCameraChangedDelegateHandle;
	FStreamLevelAction* levelStream = nullptr;
	class UProjectTapCameraComponent* camera = nullptr;
public:
	UPROPERTY( EditAnywhere , BlueprintReadWrite , Category = Audio )
	class UAudioComponent* musicPlayer = nullptr;
private:
	float musicTime = 0;
	float lastTime = 0;
	CustomGameState lastReportedState = CustomGameState::GAME_STATE_UNKNOWN;
	bool isMenu = false;
	bool loadingLevel = false;
	bool isGodMode = false;
public:
	AProjectTapGameMode ( const FObjectInitializer& initializer );
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void EndPlay(EEndPlayReason::Type reason);

	/// <summary>
	/// Loads checkpoint data from save file and sets values
	/// Does not work if there is no save file or
	///		the save's enabled variable is false
	/// </summary>
	UCheckpointSave* GetCheckpointData(UWorld* world);

	bool IsGodMode();

	void Respawn();
	UFUNCTION()
	bool LoadNextLevel();
	UFUNCTION()
		void OnStateChanged( const CustomGameState newState );
	UFUNCTION()
	void OnCameraFaded();
	UFUNCTION()
	void OnCameraChanged(UProjectTapCameraComponent* newCamera);
	UFUNCTION()
	void OnCameraFadeUpdate(const float percent);
	virtual void StartPlay() override;
};
