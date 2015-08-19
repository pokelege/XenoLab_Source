// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "Checkpoint.h"
#include "CheckpointSave.h"
#include "Pawns/BallPawn.h"


ACheckpoint::ACheckpoint()
{
	enabled = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckpointCollider"));
	Collider->SetWorldScale3D(FVector(20.0f, 20.0f, 20.0f));
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
	RootComponent = Collider;

	Billboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("CheckpointBillboard"));
	Billboard->AttachTo(RootComponent);
}

void ACheckpoint::DisplaySave_Implementation() {}

void ACheckpoint::ClearSave()
{
	UCheckpointSave* save = Cast<UCheckpointSave>(UGameplayStatics::CreateSaveGameObject(UCheckpointSave::StaticClass()));
	UGameplayStatics::SaveGameToSlot(save, save->SaveSlotName, save->UserIndex);
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
					ball->SaveCheckpointData(GetName(), Collider->GetComponentLocation(),
						Direction, InitialSpeed);
					DisplaySave();
					enabled = false;
				}
				else
					ball->Kill();
		}
	}
}