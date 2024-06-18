// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexPoint.h"
#include "TerrainStructs.generated.h"


//Forward Declarations
class AHexTile;
class AHexMap;

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

	UPROPERTY(BlueprintReadWrite)
	ETerrainType terrainType;

	UPROPERTY(BlueprintReadWrite)
	float elevation;

	UPROPERTY(BlueprintReadWrite)
	FVector2f gradient;

	FTerrainData() {
		terrainType = ETerrainType::NONE;
		elevation = 0.0;
		gradient = { 0.0, 1.0 };
	}

};

USTRUCT(BlueprintType)
struct FFireData {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float fuel;

	UPROPERTY(BlueprintReadWrite)
	float heat;

	UPROPERTY(BlueprintReadWrite)
	float moisture;

	UPROPERTY(BlueprintReadWrite)
	bool update;

	UPROPERTY(BlueprintReadWrite)
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
		tileCoords = { -1, -1 };
		tile = nullptr;
		terrainData;
		fireData;
	}
};


class CATASTROPHICDANGER_API TerrainStructs
{
public:
	TerrainStructs();
	~TerrainStructs();
};