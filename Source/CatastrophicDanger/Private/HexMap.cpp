// Fill out your copyright notice in the Description page of Project Settings.


#include "HexMap.h"
#include "HexTile.h"
#include "HexTool.h"
#include "HexPoint.h"
#include "Array2D.h"
#include "FastNoiseWrapper.h"


// Sets default values
AHexMap::AHexMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Default Map Properties
	width = 1;
	height = 1;
	tileSize = 250;
	tilesAll;
}

// Called when the game starts or when spawned
void AHexMap::BeginPlay()
{	
	//initialise all the arrays lol
	tilesAll.Init(width, height);
	ar_fuel.Init(width, height);
	ar_heat.Init(width, height);
	ar_moisture.Init(width, height);
	ar_elevation.Init(width, height);
	ar_terrainType.Init(width, height);
	ar_gradient.Init(width, height);
	ar_tiles.Init(width, height);
	Super::BeginPlay();
}

// Called every frame
void AHexMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AHexMap* AHexMap::InitialiseGridData() {
	FIntPoint i = { 0,0 };
	for (i.Y = 0; i.Y < height; i.Y++) {
		for (i.X = 0; i.X < width; i.X++) {
			tilesAll(i) = GetTileRef(FHexPoint(i));
		}
	}
	return this;
}

AHexTile* AHexMap::SpawnTile(FVector pos, const FTileData& InTileData) {
	AHexTile* newTile = Cast<AHexTile>(GetWorld()->SpawnActor<AHexTile>(tileClass, pos, { 0,0,0 }));
	newTile->Initialise(this, InTileData);
	return newTile;
}

AHexMap* AHexMap::SpawnGrid() {
	for(FTileRef& tD : tilesAll.Inner)
	{
		//tD.tile = SpawnTile(UHexTool::HexToPos(tD.tileCoords, tileSize), tD);
	}
	return this;
}

void AHexMap::NoiseToTileElevation(UFastNoiseWrapper* Noise, bool accuratePos) {
	for (FTileRef& tD : tilesAll.Inner) {
		FIntPoint coords = tD.tileCoords.ToOffset();
		*tD.terrainData.elevation.Pin() = Noise->GetNoise2D(coords.X, coords.Y);
	}
}

//FTileData AHexMap::GetTileData(const FHexPoint& Index) {
//	return tilesAll(Index);
//}

FTileRef AHexMap::GetTileRef(const FHexPoint& Index) {
	int fI = tilesAll.FlatIndex(Index);
	FTileRef tile;
	tile.terrainData.terrainType = MakeShared<ETerrainType>(ar_terrainType(fI));
	tile.terrainData.elevation = MakeShared<float>(ar_elevation(fI));
	tile.terrainData.gradient = MakeShared<FVector2D>(ar_gradient(fI));
	tile.fireData.fuel = MakeShared<float>(ar_fuel(fI));
	tile.fireData.heat = MakeShared<float>(ar_heat(fI));
	tile.fireData.moisture = MakeShared<float>(ar_moisture(fI));
	tile.tile = MakeShared<TSubclassOf<AHexTile>>(ar_tiles(fI));
	tile.tileCoords = Index;

	return tile;
}

bool AHexMap::SetTileData(const FHexPoint& Index, const FTileData& data) {
	//tilesAll(Index) = data;
	return true;
}

//
//int AHexMap::FlattenIndex(const FHexPoint& Index) {
//	return FlattenIndex(UHexTool::HexToOffset(Index));
//}
//
//int AHexMap::FlattenIndex(const FIntPoint& Index) {
//	return (Index.X + Index.Y * width);
//}
//
//int AHexMap::FlattenIndex(const int iX, const int iY) {
//	return (iX * height + iY);
//}