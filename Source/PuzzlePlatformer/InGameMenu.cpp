// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "MenuInterface.h"

void UInGameMenu::OnQuitButtonPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit"));
	if (!ensure(MenuInterface != nullptr)) return;
	Teardown();
	MenuInterface->LoadMainMenu();
}

void UInGameMenu::OnCancelButtonPressed()
{
	Teardown();
}
