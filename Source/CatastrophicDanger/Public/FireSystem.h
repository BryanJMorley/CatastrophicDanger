// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Array2D.h"
#include "HexPoint.h"
#include "TerrainStructs.h"
#include "WeatherController.h"
#include "FireSystem.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireUpdateCompleteSignature);

UCLASS()
class CATASTROPHICDANGER_API UFireSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int MapSize = 12;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool Active;

	UWeatherController* WeatherSys;

	TArray<FIntVector> ArFireBufferInner;
	TArray2D<FIntVector> ArFireBuffer = { ArFireBufferInner };
	TSet<FHexPoint> StUpdateQ;

	UFireSystem();

	UPROPERTY(BlueprintAssignable)
	FFireUpdateCompleteSignature OnFireUpdateDelegate;

	void FireUpdateComplete();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SetupMap(int Size);
	void QueueTile(FHexPoint Index);
	void CalculateFireDelta(FHexPoint Index);

	void CacheTileDelta(const TArray<int, TFixedAllocator<7>>& InDelta, const FHexPoint& Index);

	void FSH_Grassland(const FHexPoint& Index, int8 InFuel, int8 InHeat, int8 InMoisture);

private:
	virtual void Tick(float DeltaTime) override;
};