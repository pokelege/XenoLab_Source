// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "JumpTile.h"
#include "Pawns/BallPawn.h"
#include "ProjectTapGameState.h"
#include "DeflectiveTile.h"

#if WITH_EDITOR
#include "UnrealEd.h"
#endif

const FName AJumpTile::JUMP_MESH_PATH = FName("/Game/Models/Jump");

AJumpTile::AJumpTile() : ABaseRampTile()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*JUMP_MESH_PATH.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	rotationDirection = Direction::Guess;
	baseColor = FLinearColor(0.500000f, 0.500000f, 0.500000f, 1.000000f);
	baseColorHighlighted = FLinearColor(0, 0, 0, 1);
	glowColor = FLinearColor(1.000000, 0, 0, 1);
	glowColorHighlighted = FLinearColor(0, 1, 0, 1);
	if (material == nullptr) material = TileMesh == nullptr ? nullptr : TileMesh->CreateDynamicMaterialInstance(0);
	SetColor();
}

void AJumpTile::SetColor()
{
	if (material != nullptr)
	{
		material->SetVectorParameterValue(TEXT("BaseColor"), baseColor);
		material->SetVectorParameterValue(TEXT("BaseColorHighlighted"), baseColorHighlighted);
		material->SetVectorParameterValue(TEXT("EdgeColor"), glowColor);
		material->SetVectorParameterValue(TEXT("EdgeColorHighlighted"), glowColorHighlighted);
	}
}

void AJumpTile::PostLoad()
{
	Super::PostLoad();
	SetColor();
}

void AJumpTile::BeginPlay()
{
	if (target != nullptr && rotationDirection == Direction::Guess)
	{
		auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		auto xdir = dir.X >= 0 ? Direction::XPlus : Direction::xMinus;
		auto ydir = dir.Y >= 0 ? Direction::YPlus : Direction::yMinus;
		rotationDirection = FMath::Abs(dir.X) >= FMath::Abs(dir.Y) ? xdir : ydir;
	}

	Super::BeginPlay();
	if (target != nullptr)
	{
		auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		auto rot = (dir.Rotation().GetNormalized().Yaw - GetActorRotation().GetNormalized().Yaw) / 360;
		material->SetScalarParameterValue(TEXT("Rotation"), 1.0f - rot);
	}
	if (material == nullptr) material = TileMesh == nullptr ? nullptr : TileMesh->CreateDynamicMaterialInstance(0);
	SetColor();
}

void AJumpTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (target != nullptr)
	{
		auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
		auto rot = (dir.Rotation().GetNormalized().Yaw - GetActorRotation().GetNormalized().Yaw) / 360;
		material->SetScalarParameterValue(TEXT("Rotation"), 1.0f - rot);
	}
}

void AJumpTile::StopWaitingForBall()
{
	isBallComing = false;
	auto world = GetWorld();
	auto state = Cast<AProjectTapGameState>(world->GetGameState());
	auto ball = state->GetPlayer();
	ball->ballHitDuringJumpDelegate.Unbind();
}

void AJumpTile::TargetStopWaitingForBall()
{
	if (target != nullptr)
	{
		target->StopWaitingForBall();
	}
}

void AJumpTile::SetWaitForBall()
{
	auto world = GetWorld();
	auto state = Cast<AProjectTapGameState>(world->GetGameState());
	auto ball = state->GetPlayer();
	ball->ballHitDuringJumpDelegate.BindUObject(this, &AJumpTile::StopWaitingForBall);
	isBallComing = true;
}

bool AJumpTile::IsWaitingForBall()
{
	return isBallComing;
}


void AJumpTile::activate()
{
	if (rotationSequence == nullptr || target == nullptr || ball == nullptr || !IsEnabled() || activated) return;
	Super::activate();
	auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	//ball->TransitionBallToProperLocationFromRamp(GetActorLocation(), dir);
	ball->SetActorLocation(GetActorLocation());
	target->SetWaitForBall();

	calculateImpulse();
	ball->ballCollision->SetPhysicsLinearVelocity(jumpVelocity);
	ball->ballCollision->SetPhysicsAngularVelocity(FVector(0.0f, 0.0f, 0.0f));
}

void AJumpTile::calculateImpulse()
{
	//h = 1/2*a*t^2
	//t = sqrt(2h/a)

	//take ball's radius into account as height difference
	auto radius = ball->ballCollision->GetScaledSphereRadius();
	auto goUpHeight = height - radius;
	float t_up = FMath::Sqrt(2 * goUpHeight / -GetWorld()->GetGravityZ());

	float fall_height = GetActorLocation().Z + height - target->GetActorLocation().Z - 3 * radius;
	float t_down = FMath::Sqrt(2 * fall_height / -GetWorld()->GetGravityZ());

	float t = t_up + t_down;

	//vf = vi + at
	//vf == 0
	//vi = -at
	float verticalVelocity = -GetWorld()->GetGravityZ() * t_up;
	auto dir = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	
	auto startPos = FVector(ball->GetActorLocation().X, ball->GetActorLocation().Y, GetActorLocation().Z);
	auto targetPos = FVector(target->GetActorLocation().X, target->GetActorLocation().Y, target->GetActorLocation().Z);
	auto distance = FVector::Dist(targetPos, startPos);

	//d = (vi + vf) / 2 * t
	//vi = 2 * (d/t)
	auto horizontalSpeed = distance / t;

	//impuls(change in momentum) = f * t
	dir.Z = 0.0f;
	auto horizontalVec = dir * horizontalSpeed;
	jumpVelocity = horizontalVec + FVector(.0f, .0f, verticalVelocity);
}

void AJumpTile::HighlightEdge()
{
	Super::HighlightEdge();
	if (target != nullptr)
	{
		target->Super::HighlightEdge();
	}
}

void AJumpTile::CancelHighlightEdge()
{
	Super::CancelHighlightEdge();
	if (target != nullptr)
	{
		target->Super::CancelHighlightEdge();
	}
}

void AJumpTile::HighlightTile()
{
	Super::HighlightTile();
	if (target != nullptr)
	{
		target->Super::HighlightTile();
		if (isBallComing)
		{
			auto newDir = GetActorLocation() - target->GetActorLocation();
			newDir.Z = .0f;
			newDir = ADeflectiveTile::clampShortAxis(newDir).GetSafeNormal();
			auto newVelDir = newDir * ballLandingForceStrength;
			ball->AddVelocity(newVelDir, true);
			ball->TransitionBallToProperLocationFromRamp(GetActorLocation(), newVelDir);

		}
	}
}

void AJumpTile::CancelHighlightTile()
{
	Super::CancelHighlightTile();
	if (target != nullptr)
	{
		target->Super::CancelHighlightTile();
	}
}

void AJumpTile::Disable()
{
	Super::Disable();
	isBallComing = false;
}


#if WITH_EDITOR
void AJumpTile::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		auto p = PropertyChangedEvent.Property;
		auto pName = p->GetNameCPP();

		//when currentEditorPathIndex property changes in editor
		//reset current moving tile's location to desinated node's location
		if (target != nullptr && pName.Equals(TEXT("height")))
		{
			target->height = height;
		}
		else if (pName.Equals("target"))
		{
			if (target != nullptr)
			{
				target->height = height;
				target->baseColor = baseColor;
				target->baseColorHighlighted = baseColorHighlighted;
				target->glowColor = glowColor;
				target->glowColorHighlighted = glowColorHighlighted;
				target->target = this;
				target->SetColor();
			}
		}
		else if (pName.Equals("baseColor") ||
			pName.Equals("baseColorHighlighted") ||
			pName.Equals("glowColor") ||
			pName.Equals("glowColorHighlighted") )
		{
			SetColor();
			if (target != nullptr)
			{
				target->baseColor = baseColor;
				target->baseColorHighlighted = baseColorHighlighted;
				target->glowColor = glowColor;
				target->glowColorHighlighted = glowColorHighlighted;
				target->SetColor();
			}
		}
	}
}

void AJumpTile::EditorKeyPressed(FKey Key, EInputEvent Event)
{
	Super::EditorKeyPressed(Key, Event);

	if (Key == EKeys::Enter && Event == EInputEvent::IE_Released)
	{
		auto itr = GEditor->GetSelectedActorIterator();
		for (; *itr != nullptr; ++itr)
		{
			bool isJump = (*itr)->IsA(AJumpTile::StaticClass());
			bool isThis = *itr == this;
			if (isJump && !isThis)
			{
				target = Cast<AJumpTile>(*itr);
				target->target = this;
			}
		}
	}
}
#endif
