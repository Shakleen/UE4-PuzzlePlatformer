// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include <TimerManager.h>

#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++PlayersPresent;

	if (PlayersPresent == PlayerThreshold)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10.f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--PlayersPresent;
}

void ALobbyGameMode::StartGame()
{
	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (!ensure(GameInstance != nullptr)) return;
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Game?listen");
}
