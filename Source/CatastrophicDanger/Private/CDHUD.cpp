// Fill out your copyright notice in the Description page of Project Settings.


#include "CDHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ACDHUD::ACDHUD()
{
	GameInterfaceClass = nullptr;
}

void ACDHUD::ShowGameInterFace()
{
	// Make widget owned by our PlayerController
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	GameInterface = CreateWidget<UUserWidget>(PC, GameInterfaceClass);
	GameInterface->AddToViewport();
}

void ACDHUD::HideGameInterFace()
{
	if (GameInterface)
	{
		GameInterface->RemoveFromParent();
		GameInterface = nullptr;
	}
}