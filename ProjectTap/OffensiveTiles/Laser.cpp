// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Laser.h"
#include "ParticleEmitterInstances.h"
#include "../Pawns/BallPawn.h"
#include "../Tiles/DeflectiveTile.h"
// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	laserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
	SetRootComponent(laserParticle);

	debugArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("arrow"));
	debugArrow->AttachTo(RootComponent);


	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAssets(TEXT("/Game/Particles/P_LaserTest"));
	particleAsset = particleAssets.Object;
	laserParticle->SetTemplate(particleAsset);
}

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	laserParticle->EmitterInstances[0]->SetBeamSourcePoint(GetActorLocation(), 0);
}

void ALaser::SetLaserDepth(unsigned i)
{
	currentDepth = i;
}

// Called every frame
void ALaser::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (timer <= elapseTime)
	{
		timer += DeltaTime;
	}
	else
	{
	}

	checkLaserCollisions(DeltaTime);

}

#define printonscreen(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

void ALaser::checkLaserCollisions(float dt)
{
	FHitResult hit;
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;
	
	auto pos = GetActorLocation();
	auto rayStart = pos + GetActorForwardVector() * 10.0f;
	auto laserVector = GetActorForwardVector() * length;
	auto laserEmitter = laserParticle->EmitterInstances[0];

	GetWorld()->LineTraceSingle(hit,rayStart, pos + laserVector, queryParam, objectParam);
	

	laserEmitter->SetBeamSourcePoint(pos, 0);
	auto hitActor = hit.Actor.Get();
	if (hitActor != nullptr)
	{
		auto ball = Cast<ABallPawn>(hitActor);
		
		if (ball != nullptr)
		{
			ball->Kill();
		}
		else
		{

			laserEmitter->SetBeamTargetPoint(hit.ImpactPoint, 0);

			//if hits deflective tile then spawn a new laser object
			auto tile = Cast<ADeflectiveTile>(hitActor);
			if (CanSpawnSubLaser() && tile != nullptr)
			{
				SpawnSubLaser(hit.ImpactPoint, hit.ImpactNormal);
			}

			//if sub laser already exists then keep updating its rotation
			if (currentDepth < MAX_DEPTH && nextLaser != nullptr)
			{
				nextLaser->SetActorRotation(reflect(hit.ImpactPoint, hit.ImpactNormal).Rotation());
			}
		}
	}
	else
	{
		laserEmitter->SetBeamTargetPoint(pos + laserVector, 0);
		KillSubLaser();
	}

		laserEmitter->SetBeamTargetPoint(pos + laserVector, 0);

	static int degree = -45.0f;
	SetActorRotation(FRotator(0.0f, degree, 0.0f));
	degree += 1.0f;

	//if (hit.Actor.Get()->StaticClass() == DeflectiveTile::StaticClass())

}

void ALaser::SpawnSubLaser(const FVector& start, const FVector& normal)
{
	auto incomingVector = start - GetActorLocation();
	auto newDir = reflect(incomingVector, normal);
	nextLaser = GetWorld()->SpawnActor<ALaser>(start, newDir.Rotation());
	nextLaser->SetLaserDepth(currentDepth + 1);
}

FVector ALaser::reflect(const FVector& v1, const FVector& v2)
{
	auto n = -v2;
	auto dot = FVector::DotProduct(v1, n);
	auto n2 = n * dot;
	return (v1 - 2 * n2).GetSafeNormal();
}

bool ALaser::CanSpawnSubLaser()
{
	return currentDepth < MAX_DEPTH && nextLaser == nullptr;
}


void ALaser::KillSubLaser()
{
	if (nextLaser != nullptr)
	{
		nextLaser->KillSubLaser();
		nextLaser->Destroy();
		nextLaser = nullptr;
	}
}


