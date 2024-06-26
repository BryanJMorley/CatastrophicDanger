// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTile.h"
#include "HexMap.h"

// Sets default values

#pragma region Constructors

AHexTile::AHexTile()
{
	map = nullptr;
	tileCoords = FHexPoint{ -1, -1 , -1 };
	fireState = EFireState::NONE;
	terrainData;
	//tileMesh = nullptr;
}

AHexTile::AHexTile(AHexMap* inMap)
{
	map = inMap;
	tileCoords = FHexPoint{ -1, -1 , -1};
	fireState = EFireState::NONE;
	terrainData;
	//tileMesh = nullptr;
}
AHexTile::AHexTile(AHexMap* inMap, const FTileData& InData)
{
	map = inMap;
	tileCoords = InData.tileCoords;
	fireState = InData.fireData.fireState;
	terrainData = &InData.terrainData;
	fireData = &InData.fireData;
}

#pragma endregion Constructors

#pragma region Initialise

AHexTile* AHexTile::Initialise(AHexMap* inMap)
{
	map = inMap;
	tileCoords = FHexPoint{ -1, -1 , -1 };
	fireState = EFireState::NONE;
	terrainData;
	return this;
}

AHexTile* AHexTile::Initialise(AHexMap* inMap, const FTileData& InData)
{
	map = inMap;
	tileCoords = InData.tileCoords;
	fireState = InData.fireData.fireState;
	terrainData = &InData.terrainData;
	fireData = &InData.fireData;
	return this;
}

#pragma endregion Initialise


// Called when the game starts or when spawned
void AHexTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHexTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}