// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CDEnums.h"
#include "CoreMinimal.h"
#include "FireSystem.h"
#include "GameFramework/GameStateBase.h"
#include "CDGameState.generated.h"


/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerTurnStartSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerTurnEndSignature);

UCLASS(Abstract)
class CATASTROPHICDANGER_API ACDGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

#pragma region CDGameStateProperties

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CD GameState")
	EDifficulty Difficulty = EDifficulty::NONE;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CD GameState")
	EGameStates CurrentState = EGameStates::NONE;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CD GameState")
	ELevelState LevelState = ELevelState::NONE;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CD GameState")
	ETurnState TurnState = ETurnState::NONE;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CD GameState")
	int CurrentTurn = 0;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CD GameState")
	int TotalTurns = 50;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CD GameState")
	int PrepEndTurn = 25;

#pragma endregion CDGameStateProperties

	UFireSystem* FireSys;
	UWeatherController* WeatherSys;

#pragma region TurnEvents

	UPROPERTY(BlueprintAssignable)
	FPlayerTurnStartSignature OnPlayerTurnStartDelegate;

	UFUNCTION(BlueprintCallable)
	void StartPlayerTurn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Turn Events", meta = (DisplayName = "StartPlayerTurn"))
	void ReceiveStartPlayerTurn();

	UPROPERTY(BlueprintAssignable)
	FPlayerTurnStartSignature OnPlayerTurnEndDelegate;

	UFUNCTION(BlueprintCallable)
	void EndPlayerTurn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Turn Events", meta = (DisplayName = "EndPlayerTurn"))
	void ReceiveEndPlayerTurn();

#pragma endregion TurnEvents

protected:
	virtual void BeginPlay() override;

};
