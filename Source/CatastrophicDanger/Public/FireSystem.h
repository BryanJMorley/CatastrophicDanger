// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainStructs.h"
#include "WeatherController.h"
#include "FireSystem.generated.h"

/**
 * 
 */

class AHexPoint;
class ACDGameState;

DECLARE_LOG_CATEGORY_EXTERN(LogFire, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireUpdateCompleteSignature);


UCLASS()
class CATASTROPHICDANGER_API UFireSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AHexMap* Map = nullptr;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int MapSize = 1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool Active = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool QueueEmpty = true;

	UPROPERTY()
	UWeatherController* WeatherSys = nullptr;

	UPROPERTY()
	UCurveTable* FireGradientMaps = nullptr;

	UPROPERTY()
	TArray<FVector3f> ArFireBuffer;

	//TQueue<int> TileUpdateQ;
	TArray<int> TileUpdateQ;

	ACDGameState* GameState = nullptr;

	//TArray<FRealCurve> CurveRows;

	UFireSystem();

#pragma region delegates

	UPROPERTY(BlueprintAssignable)
	FFireUpdateCompleteSignature OnFireUpdateDelegate; 
	void FireUpdateComplete(); //called when the fire update is complete, and data has been pushed back down.

#pragma endregion delegates 

	//init and deinit the system, mostly as per default.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

	//add a tile to the queue of tiles to be simulated.
	UFUNCTION(BlueprintCallable)
	void QueueTile(int Index);

	//Calculate the change in heat for a single tile.
	FORCEINLINE float CalculateFireDelta(const int& F, const int& H, const int& M) const;


	//store the fire delta into the buffer array to be reapplied at the end of turn.
	void CacheTileDelta(const TArray<float, TFixedAllocator<7>>& InDelta, int Index);

	UFUNCTION(BlueprintCallable)
	void ApplyFireDelta();

	UFUNCTION(BlueprintCallable)
	void FireSpreadFunction(int Index, int InFuel, int InHeat, int InMoisture);

	void IgnitionCheck(const int& Index);



	//UFUNCTION()
	//void MakeCurvesFromTable(UCurveTable* InTable);


private:
	virtual void Tick(float DeltaTime) override;
};