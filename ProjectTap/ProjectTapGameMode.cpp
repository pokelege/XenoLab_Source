// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameMode.h"
#include "Controllers/MouseController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Pawns/BallPawn.h"
#include "Pawns/BallPlayerStart.h"
AProjectTapGameMode::AProjectTapGameMode( const FObjectInitializer& initializer ): Super( initializer )
{
	UE_LOG( LogTemp , Warning , TEXT( "mouse" ) );
	PlayerControllerClass = AMouseController::StaticClass();
	DefaultPawnClass = 0;
}

void AProjectTapGameMode::BeginPlay()
{
	if (UWorld* world = GetWorld())
	{
		AActor* playerStart = FindPlayerStart(0, FString("Player"));
		FTransform playerTransform = playerStart->GetTransform();
		if(ABallPlayerStart* realPlayerStart = Cast<ABallPlayerStart>(playerStart))
		{
			FActorSpawnParameters params;
			//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
			ball = world->SpawnActor<ABallPawn>(ABallPawn::StaticClass(), playerTransform.GetTranslation(),FRotator(playerTransform.GetRotation()), params);
			ball->AddVelocity(realPlayerStart->initialVelocity);
		}
		else
		{
			FActorSpawnParameters params;
			//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
			ball = world->SpawnActor<ABallPawn>(ABallPawn::StaticClass(), playerTransform.GetTranslation(),FRotator(playerTransform.GetRotation()), params);
		}
	}
}

void AProjectTapGameMode::Respawn()
{
	if (ball != nullptr)
	{
		ball->BeginDestroy();
		ball = nullptr;
	}

	if (UWorld* world = GetWorld())
	{
		FTransform playerStart = FindPlayerStart(0, FString("Player"))->GetTransform();
		FActorSpawnParameters params;
		//AActor* spawned = world->SpawnActor(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotation(playerStart.GetRotation());
		ball = world->SpawnActor<ABallPawn>(ABallPawn::StaticClass(), playerStart.GetTranslation(),FRotator(playerStart.GetRotation()), params);
	}
}