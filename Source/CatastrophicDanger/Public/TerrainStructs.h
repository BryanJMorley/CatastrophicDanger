// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexPoint.h"
#include "CDEnums.h"
#include "TerrainStructs.generated.h"

//Forward Declarations
class AHexTile;
class AHexMap;


USTRUCT(BlueprintType)
struct FGradientMap {
	GENERATED_BODY()

	TArray<int8, TFixedAllocator<7>> GradientMap;

	FGradientMap() {
		GradientMap.Init(0, 7);
	}

	FGradientMap(int8 Fill) {
		GradientMap.Init(Fill, 7);
	}

	int8& operator[](int i) {
		return GradientMap[i];
	}

	void operator=(int i) {
		GradientMap.Init(i, 7);
	}

};

USTRUCT(BlueprintType)
struct FTerrainData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	ETerrainType terrainType;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int elevation;

	FGradientMap Gradient;

	FTerrainData() {
		terrainType = ETerrainType::NONE;
		elevation = 0;
		Gradient;
	}

};

USTRUCT(BlueprintType)
struct FFireData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int fuel;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int heat;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int moisture;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool update;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EFireState fireState;

	FFireData() {
		fuel = 0.0;
		heat = 0.0;
		moisture = 0.0;
		update = false;
		fireState = EFireState::NONE;
	}

	//FVector3f Pack() {
	//	return { fuel, heat, moisture };
	//}
};

USTRUCT(BlueprintType)
struct FTileData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FHexPoint tileCoords;

	UPROPERTY(BlueprintReadWrite)
	AHexTile* tile;

	UPROPERTY(BlueprintReadWrite)
	FTerrainData terrainData;

	UPROPERTY(BlueprintReadWrite)
	FFireData fireData;

	FTileData() {
		tileCoords = FHexPoint{ -1,-1,-1 };
		tile = nullptr;
		terrainData;
		fireData;
	}
};

// Dont think I need this actually, delete later


USTRUCT(BlueprintType)
struct FFireRef {
	GENERATED_BODY()

	TWeakPtr<int> fuel;
	TWeakPtr<int> heat;
	TWeakPtr<int> moisture;
	TWeakPtr<bool> update;
	TWeakPtr<EFireState> fireState;

	FFireRef(int& infuel, int& inheat, int& inmoisture, bool& inupdate, EFireState& infireState){
		fuel = MakeShared<int>(infuel);
		heat = MakeShared<int>(inheat);
		moisture = MakeShared<int>(inmoisture);
		update = MakeShared<bool>(inupdate);
		fireState = MakeShared<EFireState>(infireState);
	}

	FFireRef() {
		fuel = nullptr;
		heat = nullptr;
		moisture = nullptr;
		update = nullptr;
		fireState = nullptr;
	}


};

USTRUCT(BlueprintType)
struct FTerrainRef {
	GENERATED_BODY()

	TWeakPtr<ETerrainType> terrainType;
	TWeakPtr<int> elevation;
	TWeakPtr<FGradientMap> Gradient;

	FTerrainRef(ETerrainType& inTerrainType, int& inElevation, FGradientMap& inGradient){
		terrainType = MakeShared<ETerrainType>(inTerrainType);
		elevation = MakeShared<int>(inElevation);
		Gradient = MakeShared<FGradientMap>(inGradient);
	}

	FTerrainRef() : terrainType(nullptr), elevation(nullptr), Gradient(nullptr) {}

};

USTRUCT(BlueprintType)
struct FTileRef {
	GENERATED_BODY()

	FHexPoint tileCoords;
	AHexTile* tile;
	FTerrainRef terrainRef;
	FFireRef fireRef;

	FTileRef() {
		tileCoords;
		tile = nullptr;
		terrainRef;
		fireRef;
	}
};

USTRUCT(BlueprintType)
struct FTileFhmStartValues : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Fuel = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Heat = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Moisture = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FuelRange = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HeatRange = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MoistureRange = 0;
};

UCLASS()
class CATASTROPHICDANGER_API UTileStructs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Gradient To String", CompactNodeTitle = "->", BlueprintAutocast), Category = "Tile Data")
	static FString Conv_GradientToString(FGradientMap GData);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Terrain To String", CompactNodeTitle = "->", BlueprintAutocast), Category = "Tile Data")
	static FString Conv_TerrainToString(FTerrainData TData);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Fire To String", CompactNodeTitle = "->", BlueprintAutocast), Category = "Tile Data")
	static FString Conv_FireToString(FFireData FData);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile To String", CompactNodeTitle = "->", BlueprintAutocast), Category = "Tile Data")
	static FString Conv_TileToString(FTileData HData);

};