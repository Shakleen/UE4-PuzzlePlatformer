// Fill out your copyright notice in the Description page of Project Settings.


#include "AddressRowWidget.h"

#include <Components/TextBlock.h>

#include "MainMenu.h"

void UAddressRowWidget::SetText(const FServerData &ServerData)
{
	if (ServerNameText)
	{
		ServerNameText->SetText(FText::FromString(ServerData.ServerName));
	}

	if (ServerOwnerText)
	{
		ServerOwnerText->SetText(FText::FromString(ServerData.ServerOwner));
	}

	if (RoomCapacity)
	{
		FString Text = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		RoomCapacity->SetText(FText::FromString(Text));
	}
}

void UAddressRowWidget::Setup(UMainMenu* Parent, uint32 Index)
{
	ParentWidget = Parent;
	WidgetIndex = Index;
}

void UAddressRowWidget::OnRowPressed()
{
	ParentWidget->SelectIndex(WidgetIndex);
}
