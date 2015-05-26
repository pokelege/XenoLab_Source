// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Engine/GameInstance.h"
#include "BallPawn.h"
#include "ProjectTapGameState.h"
#include "General/Bullet.h"
#include "TurretPawn.h"
#include "ParticleEmitterInstances.h"
#include "../Tiles/BlockingTile.h"

const FName ATurretPawn::BASE_MESH = FName("/Game/Models/TurretBase");
const FName ATurretPawn::GUN_MESH = FName("/Game/Models/TurretGun");

// Sets default values
ATurretPawn::ATurretPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> baseMeshSource(*BASE_MESH.ToString());

	UBoxComponent* collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Turret Collision"));
	collisionBox->SetBoxExtent(FVector(1,1,3));
	this->SetRootComponent(collisionBox);
	collisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	collisionBox->bGenerateOverlapEvents = false;
	collisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	collisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	collisionBox->SetNotifyRigidBodyCollision(true);
	UStaticMeshComponent* baseMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Turret base mesh" ) );
	baseMesh->SetStaticMesh(baseMeshSource.Object);
	baseMesh->AttachTo(collisionBox);

	ConstructorHelpers::FObjectFinder<UStaticMesh> gunMesh(*GUN_MESH.ToString());
	TurretGunMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "Turret gun mesh" ) );
	TurretGunMesh->SetStaticMesh(gunMesh.Object);
	TurretGunMesh->AttachTo(baseMesh);
	FTransform transform;
	transform.SetLocation(FVector(0,0,2));
	TurretGunMesh->SetRelativeTransform(transform);

	laserTag = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Turret Laser Tag"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> laserParticle(TEXT("/Game/Particles/P_TurretLaser"));
	laserTag->SetTemplate(laserParticle.Object);
	laserTag->AttachTo(baseMesh);

	explosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Turret Explosion Tag"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> explosionParticleTemplate(TEXT("/Game/StarterContent/Particles/P_Explosion"));
	explosionParticle->SetTemplate(explosionParticleTemplate.Object);
	explosionParticle->AttachTo(TurretGunMesh);

	auto laserPrimitive = Cast<UPrimitiveComponent>(laserTag);
	FTransform laserWorldTransform = laserPrimitive->GetComponentTransform();

	auto explosionPrimitive = Cast<UPrimitiveComponent>(explosionParticle);
	FTransform explosionWorldTransform = explosionPrimitive->GetComponentTransform();

	auto pc = Cast<UPrimitiveComponent>(RootComponent);
	pc->SetWorldScale3D(FVector(40.0f, 40.0f, 40.0f));
	laserPrimitive->SetWorldScale3D(laserWorldTransform.GetScale3D());
	explosionPrimitive->SetWorldScale3D(explosionWorldTransform.GetScale3D());
}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
	nozzleLocal = TurretGunMesh->GetSocketLocation("Nozzle");
	nozzleLocalUpdatable = TurretGunMesh->GetSocketLocation("Nozzle");
	laserTag->EmitterInstances[0]->SetBeamSourcePoint(nozzleLocal, 0);
	direction = this->GetActorForwardVector();
	explosionParticle->Deactivate();
}

bool ATurretPawn::FoundPlayerToHit()
{
	FVector forward;

	forward = this->GetActorForwardVector();

	AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
	ABallPawn* player = gameState->CurrentPawn;
	if(player == nullptr) return false;
	FVector turretToBallNormal = (player->GetTransform().GetTranslation() - nozzleLocal).GetSafeNormal();
	float distance = FVector::DistSquared(player->GetActorLocation(), nozzleLocal);

	float dot = FVector::DotProduct(turretToBallNormal,forward);
	float radians = FMath::Cos(FMath::DegreesToRadians(FOV));
	if (dot < radians || distance > maxDistance * maxDistance) return false;
	FHitResult hit;
	FCollisionQueryParams queryParam;
	queryParam.bFindInitialOverlaps = false;
	queryParam.bReturnFaceIndex = true;
	FCollisionObjectQueryParams objectParam = objectParam.DefaultObjectQueryParam;

	auto pos = TurretGunMesh->GetSocketLocation("Nozzle");
	auto rayStart = pos + (player->GetActorLocation() - nozzleLocalUpdatable).GetSafeNormal();
	auto laserVector = (player->GetActorLocation() - nozzleLocalUpdatable).GetSafeNormal() * maxDistance;

	GetWorld()->LineTraceSingle(hit,rayStart, pos + laserVector, queryParam, objectParam);
	auto hitActor = hit.Actor.Get();

	return Cast<ABallPawn>(hitActor) != nullptr;
}

void ATurretPawn::Fire()
{
	AProjectTapGameState* gameState = GetWorld()->GetGameState<AProjectTapGameState>();
	ABallPawn* player = gameState->CurrentPawn;
	if(player == nullptr) return;
	FVector turretToBallNormal = (player->GetTransform().GetTranslation() - nozzleLocalUpdatable);
	float distance = turretToBallNormal.Size();
	turretToBallNormal.Normalize();

	ABullet* bullet = this->GetWorld()->SpawnActor<ABullet>(nozzleLocalUpdatable, turretToBallNormal.Rotation());

	UPrimitiveComponent* comp = Cast<UPrimitiveComponent>(bullet->GetRootComponent());
	comp->AddImpulse(turretToBallNormal * bulletForce);
}


void ATurretPawn::AttemptToFire(const float& DeltaTime)
{

	if(currentFireCooldown < fireRate ) return;

	Fire();
	currentFireCooldown = 0;
}

// Called every frame
void ATurretPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	nozzleLocalUpdatable = TurretGunMesh->GetSocketLocation("Nozzle");
	if (!activated || died) {
		laserTag->EmitterInstances[0]->SetBeamSourcePoint(nozzleLocalUpdatable, 0);
		laserTag->EmitterInstances[0]->SetBeamTargetPoint(nozzleLocalUpdatable, 0);
		return;
	}
	currentFireCooldown += DeltaTime;
	laserTag->EmitterInstances[0]->SetBeamSourcePoint(nozzleLocalUpdatable, 0);
	if(FoundPlayerToHit())
	{

		UpdateLaserTag(DeltaTime);
		AttemptToFire(DeltaTime);
	}
	else
	{
		laserTag->EmitterInstances[0]->SetBeamTargetPoint(nozzleLocalUpdatable + direction * maxDistance, 0);
	}
}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

void ATurretPawn::UpdateLaserTag(float dt)
{
		auto state = GetWorld()->GetGameState<AProjectTapGameState>();
		auto pawn = state->CurrentPawn;

		auto l = pawn->GetActorLocation();
		direction = (pawn->GetActorLocation() - TurretGunMesh->GetComponentLocation()).GetSafeNormal();

		auto gunPrimitive = Cast<UPrimitiveComponent>(TurretGunMesh);
		gunPrimitive->SetWorldRotation(direction.Rotation());
		nozzleLocalUpdatable = TurretGunMesh->GetSocketLocation("Nozzle");
		//change laser length
		FHitResult hit;
		FCollisionQueryParams queryParam;
		queryParam.bFindInitialOverlaps = false;
		queryParam.AddIgnoredActor(this);
		FCollisionObjectQueryParams objectParam;
		objectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		objectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		GetWorld()->LineTraceSingle(hit, nozzleLocalUpdatable, direction, queryParam, objectParam);

		auto laserLength = maxDistance;

		if (hit.bBlockingHit)
		{
			laserLength = (pawn->GetActorLocation() - nozzleLocalUpdatable).Size();
		}
		laserTag->EmitterInstances[0]->SetBeamSourcePoint(nozzleLocalUpdatable, 0);
		laserTag->EmitterInstances[0]->SetBeamTargetPoint(nozzleLocalUpdatable + direction * laserLength, 0);
}

void ATurretPawn::Damage(float deathDuration)
{
	if(deathDuration == 0.0f)
	{
		Kill();
	}
	else
	{
		auto damage = MAX_HEALTH / deathDuration;
		current_hp -= damage;

		if (current_hp < 0.0f)
		{
			Kill();
		}
	}

}

void ATurretPawn::Kill()
{
	if(died)return;
	died = true;
	explosionParticle->Activate(true);
	TurretGunMesh->SetStaticMesh(nullptr);
}
