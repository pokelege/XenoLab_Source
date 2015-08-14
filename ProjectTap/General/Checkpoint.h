// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class PROJECTTAP_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		bool enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPP_Collision)
	class UBoxComponent* Collider = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CPP_Image)
	class UMaterialBillboardComponent* Billboard = nullptr;
	
	ACheckpoint();

	UFUNCTION(BlueprintCallable, Category = CPP_Visibility)
		void Disable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = CPP_Animation)
		void PlayOverlapAnimation();

	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};