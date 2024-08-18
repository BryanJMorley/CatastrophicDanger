// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CDEnums.h"
#include "WeatherController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeatherUpdateSignature);

UCLASS()
class CATASTROPHICDANGER_API UWeatherController : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int WindStrength = 2;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int WindDir = 1;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float AmbientHeat = 1;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float AmbientHeatBase = 1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UDataTable* WindEffectTable = nullptr;

	TMap<ETerrainType, TArray<float, TFixedAllocator<7>>> WindEffectMapsBase;

	TMap<ETerrainType, TArray<float, TFixedAllocator<7>>> WindEffectMaps;

	UFUNCTION(BlueprintCallable)
	void BuildWindEffectMaps();
	UFUNCTION(BlueprintCallable)
	void UpdateWindEffectMaps();


	TArray <float, TFixedAllocator<7>> WindEffect;

	const TArray<float, TFixedAllocator<7>>* GetWindEffect(ETerrainType InTerrain);
	UPROPERTY(BlueprintAssignable)
	FWeatherUpdateSignature OnWeatherUpdateDelegate;

	UFUNCTION(BlueprintCallable)
	void WeatherUpdate();

	UWeatherController();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//virtual void Deinitialize() override;
};