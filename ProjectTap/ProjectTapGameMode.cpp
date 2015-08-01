// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "ProjectTapGameState.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "Controllers/MouseController.h"
#include "Pawns/BallPlayerStart.h"
#include "General/ProjectTapCameraComponent.h"
#include "General/ProjectTapCamera.h"
#include "Runtime/Engine/Public/PhysicsPublic.h"

AProjectTapGameMode::AProjectTapGameMode( const FObjectInitializer& initializer ): Super( initializer )
{
	//UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
	PlayerControllerClass = AMouseController::StaticClass();
	DefaultPawnClass = nullptr;
	GameStateClass = AProjectTapGameState::StaticClass();
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<USoundWave> defaultMusicFile( TEXT( "/Game/Sound/S_DefaultMusic" ) );
	musicPlayer = CreateDefaultSubobject<UAudioComponent>( TEXT( "Music" ) );
	musicPlayer->SetSound( defaultMusicFile.Object );
	musicPlayer->bAutoActivate = false;
	musicPlayer->AttachTo( GetRootComponent() );
}

void AProjectTapGameMode::BeginPlay()
{
	Super::BeginPlay();
	auto gameState = GetGameState<AProjectTapGameState>();
	OnCameraChangedDelegateHandle = gameState->CameraChanged.AddUFunction( this , TEXT( "OnCameraChanged" ) );
	OnGameStateChangedDelegateHandle = gameState->GameStateChanged.AddUFunction( this , TEXT( "OnStateChanged" ) );
	auto physicsWorld = GetWorld()->GetPhysicsScene();
	auto scene = physicsWorld->GetPhysXScene(0);
}


void AProjectTapGameMode::StartPlay()
{
	Super::StartPlay();
	auto gameState = GetGameState<AProjectTapGameState>();
	ABallPawn* ball = nullptr;
	if ( UWorld* world = GetWorld() )
	{
		AActor* playerStart = FindPlayerStart( 0 , FString( "Player" ) );
		FTransform playerTransform = playerStart->GetTransform();
		if ( ABallPlayerStart* realPlayerStart = Cast<ABallPlayerStart>( playerStart ) )
		{
			auto possibleCamera = realPlayerStart->camera == nullptr ? nullptr : Cast<UProjectTapCameraComponent>( realPlayerStart->camera->GetComponentByClass( UProjectTapCameraComponent::StaticClass() ) );
			FActorSpawnParameters params;
			ball = world->SpawnActor<ABallPawn>(
				ABallPawn::StaticClass() ,
				playerTransform.GetTranslation() ,
				FRotator( playerTransform.GetRotation() ) ,
				params );

			isGodMode = realPlayerStart->godMode;

			if ( ball != nullptr )
			{
				ball->AddVelocity( realPlayerStart->initialVelocity , realPlayerStart->GetActorLocation() );
				if ( possibleCamera != nullptr && realPlayerStart->followPlayer )
				{
					ball->setCamera( realPlayerStart );
					possibleCamera = ball->GetCamera();
				}
			}
			gameState->SetCamera( possibleCamera );
			isMenu = realPlayerStart->GameMode == CustomGameMode::GAME_MODE_MAIN_MENU;
			if ( realPlayerStart->music != nullptr )musicPlayer->SetSound( realPlayerStart->music );
		}
		else
		{
			FActorSpawnParameters params;
			ball = world->SpawnActor<ABallPawn>( ABallPawn::StaticClass() , playerTransform.GetTranslation() , FRotator( playerTransform.GetRotation() ) , params );
		}

		gameState->SetPlayer(ball);
	}
	musicPlayer->Play();
	musicPlayer->SetVolumeMultiplier( 0 );

	gameState->SetGameState( CustomGameState::GAME_STATE_PLAYING );
	if ( isMenu ) gameState->SetGameMode( CustomGameMode::GAME_MODE_MAIN_MENU );
}

bool AProjectTapGameMode::IsGodMode()
{
	return isGodMode;
}


void AProjectTapGameMode::BeginDestroy()
{
	auto gameState = GetGameState<AProjectTapGameState>();
	if ( gameState )
	{
		gameState->GameStateChanged.Remove( OnGameStateChangedDelegateHandle );
		OnGameStateChangedDelegateHandle.Reset();
		gameState->CameraChanged.Remove( OnGameStateChangedDelegateHandle );
		OnCameraChangedDelegateHandle.Reset();
	}
	Super::BeginDestroy();
}

void AProjectTapGameMode::Respawn()
{
	GetWorld()->GetFirstPlayerController()->ClientTravel( TEXT("?restart"), TRAVEL_MAX );
}

bool AProjectTapGameMode::LoadNextLevel()
{
	if(loadingLevel) return false;
	
	// load in previous data
	ULevelSaveManager* LoadGameManager = Cast<ULevelSaveManager>(UGameplayStatics::CreateSaveGameObject(ULevelSaveManager::StaticClass()));
	LoadGameManager = Cast<ULevelSaveManager>(UGameplayStatics::LoadGameFromSlot("LEVEL_DATA", 0));
	int32 lastEpisode = LoadGameManager->playerEpisode;
	int32 lastLevel = LoadGameManager->playerLevel;

	FString lastLevelStr = FString::FromInt(lastEpisode) + "-" + FString::FromInt(lastLevel);
	FName lastLevelName = FName(*lastLevelStr);
	UGameplayStatics::OpenLevel(GetWorld(), lastLevelName);

	// saving level progress
	FName level = GetGameState<AProjectTapGameState>()->currentLevelToLoadWhenWin;
	FString levelStr = level.ToString();
	FString* lStr = new FString;
	FString* rStr = new FString;
	levelStr.Split("-", lStr, rStr, ESearchCase::Type::IgnoreCase, ESearchDir::FromStart);

	int32 episodeNum = FCString::Atoi(**lStr);
	int32 levelNum = FCString::Atoi(**rStr);

	// only saves if the next level is after the currently saved level
	if (lastEpisode <= episodeNum && lastLevel <= levelNum)
	{
		ULevelSaveManager* SaveLevelManager = Cast<ULevelSaveManager>(UGameplayStatics::CreateSaveGameObject(ULevelSaveManager::StaticClass()));
		SaveLevelManager->playerEpisode = episodeNum;
		SaveLevelManager->playerLevel = levelNum;
		UGameplayStatics::SaveGameToSlot(SaveLevelManager, "LEVEL_DATA", 0);
	}

	// load next level
	UGameplayStatics::OpenLevel( GetWorld() , GetGameState<AProjectTapGameState>()->currentLevelToLoadWhenWin );
	return loadingLevel = true;
}
void AProjectTapGameMode::OnStateChanged(const CustomGameState newState )
{
	if(lastReportedState == newState) return;
	lastReportedState = newState;
	if(camera != nullptr)
	{
		switch(lastReportedState)
		{
			case CustomGameState::GAME_STATE_PLAYING:
				camera->FadeIn();
				break;
			case CustomGameState::GAME_STATE_GAME_OVER:
			case CustomGameState::GAME_STATE_WIN:
				camera->FadeOut();
		}
	}
}

void AProjectTapGameMode::OnCameraFaded()
{
	switch(lastReportedState)
	{
		case CustomGameState::GAME_STATE_GAME_OVER:
			Respawn();
			break;
		case CustomGameState::GAME_STATE_WIN:
			LoadNextLevel();
	}
}

void AProjectTapGameMode::OnCameraChanged(UProjectTapCameraComponent* newCamera)
{
	if(camera != nullptr)
	{
		camera->OnFadeIn.Remove( OnCameraFadeInDelegateHandle );
		OnCameraFadeInDelegateHandle.Reset();
		camera->OnFadeOut.Remove( OnCameraFadeOutDelegateHandle );
		OnCameraFadeOutDelegateHandle.Reset();
		camera->OnFadeUpdate.Remove( OnCameraFadeUpdateDelegateHandle );
		OnCameraFadeUpdateDelegateHandle.Reset();
	}
	camera = newCamera;
	if(camera != nullptr)
	{
		OnCameraFadeInDelegateHandle = camera->OnFadeIn.AddUFunction( this , TEXT( "OnCameraFaded" ) );
		OnCameraFadeOutDelegateHandle = camera->OnFadeOut.AddUFunction( this , TEXT( "OnCameraFaded" ) );
		OnCameraFadeUpdateDelegateHandle = camera->OnFadeUpdate.AddUFunction( this , TEXT( "OnCameraFadeUpdate" ) );
	}
}
void AProjectTapGameMode::OnCameraFadeUpdate(const float percent)
{
	if(musicPlayer != nullptr) musicPlayer->SetVolumeMultiplier( percent );
}
