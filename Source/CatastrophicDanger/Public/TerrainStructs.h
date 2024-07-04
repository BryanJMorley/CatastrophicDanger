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
struct FTerrainData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	ETerrainType terrainType;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float elevation;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector2D gradient;

	FTerrainData() {
		terrainType = ETerrainType::NONE;
		elevation = 0.0;
		gradient = { 0.0, 1.0 };
	}

};

USTRUCT(BlueprintType)
struct FFireData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 fuel;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 heat;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 moisture;

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

	TWeakPtr<uint8> fuel;
	TWeakPtr<uint8> heat;
	TWeakPtr<uint8> moisture;
	TWeakPtr<bool> update;
	TWeakPtr<EFireState> fireState;

	FFireRef(uint8& infuel, uint8& inheat, uint8& inmoisture, bool& inupdate, EFireState& infireState){
		fuel = MakeShared<uint8>(infuel);
		heat = MakeShared<uint8>(inheat);
		moisture = MakeShared<uint8>(inmoisture);
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
	TWeakPtr<float> elevation;
	TWeakPtr<FVector2D> gradient;

	FTerrainRef(ETerrainType& inTerrainType, float& inElevation, FVector2D& inGradient){
		terrainType = MakeShared<ETerrainType>(inTerrainType);
		elevation = MakeShared<float>(inElevation);
		gradient = MakeShared<FVector2D>(inGradient);
	}

	FTerrainRef() : terrainType(nullptr), elevation(nullptr), gradient(nullptr) {}

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

UCLASS()
class CATASTROPHICDANGER_API UTileStructs : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Terrain To String", CompactNodeTitle = "->", BlueprintAutocast), Category = "Tile Data")
	static FString Conv_TerrainToString(FTerrainData TData);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Fire To String", CompactNodeTitle = "->", BlueprintAutocast), Category = "Tile Data")
	static FString Conv_FireToString(FFireData FData);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Tile To String", CompactNodeTitle = "->", BlueprintAutocast), Category = "Tile Data")
	static FString Conv_TileToString(FTileData HData);

};