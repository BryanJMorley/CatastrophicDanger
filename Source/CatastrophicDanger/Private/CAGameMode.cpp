// Fill out your copyright notice in the Description page of Project Settings.


#include "CAGameMode.h"
#include <FireSystem.h>

void ACAGameMode::BeginPlay()
{
	UFireSystem* const FireSys = (UFireSystem*)GetWorld()->GetSubsystem<UFireSystem>();
	FireSys->FireGradientMaps = FireGradientMaps;
	UWeatherController* const WeatherSys = (UWeatherController*)GetWorld()->GetSubsystem<UWeatherController>();
	WeatherSys->WindEffectTable = WindEffectTable;
	WeatherSys->BuildWindEffectMaps();

	//UE_LOG(LogFire, Display, TEXT("Gradient Maps: %s"), *(FireGradientMaps->GetName()));
	//UE_LOG(LogFire, Display, TEXT("Wind Maps: %s"), *(WindEffectTable->GetName()));

	Super::BeginPlay();
}
