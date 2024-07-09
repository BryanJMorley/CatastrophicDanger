// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CDEnums.h"
#include "GameFramework/GameModeBase.h"
#include "CAGameMode.generated.h"

/**
 * 
 */

UCLASS(Abstract)
class CATASTROPHICDANGER_API ACAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Map Properties|Fire Settings")
	UCurveTable* FireGradientMaps = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Map Properties|Fire Settings")
	UDataTable* WindEffectTable = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
