// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Array2D.h"
#include "CoreMinimal.h"
#include "FastNoiseWrapper.h"
#include "GameFramework/Actor.h"
#include "TerrainStructs.h"
#include <Delegates/DelegateCombinations.h>
#include "HexMap.generated.h"


class AHexTile;

DECLARE_MULTICAST_DELEGATE_OneParam(FTerrainUpdateDelegate, bool);


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
	int MapWidth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties") 
	int MapHeight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties") 
	float TileSize;
#pragma endregion MapVariables

#pragma region MapMeta	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Map Properties")

	TSubclassOf<AHexTile> TileClass = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Map Properties", meta = (Bitmask, BitmaskEnum = EMapProgress))
	int32 MapSetupState = 0;

#pragma endregion MapMeta	

#pragma region Delegates

	FTerrainUpdateDelegate OnTerrainUpdate;

#pragma endregion Delegates	

#pragma region DataArrays
	//Arrays to hold all the data. Structs are made to reference to it weakly, but it all stays here, which makes certain 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Fuel"))
	TArray<float> ArInnerFuel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Heat"))
	TArray<float> ArInnerHeat;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Moisture"))
	TArray<float> ArInnerMoisture;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Tile Update"))
	TArray<bool> ArInnerUpdate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Elevation"))
	TArray<float> ArInnerElevation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Terrain Type"))
	TArray<ETerrainType> ArInnerTerrainType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Fire State"))
	TArray<EFireState> ArInnerFireState;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Gradient"))
	TArray<FVector2D> ArInnerGradient;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties|Map Arrays", meta = (DisplayName = "Tiles"))
	TArray<AHexTile*> ArInnerTiles;

	//Construct 2D Array Wrappers around them all
	TArray2D<float> Ar2Fuel = TArray2D<float>(ArInnerFuel);
	TArray2D<float> Ar2Heat = TArray2D<float>(ArInnerHeat);
	TArray2D<float> Ar2Moisture = TArray2D<float>(ArInnerMoisture);
	TArray2D<bool> Ar2Update = TArray2D<bool>(ArInnerUpdate);
	TArray2D<float> Ar2Elevation = TArray2D<float>(ArInnerElevation);
	TArray2D<ETerrainType> Ar2TerrainType = TArray2D<ETerrainType>(ArInnerTerrainType);
	TArray2D<EFireState> Ar2FireState = TArray2D<EFireState>(ArInnerFireState);
	TArray2D<FVector2D> Ar2Gradient = TArray2D<FVector2D>(ArInnerGradient);
	TArray2D<AHexTile*> Ar2Tiles = TArray2D<AHexTile*>(ArInnerTiles);
#pragma endregion DataArrays

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
	void SpawnGrid();

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	void DestroyGrid();

	UFUNCTION(BlueprintCallable, Category = "Terrain Gen", meta = (AutoCreateRefTerm = "InTileData"))
	AHexTile* SpawnTile(FVector pos, FHexPoint Index);

	UFUNCTION(BlueprintCallable, Category = "Map Setup", meta=(IsBindableEvent))
	void TriggerTerrainUpdate();

	UFUNCTION(BlueprintCallable, Category = "TerrainGen")
	void Noise2DToFloatArray(UFastNoiseWrapper* Noise, UPARAM(ref) TArray<float>& TargetArray, bool accuratePos = false, FVector2D range = FVector2D(-1,1));
	
	UFUNCTION(BlueprintCallable, Category = "TerrainGen")
	void Noise2DToIntArray(UFastNoiseWrapper* Noise, UPARAM(ref) TArray<int>& TargetArray, FVector2D range = FVector2D(-1, 1), bool accuratePos = false);

#pragma region GetData
	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FTileData GetTileData(const FHexPoint& Index);

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FTerrainData GetTerrainData(const FHexPoint& Index);

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FFireData GetFireData(const FHexPoint& Index);

	//UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	//bool SetTileData(const FHexPoint& Index, const FTileData& data);

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FTileRef MakeTileRef(FHexPoint Index);

	FTileRef MakeTileRef(int Index);
#pragma endregion GetData


#pragma region Utilities
	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	FORCEINLINE int HexToIndex(const FHexPoint& Index) const {
		FIntPoint coord = Index.ToOffset();
		return coord.X + coord.Y * MapWidth;
	}

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	FORCEINLINE FHexPoint IndexToHex(const int& Index) const {
		FHexPoint coord = { (Index%MapWidth), (Index / MapWidth) };
		return coord;
	}

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	FORCEINLINE FIntPoint IndexToCoord(const int& Index) const {
		FIntPoint coord = { (Index % MapWidth), (Index / MapWidth) };
		return coord;
	}

#pragma endregion Utilities

};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMapProgress : uint8
{
	NONE = 0			UMETA(Hidden),
	INITARRAYS = 1		UMETA(DisplayName = "Arrays Initialised"),
	FILLDATA = 2		UMETA(DisplayName = "Arrays Filled"),
	SPAWNTILES = 4		UMETA(DisplayName = "Tiles Spawned"),
};
ENUM_CLASS_FLAGS(EMapProgress);