// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "DeflectiveTile.h"
#include "Pawns/BallPawn.h"

const GroundableInfo ADeflectiveTile::groundableInfo = GroundableInfo(FVector(0,0,40), true);

ADeflectiveTile::ADeflectiveTile()
{
	PrimaryActorTick.bCanEverTick = true;
	FName path("/Game/Models/DeflectiveTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);
	BoxCollision->SetBoxExtent(FVector(4,36,36));

	frameCollisionsComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Frame collisions"));
	frameCollisionsComponent->AttachTo(RootComponent);

	UBoxComponent* box1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Frame top"));
	box1->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	box1->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	box1->AttachTo(frameCollisionsComponent);
	box1->SetBoxExtent(FVector(8.000000,36.000000,5.000000));
	box1->SetRelativeLocation(FVector(0,0,38));
	UBoxComponent* box2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Frame bottom"));
	box2->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	box2->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	box2->AttachTo(frameCollisionsComponent);
	box2->SetBoxExtent(FVector(8.000000,36.000000,5.000000));
	box2->SetRelativeLocation(FVector(0,0,-38));

	UBoxComponent* box3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Frame right"));
	box3->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	box3->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	box3->AttachTo(frameCollisionsComponent);
	box3->SetBoxExtent(FVector(8.000000,5.000000,40.000000));
	box3->SetRelativeLocation(FVector(0,38,0));

	UBoxComponent* box4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Frame left"));
	box4->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	box4->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	box4->AttachTo(frameCollisionsComponent);
	box4->SetBoxExtent(FVector(8.000000,5.000000,40.000000));
	box4->SetRelativeLocation(FVector(0,-38,0));

	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	TileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	BoxCollision->OnComponentHit.AddDynamic(this, &ADeflectiveTile::OnHit);

	glowPowerHighlighted = 70.0f;

	ConstructorHelpers::FObjectFinder<USoundWave> ballDeflectSoundFile( TEXT( "/Game/Sound/S_MetalHit" ) );
	ballDeflectSound = CreateDefaultSubobject<UAudioComponent>( TEXT( "Ball Deflect Sound" ) );
	ballDeflectSound->SetSound( ballDeflectSoundFile.Object );
	ballDeflectSound->bAutoActivate = false;
	ballDeflectSound->AttachTo( BoxCollision );
	AdjustMeshOrientation();
}

const GroundableInfo* ADeflectiveTile::GetGroundableInfo() const
{
	return &ADeflectiveTile::groundableInfo;
}


// Called when the game starts or when spawned
void ADeflectiveTile::BeginPlay()
{
	Super::BeginPlay();

	glowColor = FLinearColor(0.3f, 0.7f, 1.0f);
	baseColor = FLinearColor(0.72f, 3.0f, 0.74, 0.5f);

	glowColorHighlighted *= glowColor;
	baseColorHighlighted *= baseColor;
	AdjustMeshOrientation();
}

OffsetInfo ADeflectiveTile::getOffsetInfo()
{
	OffsetInfo data;
	data.offsetForCollision = FVector(0.0f, 0.0f, 40.0f);
	data.scaleForCollision = FVector(1.2f, 1.2f, 6.5f);
	data.offsetForCarryOn = FVector(0.0f, 0.0f, 50.0f);
	return data;
}

// Called every frame
void ADeflectiveTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Spin(DeltaTime);

	UpdateEdgeHighlight(DeltaTime);

}

void ADeflectiveTile::UpdateEdgeHighlight(float dt)
{
	static bool glowPowerIncreased = false;

	if (edgeHighlightDuration > 0.0f)
	{
		if (!glowPowerIncreased)
		{
			glowColorHighlighted *= glowPower;
			glowPowerIncreased = true;
		}

		HighlightEdge();
		edgeHighlightTimer += dt;

		edgeHighlightDuration = edgeHighlightTimer >= edgeHighlightDuration ? 0.0f : edgeHighlightDuration;

		if (edgeHighlightDuration == 0.0f)
		{
			edgeHighlightTimer = 0.0f;
			glowColorHighlighted /= glowPower;
			glowPowerIncreased = false;
			CancelHighlightEdge();
		}
	}
}

void ADeflectiveTile::AdjustMeshOrientation()
{
	switch (type)
	{
	case DeflectiveTileType::HORIZONTAL:
		SetActorRotation(FRotator(0.0f, currentRotation, 0.0f));
		break;
	case DeflectiveTileType::VERTICAL_NORMAL_X:
		SetActorRotation(FRotator(currentRotation, 0.0f, rotationDegreeLimit));
		break;
	case DeflectiveTileType::VERTICAL_NORMAL_Y:
		SetActorRotation(FRotator(currentRotation, rotationDegreeLimit, rotationDegreeLimit));
		break;
	default:
		break;
	}
}

void ADeflectiveTile::Spin(float dt)
{

	if (activated && accum < rotationDegreeLimit)
	{
		auto rotationPerFrame = rotationDegreeLimit / rotationDuration * dt;

		accum += rotationPerFrame;

		if (accum >= rotationDegreeLimit)
		{
			accum = 0.0f;
			currentRotation += rotationDegreeLimit;
			currentRotation = static_cast<int>(currentRotation) % 360;
			deactivate();
		}

		switch (type)
		{
			case DeflectiveTileType::HORIZONTAL:
				SetActorRotation(FRotator(0.0f, accum + currentRotation, 0.0f));
				break;
			case DeflectiveTileType::VERTICAL_NORMAL_X:
				SetActorRotation(FRotator(accum + currentRotation, 0.0f, rotationDegreeLimit));
				break;
			case DeflectiveTileType::VERTICAL_NORMAL_Y:
				SetActorRotation(FRotator(accum + currentRotation, rotationDegreeLimit, rotationDegreeLimit));
				break;
			default:
			break;
		}

		isSpining = false;
	}
	else
	{
		isSpining = true;
	}
}

void ADeflectiveTile::turnOffHighlight(bool offTile , bool offEdge )
{
	Super::turnOffHighlightEdge();
}

void ADeflectiveTile::activate()
{
	Super::activate();
	Disable();
}

void ADeflectiveTile::deactivate()
{
	Super::deactivate();
	Enable();
	Super::CancelHighlight();
}

void ADeflectiveTile::HighlightEdgeForDuration(float duration)
{
	edgeHighlightDuration = duration;
}

#if WITH_EDITOR
void ADeflectiveTile::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		auto p = PropertyChangedEvent.Property;
		auto pName = p->GetName();

		//when currentEditorPathIndex property changes in editor
		//reset current moving tile's location to desinated node's location
		if (pName.Equals(TEXT("type")))
		{
			AdjustMeshOrientation();
			transitionDistance = type == DeflectiveTileType::HORIZONTAL ? 70.0f : 130.0f;
		}
		else if(pName.Equals(TEXT("currentRotation")))
		{
			AdjustMeshOrientation();
		}
	}
}
#endif

FVector ADeflectiveTile::clampShortAxis(const FVector& vec, bool resetValueToOne)
{
	float abs_x = FMath::Abs(vec.X);
	float abs_y = FMath::Abs(vec.Y);
	float abs_z = FMath::Abs(vec.Z);

	auto abs_longest = FMath::Max3(abs_x, abs_y, abs_z);

	FVector newVec;

	if (abs_longest - abs_x <= FLT_EPSILON)
	{
		newVec = FVector(resetValueToOne ? (vec.X > .0f ? 1.0f : -1.0f) : vec.X, 0.0f, 0.0f);
	}
	else if (abs_longest - abs_y <= FLT_EPSILON)
	{
		newVec = FVector(0.0f, resetValueToOne ? (vec.Y > .0f ? 1.0f : -1.0f) : vec.Y, 0.0f);
	}
	else
	{
		newVec = FVector(0.0f, 0.0f, resetValueToOne ? (vec.Z > .0f ? 1.0f : -1.0f) : vec.Z);
	}

	return newVec;
}

void ADeflectiveTile::OnHit(class AActor* OtherActor,
							class UPrimitiveComponent* OtherComp,
							FVector NormalImpulse,
							const FHitResult& Hit)
{
	if (auto ball = Cast<ABallPawn>(OtherActor))
	{
		if (isSpining && !ball->isDying())
		{
			auto incomingSpeed = Cast<USphereComponent>(ball->RootComponent)->GetPhysicsLinearVelocity().Size();
			auto incomingVector = clampShortAxis(GetActorLocation() - ball->GetActorLocation());
			auto newDir = FMath::GetReflectionVector(incomingVector, -Hit.ImpactNormal).GetSafeNormal();
			auto newVel = (additionalDeflectingSpeed + incomingSpeed) * clampShortAxis(newDir);
			ball->ballCollision->SetPhysicsAngularVelocity(FVector::ZeroVector);
			ball->ballCollision->SetPhysicsLinearVelocity(newVel);

			auto from = GetActorLocation();
			auto to = from + clampShortAxis(newDir) * transitionDistance;

			float transitionSpeed = type == DeflectiveTileType::HORIZONTAL ? 300.0f : 800.0f;
			bool disableTransitionGravity = type != DeflectiveTileType::HORIZONTAL;
			ball->TransitionBallToProperLocationFromDeflectiveTile(to, from, newVel, transitionSpeed, disableTransitionGravity);
			HighlightEdgeForDuration(0.3f);
			ballDeflectSound->Play();
		}
		else
		{
			ball->Kill();
		}
	}

}
