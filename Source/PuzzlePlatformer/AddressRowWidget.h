// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AddressRowWidget.generated.h"

class UTextBlock;
class UMainMenu;
struct FServerData;

UCLASS()
class PUZZLEPLATFORMER_API UAddressRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetText(const FServerData& ServerData);
	void Setup(UMainMenu* Parent, uint32 Index);

	UFUNCTION(BlueprintCallable)
		void OnRowPressed();

	UPROPERTY(BlueprintReadOnly)
		bool bIsSelected = false;
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* ServerNameText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* ServerOwnerText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* RoomCapacity = nullptr;

	UPROPERTY()
		UMainMenu* ParentWidget = nullptr;

	uint32 WidgetIndex = 0;
};
