// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableTextBox;
class UScrollBox;
class UAddressRowWidget;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

public:
	FString ServerName;
	FString ServerOwner;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
};

UCLASS()
class PUZZLEPLATFORMER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void OnJoinButtonClicked();
	
	UFUNCTION(BlueprintCallable)
	void OnHostButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnQuitButtonClicked();

	UFUNCTION(BlueprintCallable)
	void ShowJoinMenu();

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void ShowHostMenu();

	void SetServerList(TArray<FServerData> ServerData);
	void SelectIndex(uint32 Index);

private:
	void UpdateChildren();

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* OpenJoinButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* OpenHostButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* JoinButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* HostButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* BackFromJoinButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* BackFromHostButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* QuitButton = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UWidgetSwitcher* MenuSwitcher = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UScrollBox* SessionList = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UEditableTextBox* ServerNameTextField = nullptr;

	TSubclassOf<UAddressRowWidget> AddressRowClass;
	TOptional<uint32> SelectedIndex;
};
