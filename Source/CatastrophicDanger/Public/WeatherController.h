// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeatherController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeatherUpdateSignature);

UCLASS()
class CATASTROPHICDANGER_API UWeatherController : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int WindStrength = 1;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int WindDir = 1;

	TArray<float, TFixedAllocator<7>> WindEffect;

	TArray<float, TFixedAllocator<7>>* GetWindEffect();

	UPROPERTY(BlueprintAssignable)
	FWeatherUpdateSignature OnWeatherUpdateDelegate;

	UFUNCTION()
	void WeatherUpdate();

	UWeatherController();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//virtual void Deinitialize() override;
};