// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORMER_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

	void Move(FVector& Location, float DeltaSeconds);

	void SwapStartAndTarget();
	void AddActiveTrigger();
	void RemoveActiveTrigger();

private:
	UPROPERTY(EditAnywhere, Category = "Configurations")
		float MovingSpeed = 5.f;
	UPROPERTY(EditAnywhere, Category = "Configurations", meta = (MakeEditWidget = "true"))
		FVector TargetLocation;

	UPROPERTY(EditAnywhere)
		uint8 ActiveTriggers = 1;

	FVector GlobalTargetLocation, GlobalStartingLocation;
};
