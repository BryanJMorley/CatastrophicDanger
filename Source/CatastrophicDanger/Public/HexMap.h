// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Array2D.h"
#include "CoreMinimal.h"
#include "FastNoiseWrapper.h"
#include "GameFramework/Actor.h"
#include "TerrainStructs.h"
#include <Delegates/DelegateCombinations.h>
#include "CDGameState.h"
#include "HexMap.generated.h"


class AHexTile;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTerrainUpdateSignature, bool, DoTransform, bool, MatSetupUpdate);

DECLARE_MULTICAST_DELEGATE_OneParam(FMapSetupCompleteSignature, EMapProgress);

#pragma region ClassBody
UCLASS(Blueprintable, Placeable)
class CATASTROPHICDANGER_API AHexMap : public AActor
{
	GENERATED_BODY()

#pragma region Properties

public:	
	AHexMap();
#pragma region MapVariables

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties")
	int MapSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties") 
	float TileSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties")
	float ElevationHeightSeperation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Map Properties")
	TSubclassOf<AHexTile> TileClass = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Map Properties|Fire Settings")
	UCurveTable* FireGradientMaps = nullptr;
	
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Map Properties", meta = (Bitmask, BitmaskEnum = "/Script/CatastrophicDanger.EMapProgress"))
	int32 MapSetupState = 0;

#pragma endregion MapVariables

#pragma region DataArrays
	//Arrays to hold all the data. Structs are made to reference to it weakly, but it all stays here, which makes certain 
	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Fuel"))
	TArray<float> ArFuel;
	TArray<float> ArStartFuel;

	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Heat"))
	TArray<float> ArHeat;
	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Moisture"))
	TArray<float> ArMoisture;
	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Tile Update"))
	TArray<bool> ArQueued;
	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Elevation"))
	TArray<int> ArElevation;
	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Terrain Type"))
	TArray<ETerrainType> ArTerrainType;
	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Fire State"))
	TArray<EFireState> ArFireState;
	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Gradient"))
	TArray<FGradientMap> ArGradient;
	UPROPERTY(BlueprintReadWrite, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Tiles"))
	TArray<AHexTile*> ArTiles;

#pragma endregion DataArrays

#pragma region Delegates

	UPROPERTY(BlueprintAssignable)
	FTerrainUpdateSignature OnTerrainUpdateDelegate;

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	void TriggerTerrainUpdate(bool DoMovement = false, bool MapSetupTrigger = false);

	FMapSetupCompleteSignature OnMapSetupCompleteDelegate;

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	void TriggerMapSetupComplete(EMapProgress Progress);


#pragma endregion Delegates

#pragma endregion Properties


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	void InitialiseGrids();

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	void FillFhmFromTerrainTable(UDataTable* InTable);

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	void SpawnGrid(bool TriggerUpdate = true);

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	void DestroyGrid();

	UFUNCTION(BlueprintCallable, Category = "Terrain Gen", meta = (AutoCreateRefTerm = "InTileData"))
	AHexTile* SpawnTile(FVector pos, FHexPoint Index);

	UFUNCTION(BlueprintCallable, Category = "Terrain Gen")
	void CalculateGradient();

	UFUNCTION(BlueprintCallable, Category = "Fire Sim")
	void QueueBurningTiles();

	//setup the FireSystem with the info we need that we need for doing the actual simulation
	UFUNCTION(BlueprintCallable)
	void SetupFireSystem(UFireSystem* FireSys);
	
#pragma region NoiseFunctions
	
	UFUNCTION(BlueprintCallable, Category = "TerrainGen")
	FVector2f Noise2DToFloatArray(UFastNoiseWrapper* Noise, UPARAM(ref) TArray<float>& TargetArray, bool accuratePos = false, FVector2D range = FVector2D(-1,1));
	
	UFUNCTION(BlueprintCallable, Category = "TerrainGen")
	FIntPoint Noise2DToIntArray(UFastNoiseWrapper* Noise, UPARAM(ref) TArray<int>& TargetArray, bool accuratePos, FVector2D range = FVector2D(-1, 1));

	//TODO: ACCURATE REMAP NOISE FUNCTION HERE

#pragma endregion NoiseFunctions

#pragma region GetData
	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FTileData GetTileData(const FHexPoint& Index);

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FTerrainData GetTerrainData(const FHexPoint& Index);

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FFireData GetFireData(const FHexPoint& Index);

	//UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	//bool SetTileData(const FHexPoint& Index, const FTileData& data);

	//UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	//FTileRef MakeTileRef(FHexPoint Index);

	//FTileRef MakeTileRef(int Index);
#pragma endregion GetData


#pragma region Utilities
	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	FORCEINLINE int HexToIndex(const FHexPoint& Index) const {
		FIntPoint coord = Index.ToOffset();
		return coord.X + coord.Y * MapSize;
	}

	FORCEINLINE bool HexInBounds(const FHexPoint& Index) const {
		FIntPoint coord = Index.ToOffset();
		return (coord.X >= 0 && coord.X < MapSize && coord.Y >= 0 && coord.Y < MapSize);
	}

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	FORCEINLINE FHexPoint IndexToHex(const int& Index) const {
		FHexPoint coord = { (Index%MapSize), (Index / MapSize) };
		return coord;
	}

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	FORCEINLINE FIntPoint IndexToCoord(const int& Index) const {
		return { (Index % MapSize), (Index / MapSize) };
	}

	FORCEINLINE FVector IndexToCoord3(const int& Index) const {
		return { double(Index % MapSize), double(Index / MapSize), 0.0 };
	}

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	ETerrainType TerrainTypeSelector(const float& SoilQual, const float& Temp, FVector2f RangeSoil, FVector2f RangeTemp) const;

	UFUNCTION(BlueprintCallable, Category = "Fire Sim")
	void ClearUpdateState(bool Invert = false);

	UFUNCTION(BlueprintCallable, Category = "Fire Sim")
	void HeatLowerToAmbient(float percent);

#pragma endregion Utilities

};