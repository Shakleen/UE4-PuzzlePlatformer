// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include <OnlineSubsystem.h>
#include <Interfaces/OnlineSessionInterface.h>
#include "PuzzlePlatformsGameInstance.generated.h"

class UMainMenu;
class UInGameMenu;
class FOnlineSessionSearch;

UCLASS()
class PUZZLEPLATFORMER_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(Exec)
	void Host(const FString &ServerName) override;


	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	UFUNCTION(BlueprintCallable)
	void ShowInGameMenu();

	void StartSession();

	void OnNetworkFailure(UWorld* World, UNetDriver* NetworkDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

private:
	void RefreshServerList();
	virtual void LoadMainMenu() override;
	virtual void Quit() override;
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bSucceeded);
	void OnDestroySessionComplete(FName SessionName, bool bSucceeded);
	void OnFindSessionComplete(bool bSucceeded);
	void OnSessionJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);

private:
	TSubclassOf<UMainMenu> MenuClass;
	UMainMenu* MainMenu = nullptr;

	TSubclassOf<UInGameMenu> InGameMenuClass;
	UInGameMenu* InGameMenu = nullptr;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FString HostedServerName;
};
