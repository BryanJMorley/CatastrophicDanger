// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherController.h"
#include <FireSystem.h>
#include "CDGameState.h"

void UWeatherController::WeatherUpdate()
{
	WindStrength = FMath::Clamp(WindStrength+FMath::RandRange(-1, 1), 0, 3);
	WindDir = FMath::Abs(FMath::Modulo(WindDir + FMath::RandRange(-1, 1), 6));
	OnWeatherUpdateDelegate.Broadcast();
}

UWeatherController::UWeatherController()
{
	WindStrength = 0;
	WindDir = 1;
	WindEffect.Init(1, 7);
}

void UWeatherController::Initialize(FSubsystemCollectionBase& Collection)
{
}

TArray<float, TFixedAllocator<7>>* UWeatherController::GetWindEffect()
{
	UE_LOG(LogTemp, Warning, TEXT("GET WIND EFFECT"));
	return &WindEffect;
}