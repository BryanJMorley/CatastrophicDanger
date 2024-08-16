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
	MapSize = 1;
	TileSize = 250;	
}

// Called when the game starts or when spawned
void AHexMap::BeginPlay()
{	
	InitialiseGrids();
	UFireSystem* const FireSys = (UFireSystem*)GetWorld()->GetSubsystem<UFireSystem>();
	SetupFireSystem(FireSys);
	//ACDGameState* const GameState = (ACDGameState*)GetWorld()->GetGameState();

	Super::BeginPlay();
}

// Called every frame
void AHexMap::Tick(float DeltaTime)
{


	Super::Tick(DeltaTime);
}

#pragma region MapSetup

void AHexMap::InitialiseGrids() {
	int MapSize2 = MapSize * MapSize;
	ArFuel.Init(0, MapSize2);
	ArHeat.Init(0, MapSize2);
	ArMoisture.Init(0, MapSize2);
	ArUpdate.Init(false, MapSize2);
	ArElevation.Init(0, MapSize2);
	ArTerrainType.Init(ETerrainType::NONE, MapSize2);
	ArFireState.Init(EFireState::NONE, MapSize2);
	ArTiles.Init(nullptr, MapSize2);
	ArGradient.Init(FGradientMap(1), MapSize2);
	MapSetupState = MapSetupState|int(EMapProgress::INITARRAYS);
}

void AHexMap::SetupFireSystem(UFireSystem* FireSys)
{
	FireSys->MapSize = MapSize;
	FireSys->Map = this;
	FireSys->ArFireBuffer.Init({ 0,0,0 }, MapSize*MapSize);
	UE_LOG(LogFire, Display, TEXT("Fire Map Linked to: %s Object"), *(this->GetName()));
	FireSys->Active = true;
	FireSys->GameState = (ACDGameState*)GetWorld()->GetGameState();
}

void AHexMap::FillFhmFromTerrainTable(UDataTable* InTable)
{
	if (MapSetupState & int(EMapProgress::INITARRAYS)) {
		for (int i = 0; i < MapSize * MapSize; i++) {
			FTileFhmStartValues* FHM = InTable->FindRow<FTileFhmStartValues>(UEnum::GetValueAsName(ArTerrainType[i]), "HexMap::FillFhmFromTerrainTable", true);
			if (FHM) {
				/*const FVector coords = IndexToCoord3(i);
				ArFuel[i] = FHM->Fuel + FMath::Lerp(-FHM->FuelRange.X, FHM->FuelRange.Y, (1+FMath::PerlinNoise3D(coords))/2.0f);
				ArHeat[i] = FHM->Heat + FMath::Lerp(-FHM->HeatRange.X, FHM->HeatRange.Y, (1+FMath::PerlinNoise3D(coords + 1))/2.0f);
				ArMoisture[i] = FHM->Moisture + FMath::Lerp(-FHM->MoistureRange.X, FHM->MoistureRange.Y, (1+FMath::PerlinNoise3D(coords + 2))/2.0f);*/
				ArFuel[i] = FHM->Fuel + FMath::FRandRange(-FHM->FuelRange.X, FHM->FuelRange.Y);
				ArHeat[i] = FHM->Heat + FMath::FRandRange(-FHM->HeatRange.X, FHM->HeatRange.Y);
				ArMoisture[i] = FHM->Moisture + FMath::FRandRange(-FHM->MoistureRange.X, FHM->MoistureRange.Y);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("FAILED TO FIND ROW"));
				break;
			}
		}
		MapSetupState = MapSetupState|int(EMapProgress::FILLDATA);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ARRAYS NOT INITIALISED!"));
	}
}

void AHexMap::CalculateGradient()
{
	if (MapSetupState & int(EMapProgress::INITARRAYS)) { //bitmask check we've setup the arrays
		for (int i = 0; i < MapSize * MapSize; i++)
		{	//for each grid cell
			TArray<FHexPoint, TFixedAllocator<7>> Adj = IndexToHex(i).HexRadius1(); //get the neighbor cells,
			for (int ii = 0; ii < 7; ii++) {
				int AdjIndex = Adj[ii].Flatten(MapSize); //for each of those,
				if (HexInBounds(Adj[ii])) { //if they are inside the map
					ArGradient[i][ii] = ArElevation[Adj[ii].Flatten(MapSize)]- ArElevation[i]; //store the diff in elevation.
					//ArGradient[i][ii++] the ii++ is so we skip the first array element, which is mostly there to make alignment easier with the wind array.
				}
				else {
					ArGradient[i][ii] = INT8_MIN; //if its out of bounds, set it to int8 min
				}
			}
			ArGradient[i][0] = 0; //set the 'self' gradient to zero, just in case.
		}
	}
}

void AHexMap::QueueBurningTiles()
{
	UFireSystem* const FireSys = (UFireSystem*)GetWorld()->GetSubsystem<UFireSystem>();

	for (int i = 0; i < MapSize * MapSize; i++) {
		if (ArFireState[i] == EFireState::BURNING) {
			FireSys->QueueTile(i);
		}
	}
}

ETerrainType AHexMap::TerrainTypeSelector(const float& SoilQual, const float& Temp, FVector2f RangeSoil, FVector2f RangeTemp) const
{
	float SoilLow = RangeSoil.X + (RangeSoil.Y - RangeSoil.X) / 3.0f;
	float SoilMed = RangeSoil.X + 2 * (RangeSoil.Y - RangeSoil.X) / 3.0f;
	float TempLow = RangeTemp.X + (RangeTemp.Y - RangeTemp.X) / 3.0f;
	float TempMed = RangeTemp.X + 2 * (RangeTemp.Y - RangeTemp.X) / 3.0f;

	if (SoilQual < SoilLow) {
		if (Temp < TempLow) {
			return ETerrainType::GRASSLAND;
		}
		else {
			return ETerrainType::SHRUBLAND;
		}
	}
	else if (SoilQual < SoilMed) {
		if (Temp < TempLow) {
			return ETerrainType::WETFOREST;
		}
		else if (Temp < TempMed) {
			return ETerrainType::DRYFOREST;
		}
		else {
			return ETerrainType::GRASSLAND;
		}
	}
	else {
		if (Temp < TempLow) {
			return ETerrainType::PINEFOREST;
		}
		else if (Temp < TempMed) {
			return ETerrainType::WETFOREST;
		}
		else {
			return ETerrainType::DRYFOREST;
		}
	}
}


#pragma endregion MapSetup

#pragma region MapSpawning

void AHexMap::SpawnGrid(bool TriggerUpdate) {
	if (MapSetupState & int(EMapProgress::INITARRAYS)) { //bitmask check we've setup the arrays
		for (int i = 0; i < MapSize*MapSize; i++)
		{
			FVector pos = UHexTool::HexToPos(IndexToHex(i), TileSize, ArElevation[i]* ElevationHeightSeperation);
			ArTiles[i] = SpawnTile(pos, IndexToHex(i));
		}
		MapSetupState = MapSetupState|int(EMapProgress::SPAWNTILES);
		if(TriggerUpdate) TriggerTerrainUpdate(false, true);

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ARRAYS NOT INITIALISED!"))
	}
}

AHexTile* AHexMap::SpawnTile(FVector pos, FHexPoint Index) {
	if (TileClass == nullptr) return nullptr;
	AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(TileClass, pos, { 0,0,0 });
	newTile->Initialise(this, Index);
	//newTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	return newTile;
}

#pragma endregion MapSpawning

#pragma region Noise2Array

FVector2f AHexMap::Noise2DToFloatArray(UFastNoiseWrapper* Noise, UPARAM(ref) TArray<float>& TargetArray, bool accuratePos, FVector2D range) {
	const bool rangeCheck = (range != FVector2D(-1, 1)); //check if we got given a non default range remap value
	float newRangeMult = (range.Y - range.X) / 2.0f;
	FVector2f MinMax = { 0,0 };

	if (!accuratePos) {
			if (rangeCheck) { //Fast with Remapping
				for (int i = 0; i < TargetArray.Num(); i++) {
					const FHexPoint coords = IndexToCoord(i);
					TargetArray[i] = (Noise->GetNoise2D(coords.X, coords.Y)+1)*newRangeMult-range.X;
					MinMax.Y = fmaxf(MinMax.Y, TargetArray[i]);
					MinMax.X = fminf(MinMax.X, TargetArray[i]);
				}
			}
			else { //Fast without Remapping
				for (int i = 0; i < TargetArray.Num(); i++) {
					const FHexPoint coords = IndexToCoord(i);
					TargetArray[i] = Noise->GetNoise2D(coords.X, coords.Y);
					MinMax.Y = fmaxf(MinMax.Y, TargetArray[i]);
					MinMax.X = fminf(MinMax.X, TargetArray[i]);
				}
			}
	}
	else {
		if (rangeCheck) { //Accurate with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				const FHexPoint coords = IndexToCoord(i);
				FVector pos = UHexTool::HexToPos(coords, TileSize);
				TargetArray[i] = (Noise->GetNoise2D(pos.X, pos.Y) + 1) * newRangeMult - range.X;
				MinMax.Y = fmaxf(MinMax.Y, TargetArray[i]);
				MinMax.X = fminf(MinMax.X, TargetArray[i]);
			}
		}
		else { //Accurate with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				const FHexPoint coords = IndexToCoord(i);
				FVector pos = UHexTool::HexToPos(coords, TileSize);
				TargetArray[i] = Noise->GetNoise2D(pos.X, pos.Y);
				MinMax.Y = fmaxf(MinMax.Y, TargetArray[i]);
				MinMax.X = fminf(MinMax.X, TargetArray[i]);
			}
		}
	}
	return MinMax;
}

FIntPoint AHexMap::Noise2DToIntArray(UFastNoiseWrapper* Noise, UPARAM(ref) TArray<int>& TargetArray, bool accuratePos, FVector2D range) {
	const bool rangeCheck = (range != FVector2D(-1, 1)); //check if we got given a non default range remap value
	float newRangeMult = (range.Y - range.X) / 2.0f;
	FIntPoint MinMax = { 0,0 };

	if (!accuratePos) {
		if (rangeCheck) { //Fast with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				TargetArray[i] = int((Noise->GetNoise2D(coords.X, coords.Y) + 1) * newRangeMult - range.X);
				MinMax.Y = std::max(MinMax.Y, TargetArray[i]);
				MinMax.X = std::max(MinMax.X, TargetArray[i]);
			}
		}
		else { //Fast without Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				TargetArray[i] = int(Noise->GetNoise2D(coords.X, coords.Y));
				MinMax.Y = std::max(MinMax.Y, TargetArray[i]);
				MinMax.X = std::max(MinMax.X, TargetArray[i]);
			}
		}
	}
	else {
		if (rangeCheck) { //Accurate with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				FVector pos = UHexTool::HexToPos(coords, TileSize);
				TargetArray[i] = int((Noise->GetNoise2D(pos.X, pos.Y) + 1) * newRangeMult - range.X);
				MinMax.Y = std::max(MinMax.Y, TargetArray[i]);
				MinMax.X = std::max(MinMax.X, TargetArray[i]);
			}
		}
		else { //Accurate with Remapping
			for (int i = 0; i < TargetArray.Num(); i++) {
				FHexPoint coords = IndexToCoord(i);
				FVector pos = UHexTool::HexToPos(coords, TileSize);
				TargetArray[i] = int(Noise->GetNoise2D(pos.X, pos.Y));
				MinMax.Y = std::max(MinMax.Y, TargetArray[i]);
				MinMax.X = std::max(MinMax.X, TargetArray[i]);
			}
		}
	}
	return MinMax;
}

#pragma endregion Noise2Array

#pragma region tileDataStructs
//
//FTileRef AHexMap::MakeTileRef(FHexPoint Index) {
//	int i = HexToIndex(Index);
//	FTileRef tile;
//	tile.terrainRef = FTerrainRef(ArTerrainType[i], ArElevation[i], ArGradient[i]);
//	tile.fireRef = FFireRef(ArFuel[i], ArHeat[i], ArMoisture[i], ArUpdate[i], ArFireState[i]);
//	tile.tile = ArTiles[i];
//	tile.tileCoords = Index;
//	return tile;
//}
//
//FTileRef AHexMap::MakeTileRef(int i) {
//	FTileRef tile;
//	tile.terrainRef = FTerrainRef(ArTerrainType[i], ArElevation[i], ArGradient[i]);
//	tile.fireRef = FFireRef(ArFuel[i], ArHeat[i], ArMoisture[i], ArUpdate[i], ArFireState[i]);
//	tile.tile = ArTiles[i];
//	tile.tileCoords = FHexPoint((i % MapSize), int(i / MapSize));
//	return tile;
//}

#pragma endregion tileDataStructs

#pragma region getData 
FTileData AHexMap::GetTileData(const FHexPoint& Index) {
	int flat = Index.Flatten(MapSize);
	FTileData tile;
	tile.terrainData.terrainType = ArTerrainType[flat];
	tile.terrainData.elevation = ArElevation[flat];
	tile.terrainData.Gradient = ArGradient[flat];
	tile.fireData.fuel = ArFuel[flat];
	tile.fireData.heat = ArHeat[flat];
	tile.fireData.moisture = ArMoisture[flat];
	tile.fireData.fireState = ArFireState[flat];
	tile.tile = ArTiles[flat];
	tile.tileCoords = Index;
	return tile;
}

FFireData AHexMap::GetFireData(const FHexPoint& Index) {
	int flat = Index.Flatten(MapSize);
	FFireData tile;
	tile.fuel = ArFuel[flat];
	tile.heat = ArHeat[flat];
	tile.moisture = ArMoisture[flat];
	tile.fireState = ArFireState[flat];
	return tile;
}

FTerrainData AHexMap::GetTerrainData(const FHexPoint& Index) {
	int flat = Index.Flatten(MapSize);
	FTerrainData tile;
	tile.terrainType = ArTerrainType[flat];
	tile.elevation = ArElevation[flat];
	tile.Gradient = ArGradient[flat];
	return tile;
}
#pragma endregion getData 

#pragma region DelegateTriggers

void AHexMap::TriggerTerrainUpdate(bool DoMovement, bool MapSetupTrigger) {
	OnTerrainUpdateDelegate.Broadcast(DoMovement, MapSetupTrigger);
}

void AHexMap::TriggerMapSetupComplete(EMapProgress Progress)
{
	OnMapSetupCompleteDelegate.Broadcast(Progress);
}

#pragma endregion DelegateTriggers

void AHexMap::DestroyGrid() {
	//Delete all Tiles of Tile Class first
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TileClass, FoundActors);
	for (AActor* ActorFound : FoundActors)
	{
		ActorFound->Destroy();
	}
	//then fill the grid with nullptr
	ArTiles.SetNumZeroed(MapSize * MapSize);
}

//bool AHexMap::SetTileData(const FHexPoint& Index, const FTileData& data) {
	//tilesAll(Index) = data;
//	return true;
//}