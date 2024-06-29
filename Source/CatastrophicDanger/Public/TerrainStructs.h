// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexPoint.h"
#include "TerrainStructs.generated.h"


//Forward Declarations
class AHexTile;
class AHexMap;

UENUM(BlueprintType)
enum class ETileVariable : uint8
{
	ELEVATION		UMETA(DisplayName = "ELEVATION"),
	FUEL			UMETA(DisplayName = "FUEL"),
	HEAT			UMETA(DisplayName = "HEAT"),
	MOISTURE		UMETA(DisplayName = "MOISTURE"),
};

UENUM(BlueprintType)
enum class ETerrainType : uint8
{
	NONE		UMETA(DisplayName = "None"),
	GRASSLAND	UMETA(DisplayName = "Grassland"),
	FOREST		UMETA(DisplayName = "Forest"),
};  

UENUM(BlueprintType)
enum class EFireState : uint8
{
	NONE		UMETA(DisplayName = "None"),
	BURNING		UMETA(DisplayName = "Burning"),
	BURNT		UMETA(DisplayName = "Burnt"),
};

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
	float fuel;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float heat;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float moisture;

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

	FVector3f Pack() {
		return { fuel, heat, moisture };
	}
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

	TWeakPtr<float> fuel;
	TWeakPtr<float> heat;
	TWeakPtr<float> moisture;
	TWeakPtr<bool> update;
	TWeakPtr<EFireState> fireState;

	FFireRef(float& infuel, float& inheat, float& inmoisture, bool& inupdate, EFireState& infireState){
		fuel = MakeShared<float>(infuel);
		heat = MakeShared<float>(inheat);
		moisture = MakeShared<float>(inmoisture);
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