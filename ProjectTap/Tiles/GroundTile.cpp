// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectTap.h"
#include "GroundTile.h"
#include "GroundTileComponent.h"

// Sets default values
AGroundTile::AGroundTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
 	Collision = CreateDefaultSubobject<UBoxComponent>( TEXT( "Collision" ) );
	SetRootComponent( Collision );

	Collision->SetCollisionResponseToAllChannels( ECollisionResponse::ECR_Ignore );
	Collision->SetCollisionResponseToChannel( ECollisionChannel::ECC_Pawn , ECollisionResponse::ECR_Block );
	Collision->SetCollisionResponseToChannel( ECollisionChannel::ECC_WorldStatic , ECollisionResponse::ECR_Block );
	Collision->SetCollisionEnabled( ECollisionEnabled::QueryAndPhysics );
	Collision->SetCollisionObjectType( ECollisionChannel::ECC_WorldStatic );
	Collision->SetMobility( EComponentMobility::Static );
	Meshes = CreateDefaultSubobject<USceneComponent>( TEXT( "Meshes" ) );
}

void AGroundTile::PostLoad()
{
	Super::PostLoad();
	Generate(true);
}
void AGroundTile::PostActorCreated()
{
	Super::PostActorCreated();
	Generate(true);
}

void AGroundTile::Generate( bool isEditor )
{
	FVector boxExtent( ( float ) NumTilesX * MeshScaleX , ( float ) NumTilesY * MeshScaleY , MeshScaleZ );
	Collision->SetBoxExtent( boxExtent );
	TArray<USceneComponent*> children;
	Meshes->GetChildrenComponents( false , children );
	auto meshesIndex = 0;
	for ( int x = 1; x <= NumTilesX; ++x )
	{
		for ( int y = 1; y <= NumTilesY; ++y )
		{
			bool ignoreMesh = false;
			for ( int currentIgnoreMesh = 0; currentIgnoreMesh < IgnoredMeshes.Num(); ++currentIgnoreMesh )
			{
				FVector currentVector = IgnoredMeshes[currentIgnoreMesh];
				ignoreMesh = ( ( ( int ) currentVector.X ) == x && ( ( int ) currentVector.Y ) == y );
				if ( ignoreMesh )
				{
					break;
				}
			}
			if ( ignoreMesh ) continue;
			UStaticMeshComponent* newMesh = nullptr;
			if ( meshesIndex < children.Num() )
			{
				newMesh = Cast<UGroundTileComponent>( children[meshesIndex++] );
			}
			else if ( isEditor )
			{
				newMesh = NewObject<UGroundTileComponent>( this );
				newMesh->RegisterComponent();
			}
			else newMesh = CreateDefaultSubobject<UGroundTileComponent>( FName( *FString( "X" ).Append( FString::FromInt( x ) ).Append( "Y" ).Append( FString::FromInt( y ) ) ) );
			newMesh->AttachTo( Meshes );
			FVector translation( 0 );
			//todo optimize if possible
			translation.X = ( ( x - 1 ) * ( MeshScaleX * 2.0f ) ) - ( ( ( NumTilesX - 1 ) * ( MeshScaleX * 2.0f ) ) / 2.0f );
			translation.Y = ( ( y - 1 ) * ( MeshScaleY * 2.0f ) ) - ( ( ( NumTilesY - 1 ) * ( MeshScaleY * 2.0f ) ) / 2.0f );
			newMesh->SetRelativeLocation( translation );
		}
	}

	for ( int i = meshesIndex; i < children.Num();++i )
	{
		children[i]->DestroyComponent( false );
	}
}

#if WITH_EDITOR
void AGroundTile::PostEditChangeProperty( FPropertyChangedEvent & PropertyChangedEvent )
{
	//todo optimize
	Generate( true );
	Super::PostEditChangeProperty( PropertyChangedEvent );
}
#endif