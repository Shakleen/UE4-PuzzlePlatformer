// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/EditableTextBox.h>
#include <Components/ScrollBox.h>
#include <UObject/ConstructorHelpers.h>

#include "AddressRowWidget.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	AddressRowClass = ConstructorHelpers::FClassFinder<UAddressRowWidget>(TEXT("/Game/MenuSystem/WBP_RowWidget")).Class;
}

void UMainMenu::OnHostButtonClicked()
{
	if (!ensure(MenuInterface != nullptr)) return;
	if (!ensure(ServerNameTextField != nullptr)) return;

	FString ServerName = ServerNameTextField->GetText().ToString();
	MenuInterface->Host(ServerName);
}

void UMainMenu::OnQuitButtonClicked()
{
	if (!ensure(MenuInterface != nullptr)) return;
	MenuInterface->Quit();
}

void UMainMenu::ShowJoinMenu()
{
	MenuSwitcher->SetActiveWidgetIndex(1);
}

void UMainMenu::ShowMainMenu()
{
	MenuSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenu::ShowHostMenu()
{
	MenuSwitcher->SetActiveWidgetIndex(2);
}

void UMainMenu::SetServerList(TArray<FServerData> ServerData)
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	if (!ensure(SessionList != nullptr)) return;
	SessionList->ClearChildren();

	uint32 index = 0;
	for (const FServerData& Data : ServerData)
	{
		if (!ensure(AddressRowClass != nullptr)) return;
		UAddressRowWidget* AddressRowWidget = CreateWidget<UAddressRowWidget>(World, AddressRowClass);

		if (!ensure(AddressRowWidget != nullptr)) return;
		AddressRowWidget->SetText(Data);
		AddressRowWidget->Setup(this, index++);

		SessionList->AddChild(AddressRowWidget);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	int i = 0;
	for (UWidget* Child : SessionList->GetAllChildren())
	{
		UAddressRowWidget* ChildRow = Cast<UAddressRowWidget>(Child);
		if (!ChildRow) continue;
		ChildRow->bIsSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i++);
	}
}

void UMainMenu::OnJoinButtonClicked()
{
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index: %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index is not set."));
	}
}
