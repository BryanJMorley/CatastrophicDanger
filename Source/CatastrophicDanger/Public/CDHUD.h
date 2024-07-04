// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CDHUD.generated.h"

UCLASS()
class CATASTROPHICDANGER_API ACDHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACDHUD();

	// Make BlueprintCallable for testing
	UFUNCTION(BlueprintCallable)
	void ShowGameInterFace();

	UFUNCTION(BlueprintCallable)
	void HideGameInterFace();

	//UFUNCTION(BlueprintCallable)
	//void HideMainMenu();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> GameInterfaceClass = nullptr;

	// Keep a pointer to be able to hide it
	UPROPERTY()
	TObjectPtr<UUserWidget> GameInterface;
	
};