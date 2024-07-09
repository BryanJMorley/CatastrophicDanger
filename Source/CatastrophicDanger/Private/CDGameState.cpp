// Fill out your copyright notice in the Description page of Project Settings.

#include "CDGameState.h"

#include "FireSystem.h"

void ACDGameState::StartPlayerTurn()
{
	CurrentTurn += 1;
	TurnState = ETurnState::PLAYER_TURN;
	OnPlayerTurnStartDelegate.Broadcast();
	ReceiveStartPlayerTurn();
}

void ACDGameState::EndPlayerTurn()
{
	TurnState = ETurnState::FIRE_THINKING;
	OnPlayerTurnEndDelegate.Broadcast();
	ReceiveEndPlayerTurn();
}

void ACDGameState::BeginPlay()
{
	FireSys = GetWorld()->GetSubsystem<UFireSystem>();
	FireSys->OnFireUpdateDelegate.AddDynamic(this, &ACDGameState::StartPlayerTurn);

	WeatherSys = GetWorld()->GetSubsystem<UWeatherController>();
	OnPlayerTurnStartDelegate.AddDynamic(WeatherSys, &UWeatherController::WeatherUpdate);

	Super::BeginPlay();
}
