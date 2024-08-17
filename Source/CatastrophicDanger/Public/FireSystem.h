// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainStructs.h"
#include "WeatherController.h"
#include "PriorityQueue.h"
#include "FireSystem.generated.h"

/**
 * 
 */

class AHexPoint;
class ACDGameState;

DECLARE_LOG_CATEGORY_EXTERN(LogFire, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireUpdateCompleteSignature);

USTRUCT()
struct FQTile {
	GENERATED_BODY()
public:
	int Index;
	bool Reverse;

	FQTile() : FQTile(0, false)
	{}

	FQTile(int Index, bool Reverse): Index(Index), Reverse(Reverse)
	{}

	FQTile(const FQTile& Other) : FQTile(Other.Index, Other.Reverse)
	{}

	FORCEINLINE uint32 GetTypeHash(const FQTile& Thing)
	{
		uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FQTile));
		return Hash;
	}

	bool operator==(const FQTile Other) const
	{
		return Index == Other.Index;
	}


};


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
	bool DivideHeatAcrossTiles = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float FireHeatScale = 1;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int ProcessCount = 3;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int ProcessCountBase = 3;

	UPROPERTY()
	UWeatherController* WeatherSys = nullptr;

	UPROPERTY()
	UCurveTable* FireGradientMaps = nullptr;

	UPROPERTY()
	TArray<FVector3f> ArFireBuffer;

	//<int> TileUpdateQ;
	TPriorityQueue<FQTile> TileUpdateQ;

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
	void QueueTile(int Index, int Priority = 99, bool reverse = false);

	UFUNCTION(BlueprintCallable)
	void SetBatchSize(int BatchSize);

	//Calculate the change in heat for a single tile.
	FORCEINLINE float CalculateFireDelta(const float& F, const float& H, const float& M) const;


	//store the fire delta into the buffer array to be reapplied at the end of turn.
	void CacheTileDelta(const TArray<float, TFixedAllocator<7>>& InDelta, int Index);

	UFUNCTION(BlueprintCallable)
	void ApplyFireDelta();

	UFUNCTION(BlueprintCallable)
	void FireSpreadFunction(int Index, const float& F, const float& H, const float& M, bool Reverse = false);

	void IgnitionCheck(const int& Index);

	//UFUNCTION()
	//void MakeCurvesFromTable(UCurveTable* InTable);

	UFUNCTION(BlueprintCallable)
	bool RemoveFromQueue(int Index);

private:
	virtual void Tick(float DeltaTime) override;
};