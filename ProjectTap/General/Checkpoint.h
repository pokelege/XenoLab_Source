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

	UPROPERTY(EditAnywhere, Category = SaveData)
		FVector Direction = FVector(1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = SaveData)
		float InitialSpeed = 0.0f;
	
	ACheckpoint();

	UFUNCTION(BlueprintCallable, Category = CPP_SaveData)
		static void ClearSave();

	UFUNCTION()
		void OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};