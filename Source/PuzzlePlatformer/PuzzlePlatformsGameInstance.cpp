// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include <Engine/Engine.h>
#include <UObject/ConstructorHelpers.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/KismetSystemLibrary.h>
#include <OnlineSessionSettings.h>

#include "PlatformTrigger.h"
#include "MainMenu.h"
#include "InGameMenu.h"

static const FName SESSION_NAME = TEXT("GameSession");
static const FName SERVER_NAME = TEXT("ServerName");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello from UPuzzlePlatformsGameInstance Constructor."));

	MenuClass = ConstructorHelpers::FClassFinder<UMainMenu>(TEXT("/Game/MenuSystem/WBP_MainMenu")).Class;
	if (!ensure(MenuClass != nullptr)) return;
	UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *MenuClass->GetName());

	InGameMenuClass = ConstructorHelpers::FClassFinder<UInGameMenu>(TEXT("/Game/MenuSystem/WBP_InGameMenu")).Class;
	if (!ensure(InGameMenuClass != nullptr)) return;
	UE_LOG(LogTemp, Warning, TEXT("Found class: %s"), *InGameMenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No subsystem found"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Found subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
	SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return;
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
		this,
		&UPuzzlePlatformsGameInstance::OnCreateSessionComplete
	);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
		this,
		&UPuzzlePlatformsGameInstance::OnDestroySessionComplete
	);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
		this,
		&UPuzzlePlatformsGameInstance::OnFindSessionComplete
	);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
		this,
		&UPuzzlePlatformsGameInstance::OnSessionJoinComplete
	);

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformsGameInstance::OnNetworkFailure);
	}

	RefreshServerList();
}

void UPuzzlePlatformsGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	MainMenu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(MainMenu != nullptr)) return;
	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host(const FString &ServerName)
{
	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		HostedServerName = ServerName;

		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSettings.Set(SERVER_NAME, HostedServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;
	if (!ensure(MainMenu != nullptr)) return;
	MainMenu->Teardown();

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->MaxSearchResults = 100;
	if (SessionSearch.IsValid())
	{
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::ShowInGameMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;
	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);

	if (!ensure(InGameMenu != nullptr)) return;
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UPuzzlePlatformsGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetworkDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Quit()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
	if (!bSucceeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s session creation failed."), *SessionName.ToString());
		return;
	}

	if (!ensure(MainMenu != nullptr)) return;
	MainMenu->Teardown();

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bSucceeded)
{
	if (bSucceeded)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool bSucceeded)
{

	if (bSucceeded && SessionSearch.IsValid())
	{
		if (SessionSearch->SearchResults.Num() > 0)
		{
			TArray<FServerData> ServerData;
			ServerData.Reserve(SessionSearch->SearchResults.Num());

			for (FOnlineSessionSearchResult SearchResult : SessionSearch->SearchResults)
			{
				FServerData Data;
				Data.ServerOwner = SearchResult.Session.OwningUserName;
				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
				
				FString ServerName;
				if (SearchResult.Session.SessionSettings.Get(SERVER_NAME, ServerName))
				{
					Data.ServerName = ServerName;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ServerName wasn't found in settings"));
				}

				ServerData.Emplace(Data);
			}

			MainMenu->SetServerList(ServerData);
		}
	}
}

void UPuzzlePlatformsGameInstance::OnSessionJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	if (!SessionInterface.IsValid()) return;

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FString Address;

	if (SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining: %s"), *Address));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't join: %s"), *SessionName.ToString());
	}
}
