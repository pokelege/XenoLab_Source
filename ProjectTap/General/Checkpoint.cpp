// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Checkpoint.h"
#include "Pawns/BallPawn.h"


ACheckpoint::ACheckpoint()
{
	enabled = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckpointCollider"));
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
	RootComponent = Collider;

	Billboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("CheckpointBillboard"));
	Billboard->AttachTo(RootComponent);
}

void ACheckpoint::Disable()
{
	enabled = false;
	RootComponent->SetVisibility(false, true);
}

void ACheckpoint::PlayOverlapAnimation_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("PLAYOVERLAPANIMATION NOT IMPLEMENTED - Use Native Event!"));
}

void ACheckpoint::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (enabled)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			if (auto ball = Cast<ABallPawn>(OtherActor))
				if (!ball->isDying())
				{
					ball->SaveCheckpointData(GetName(), Collider->GetComponentLocation());
					enabled = false;
					PlayOverlapAnimation();
				}
				else
					ball->Kill();
		}
	}
}