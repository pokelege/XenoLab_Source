// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "BlockingTile.h"
#include "../Utils/LoadAssetsHelper.h"

// Sets default values
ABlockingTile::ABlockingTile( )
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FName path("/Game/Models/BlockingTile");
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path.ToString());
	TileMesh->SetStaticMesh(mesh.Object);

	BoxCollision->SetWorldScale3D(FVector(40.0f, 40.0f, 80.0f));
	//SetActorScale3D(FVector(.25f, 0.25f, .1f));
	auto m = TileMesh->GetMaterial(0);
	//TileMesh->SetMaterial(0, material);
}

// Called when the game starts or when spawned
void ABlockingTile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABlockingTile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//count time
	if (activated) {

		if (time_counter < activation_time)
		{
			time_counter += DeltaTime;
		}
		else
		{
			deactivate();
			time_counter = 0.0f;
		}
	}
}

void ABlockingTile::MaterialHighlight()
{
	material = TileMesh->CreateAndSetMaterialInstanceDynamic(0);

	if (material != nullptr)
	{
		material->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(0.2f, 0.2f, .2f));
		material->SetScalarParameterValue(TEXT("glow"), 100.0f);
		material->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.2f, 0.2f, 0.2f));
	}
}
