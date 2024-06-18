// Fill out your copyright notice in the Description page of Project Settings.


#include "HexMap.h"
#include "HexTile.h"
#include "HexTool.h"
#include "HexPoint.h"



// Sets default values
AHexMap::AHexMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Default Map Properties
	width = 0;
	height = 0;
	tileSize = 50;
	tileClass = nullptr;
	defaultVec = { -1,-1,-1 };
	tilesAll = {};


	//Default Spawning Properties
	spawnP = {};
	spawnP.Owner = this;
}

// Called when the game starts or when spawned
void AHexMap::BeginPlay()
{
	tilesAll.Reserve(width * height + 1);
	tilesAll.SetNumZeroed(width * height + 1);

	Super::BeginPlay();
	
}

// Called every frame
void AHexMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AHexMap* AHexMap::SpawnGrid() {
	for (int ix = 0; ix < width; ix++) {
		for (int iy = 0; iy < height; iy++) {
			AddTileData({ ix, iy });
			tilesAll[FlattenIndex(ix, iy)].tile = SpawnTile(UHexTool::HexToPos({ ix, iy }, tileSize), tilesAll[FlattenIndex(ix, iy)]);
		}
	}
	return this;
}

bool AHexMap::AddTileData(const FHexPoint& Index, const FTileData& TileData, bool createTile) {
	if (Index.X < 0 || Index.Y < 0 || Index.X > width || Index.Y > height) { return false; }
	int fInt = FlattenIndex(Index);
	tilesAll[fInt] = TileData;
	tilesAll[fInt].tileCoords = Index;
	return true;
}


//YOU WERE HERE LOOKING AT SPAWN ACTOR
AHexTile* AHexMap::SpawnTile(const FVector& pos, const FTileData& InTileData, FName Name) {
	//AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(tileClass, Name, pos, { 0,0,0 }, NULL, spawnP);
	newTile->Initialise(this, InTileData);
	return newTile;
}

FTileData AHexMap::GetTileData(const FHexPoint& Index) {
	if (Index.X < 0 || Index.Y < 0 || Index.X > width || Index.Y > height) { return FTileData(); }
	return (tilesAll[FlattenIndex(Index)]);
}

bool AHexMap::SetTileData(const FHexPoint& Index, const FTileData& data) {
	if (Index.X < 0 || Index.Y < 0 || Index.X > width || Index.Y > height) { return false; }
	tilesAll[FlattenIndex(Index)] = data;
	return true;
}

int AHexMap::FlattenIndex(const FHexPoint& Index) {
	return (Index.X + Index.Y*width);
}

int AHexMap::FlattenIndex(const int iX, const int iY) {
	return (iX + iY * width);
}