// Fill out your copyright notice in the Description page of Project Settings.


#include "HexMap.h"
#include "HexTile.h"
#include "HexTool.h"
#include "HexPoint.h"
#include "Array2D.h"
#include "Kismet/GameplayStatics.h"
#include "FastNoiseWrapper.h"


// Sets default values
AHexMap::AHexMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Default Map Properties
	MapWidth = 1;
	MapHeight = 1;
	TileSize = 250;	
}

// Called when the game starts or when spawned
void AHexMap::BeginPlay()
{	
	InitialiseGrids();
	Super::BeginPlay();
}

// Called every frame
void AHexMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHexMap::InitialiseGrids() {
	Ar2Fuel.Init(ArInnerFuel, MapWidth, MapHeight);
	Ar2Heat.Init(ArInnerHeat, MapWidth, MapHeight);
	Ar2Moisture.Init(ArInnerMoisture, MapWidth, MapHeight);
	Ar2Update.Init(ArInnerUpdate, MapWidth, MapHeight);
	Ar2Elevation.Init(ArInnerElevation, MapWidth, MapHeight);
	Ar2TerrainType.Init(ArInnerTerrainType, MapWidth, MapHeight);
	Ar2FireState.Init(ArInnerFireState, MapWidth, MapHeight);
	Ar2Gradient.Init(ArInnerGradient, MapWidth, MapHeight);
	Ar2Tiles.Init(ArInnerTiles, MapWidth, MapHeight);
	MapSetupState = MapSetupState|int(EMapProgress::INITARRAYS);
}

AHexTile* AHexMap::SpawnTile(FVector pos, FHexPoint Index) {
	if (TileClass == nullptr) return nullptr;
	AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(TileClass, pos, { 0,0,0 });
	newTile->Initialise(this, Index);
	return newTile;
}

void AHexMap::TriggerTerrainUpdate() {
	OnTerrainUpdate.Broadcast(true);
}

void AHexMap::SpawnGrid() {
	if (MapSetupState & int(EMapProgress::INITARRAYS)) {
		for (int i = 0; i < MapWidth * MapHeight; i++)
		{
			FVector pos = UHexTool::HexToPos(IndexToHex(i), TileSize, ArInnerElevation[i]);
			ArInnerTiles[i] = SpawnTile(pos, IndexToHex(i));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ARRAYS NOT INITIALISED!"))
	}
}

void AHexMap::DestroyGrid() {
	//Delete all Tiles of Tile Class first
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TileClass, FoundActors);
	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}
	//then fill the grid with nullptr
	ArInnerTiles.SetNumZeroed(MapWidth * MapHeight);
}

#pragma region Noise2Array

void AHexMap::Noise2DToFloatArray(UFastNoiseWrapper* Noise, UPARAM(ref) TArray<float>& TargetArray, bool accuratePos, FVector2D range) {
	const bool rangeCheck = (range != FVector2D(-1, 1)); //check if we got given a non default range remap value
	float newRangeMult = (range.Y - range.X) / 2.0f;
	
	if (!accuratePos) {
			if (rangeCheck) { //Fast with Remapping
				for (int i = 0; i < TargetArray.Num(); i++) {
					FHexPoint coords = IndexToCoord(i);
					TargetArray[i] = (Noise->GetNoise2D(coords.X, coords.Y)+1)*newRangeMult-range.X;
				}
			}
			else { //Fast without Remapping
				for (int i = 0; i < TargetArray.Num(); i++) {
					FHexPoint coords = IndexToCoord(i);
					TargetArray[i] = Noise->GetNoise2D(coords.X, coords.Y);
				}
			}
	}
	else {
		if (rangeCheck) { //Accurate with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				FVector pos = UHexTool::HexToPos(coords, TileSize);
				TargetArray[i] = (Noise->GetNoise2D(pos.X, pos.Y) + 1) * newRangeMult - range.X;
			}
		}
		else { //Accurate with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				FVector pos = UHexTool::HexToPos(coords, TileSize);
				TargetArray[i] = Noise->GetNoise2D(pos.X, pos.Y);
			}
		}
	}
}

void AHexMap::Noise2DToIntArray(UFastNoiseWrapper* Noise, UPARAM(ref) TArray<int>& TargetArray, FVector2D range, bool accuratePos) {
	const bool rangeCheck = (range != FVector2D(-1, 1)); //check if we got given a non default range remap value
	float newRangeMult = (range.Y - range.X) / 2.0f;

	if (!accuratePos) {
		if (rangeCheck) { //Fast with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				TargetArray[i] = int((Noise->GetNoise2D(coords.X, coords.Y) + 1) * newRangeMult - range.X);
			}
		}
		else { //Fast without Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				TargetArray[i] = int(Noise->GetNoise2D(coords.X, coords.Y));
			}
		}
	}
	else {
		if (rangeCheck) { //Accurate with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				FVector pos = UHexTool::HexToPos(coords, TileSize);
				TargetArray[i] = int((Noise->GetNoise2D(pos.X, pos.Y) + 1) * newRangeMult - range.X);
			}
		}
		else { //Accurate with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				FVector pos = UHexTool::HexToPos(coords, TileSize);
				TargetArray[i] = int(Noise->GetNoise2D(pos.X, pos.Y));
			}
		}
	}
}

#pragma endregion Noise2Array

FTileRef AHexMap::MakeTileRef(FHexPoint Index) {
	int i = HexToIndex(Index);
	FTileRef tile;
	tile.terrainRef = FTerrainRef(ArInnerTerrainType[i], ArInnerElevation[i], ArInnerGradient[i]);
	tile.fireRef = FFireRef(ArInnerFuel[i], ArInnerHeat[i], ArInnerMoisture[i], ArInnerUpdate[i], ArInnerFireState[i]);
	tile.tile = ArInnerTiles[i];
	tile.tileCoords = Index;
	return tile;
}

FTileRef AHexMap::MakeTileRef(int i) {
	FTileRef tile;
	tile.terrainRef = FTerrainRef(ArInnerTerrainType[i], ArInnerElevation[i], ArInnerGradient[i]);
	tile.fireRef = FFireRef(ArInnerFuel[i], ArInnerHeat[i], ArInnerMoisture[i], ArInnerUpdate[i], ArInnerFireState[i]);
	tile.tile = ArInnerTiles[i];
	tile.tileCoords = FHexPoint((i % MapWidth), int(i / MapHeight));
	return tile;
}

#pragma region getData 
FTileData AHexMap::GetTileData(const FHexPoint& Index) {
	int fI = HexToIndex(Index);
	FTileData tile;
	tile.terrainData.terrainType = Ar2TerrainType(fI);
	tile.terrainData.elevation = Ar2Elevation(fI);
	tile.terrainData.gradient = Ar2Gradient(fI);
	tile.fireData.fuel = Ar2Fuel(fI);
	tile.fireData.heat = Ar2Heat(fI);
	tile.fireData.moisture = Ar2Moisture(fI);
	tile.fireData.fireState = Ar2FireState(fI);
	tile.tile = Ar2Tiles(fI);
	tile.tileCoords = Index;
	return tile;
}

FFireData AHexMap::GetFireData(const FHexPoint& Index) {
	int fI = HexToIndex(Index);
	FFireData tile;
	tile.fuel = Ar2Fuel(fI);
	tile.heat = Ar2Heat(fI);
	tile.moisture = Ar2Moisture(fI);
	tile.fireState = Ar2FireState(fI);
	return tile;
}

FTerrainData AHexMap::GetTerrainData(const FHexPoint& Index) {
	int fI = HexToIndex(Index);
	FTerrainData tile;
	tile.terrainType = Ar2TerrainType(fI);
	tile.elevation = Ar2Elevation(fI);
	tile.gradient = Ar2Gradient(fI);
	return tile;
}
#pragma endregion getData 

//bool AHexMap::SetTileData(const FHexPoint& Index, const FTileData& data) {
	//tilesAll(Index) = data;
//	return true;
//}