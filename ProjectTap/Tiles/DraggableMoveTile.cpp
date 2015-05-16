// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DraggableMoveTile.h"


ADraggableMoveTile::ADraggableMoveTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ADraggableMoveTile::BeginPlay()
{
	Super::BeginPlay();
}

void ADraggableMoveTile::Tick( float DeltaTime )
{
	UpdateDragMove(DeltaTime);
}

void ADraggableMoveTile::DragTo(const FHitResult& hit, 
								const FVector& cameraLocation,
								const FVector& camRayDirection)
{
	if (isSelected)
	{
		auto camRay = cameraLocation + camRayDirection * cameraRayLength;
		auto moveRay = camRay - anchorHitPoint;

		auto deltaLength = moveRay.SizeSquared();
		if (deltaLength > dragTolerance * dragTolerance)
		{
			auto moveDelta = moveRay.ProjectOnTo(FVector(0.0f, 1.0f, 0.0f));

			if (moveDelta.Size() > dragTolerance * dragTolerance)
			{
				newGoalPos = moveDelta + anchorHitPoint;
			}
			else
			{
				isSelected = false;
				newGoalPos = GetActorLocation();
			}
		}
		else
		{
			isSelected = false;
			newGoalPos = GetActorLocation();
		}
	}
	else
	{
		anchorHitPoint = GetActorLocation();
		cameraRayLength = (anchorHitPoint - cameraLocation).Size();
		newGoalPos = GetActorLocation();
		isSelected = true;
	}
}

void ADraggableMoveTile::UpdateDragMove(float dt)
{
	if (isSelected)
	{		
		auto moveDir = (newGoalPos - GetActorLocation()).GetSafeNormal();
		auto reachedPos = FVector::DistSquared(newGoalPos, GetActorLocation()) < 1.0f;
	
		if (reachedPos)
		{
			SetActorLocation(newGoalPos);
		}
		else if (moveDir.SizeSquared() > 0.1f)
		{
			SetActorLocation(GetActorLocation() + moveDir * dragMoveSpeed * dt);
		}
	}
}

FVector ADraggableMoveTile::calculateCurrentDir()
{
	auto startNode = path[currNode];
	//auto endNode = path[];
	return FVector();
}


void ADraggableMoveTile::RemoveFocus()
{
	isSelected = false;
}
