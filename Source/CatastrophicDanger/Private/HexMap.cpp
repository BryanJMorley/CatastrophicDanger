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
	defaultVec = { -1,-1,-1 };
	tilesAll = {};
	tileTransforms = {};

	//Default Spawning Properties
	spawnP = {};
	spawnP.Owner = this;
}

// Called when the game starts or when spawned
void AHexMap::BeginPlay()
{
	tilesAll.Reserve(width * height);
	tilesAll.SetNumZeroed(width * height);
	tileTransforms.Reserve(width * height);
	tileTransforms.SetNumZeroed(width * height);
	Super::BeginPlay();
}

// Called every frame
void AHexMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AHexMap* AHexMap::InitialiseGridData() {
	//clear the grid and double check size just in case
	tilesAll.Reserve(width * height);
	tilesAll.SetNumZeroed(width * height);
	tileTransforms.Reserve(width * height);
	tileTransforms.SetNumZeroed(width * height);

	FIntPoint i = { 0,0 };
	for (i.X = 0; i.X < width; i.X++) {
		for (i.Y = 0; i.Y < height; i.Y++) {
			FHexPoint iH = UHexTool::OffsetToHex(i);
			AddTileData(iH);
			FVector pos = UHexTool::HexToPos(FHexPoint(i.X, i.Y), tileSize);
			FTransform M = FTransform(FQuat(0,0,0,1).GetNormalized(), pos, FVector(1, 1, 1));
			tileTransforms[FlattenIndex(iH)] = M;
		}
	}
	return this;
}

AHexTile* AHexMap::SpawnTile(FVector pos, const FTileData& InTileData) {
	AHexTile* newTile = Cast<AHexTile>(GetWorld()->SpawnActor<AHexTile>(tileClass, pos, { 0,0,0 }));
	newTile->Initialise(this, InTileData);
	return newTile;
}

bool AHexMap::AddTileData(const FHexPoint& Index, const FTileData& TileData, bool createTile) {
	int fInt = FlattenIndex(Index);
	tilesAll[fInt] = TileData;
	tilesAll[fInt].tileCoords = Index;
	return true;
}

FTileData AHexMap::GetTileData(const FHexPoint& Index) {
	int fI = FlattenIndex(Index);
	if (fI<0 || fI > width * height) { return FTileData(); }
	return tilesAll[fI];
}

bool AHexMap::SetTileData(const FHexPoint& Index, const FTileData& data) {
	int fI = FlattenIndex(Index);
	if (fI<0 || fI > width * height) { return false; }
	tilesAll[fI] = data;
	return true;
}

int AHexMap::FlattenIndex(const FHexPoint& Index) {
	return FlattenIndex(UHexTool::HexToOffset(Index));
}

int AHexMap::FlattenIndex(const FIntPoint& Index) {
	return (Index.X + Index.Y * width);
}

int AHexMap::FlattenIndex(const int iX, const int iY) {
	return (iX * height + iY);
}


//Probably dont need this now
AHexMap* AHexMap::SpawnGrid() {
	FIntPoint i = { 0,0 };
	for (i.X = 0; i.X < width; i.X++) {
		for (i.Y = 0; i.Y < height; i.Y++) {
			FHexPoint iH = UHexTool::OffsetToHex(i);
			AddTileData(iH);
			tilesAll[FlattenIndex(i)].tile = SpawnTile(UHexTool::HexToPos(iH, tileSize), tilesAll[FlattenIndex(i)]);
		}
	}
	return this;
}