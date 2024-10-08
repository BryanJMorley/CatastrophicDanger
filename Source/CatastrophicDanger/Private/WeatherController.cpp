// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherController.h"
#include <FireSystem.h>
#include "CDGameState.h"

void UWeatherController::WeatherUpdate()
{
	/*WindStrength = FMath::Clamp(WindStrength+round(FMath::FRandRange(-1.0f, 1.0f)), 0, 3);
	WindDir = FMath::Abs(FMath::Modulo(int(WindDir + round(FMath::FRandRange(-1.0f, 1.0f))), 6));*/
	UpdateWindEffectMaps();
	OnWeatherUpdateDelegate.Broadcast();
}

UWeatherController::UWeatherController()
{
	WindStrength = 1;
	WindDir = 1;
	WindEffect.Init(1, 7);
}

void UWeatherController::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UWeatherController::BuildWindEffectMaps()
{
	UE_LOG(LogTemp, Display, TEXT("Building Wind Maps"));
	for (ETerrainType T : TEnumRange<ETerrainType>()) {
		WindEffectMapsBase.Add(T);
		WindEffectMapsBase[T].Init(0, 7);
		FWindEffectMap* WindEffectT = WindEffectTable->FindRow<FWindEffectMap>(UEnum::GetValueAsName(T), "WeatherController::BuildWindEffect", true);
		WindEffectT->AsArray(WindEffectMapsBase[T]);
	}
}
void UWeatherController::UpdateWindEffectMaps()
{
	WindEffectMaps = WindEffectMapsBase;
	for (ETerrainType T : TEnumRange<ETerrainType>()) {
		for (int i = 0; i < 6; i++) {
			int Rotated = (i + WindDir) % 6;
			WindEffectMaps[T][Rotated+1] = FMath::Lerp(1.0, WindEffectMapsBase[T][i+1], WindStrength/3.0f);// We skip the first Element so we dont change the centre.
		}
		WindEffectMaps[T][0] = FMath::Lerp(1.0, WindEffectMapsBase[T][0], WindStrength / 3.0f);// We skip the first Element so we dont change the centre.
	}

}

const TArray<float, TFixedAllocator<7>>* UWeatherController::GetWindEffect(ETerrainType InTerrain)
{
	return &WindEffectMaps[InTerrain];
}