// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "ProjectTapGameState.h"




float AProjectTapGameState::getCameraSaturation() const
{
	return cameraSaturation;
}

void AProjectTapGameState::setCameraSaturation(float value)
{
	cameraSaturation = value;
}
void AProjectTapGameState::SetState( GameState NewState , bool notifyListeners )
{
	CurrentState = NewState;
	if ( notifyListeners ) GameStateChanged.Broadcast( CurrentState );
}

GameState AProjectTapGameState::GetState()
{
	return CurrentState;
}