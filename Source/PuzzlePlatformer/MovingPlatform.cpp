// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartingLocation = GetActorLocation();
	GlobalTargetLocation = GetActorTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ActiveTriggers > 0)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
			float JourneyLength = (GlobalStartingLocation - GlobalTargetLocation).Size();
			float JourneyTraveled = (GlobalStartingLocation - Location).Size();

			if (JourneyTraveled > JourneyLength)
			{
				SwapStartAndTarget();
			}

			Move(Location, DeltaSeconds);
		}
	}
}

void AMovingPlatform::Move(FVector& Location, float DeltaSeconds)
{
	FVector Direction = (GlobalTargetLocation - GlobalStartingLocation).GetSafeNormal();
	FVector NewLocation = Location + MovingSpeed * DeltaSeconds * Direction;
	SetActorLocation(NewLocation);
}

void AMovingPlatform::SwapStartAndTarget()
{
	FVector TempLocation = GlobalStartingLocation;
	GlobalStartingLocation = GlobalTargetLocation;
	GlobalTargetLocation = TempLocation;
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	ActiveTriggers = ActiveTriggers > 0 ? ActiveTriggers - 1 : 0;
}
