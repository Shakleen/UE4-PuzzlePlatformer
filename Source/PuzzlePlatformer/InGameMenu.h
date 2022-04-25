// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

class UButton;
class IMenuInterface;

UCLASS()
class PUZZLEPLATFORMER_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnQuitButtonPressed();
	UFUNCTION(BlueprintCallable)
	void OnCancelButtonPressed();
	
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* QuitButton = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* CancelButton = nullptr;
};
