// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSystem.h"
#include "HexMap.h"
#include "Logging/StructuredLog.h"
#include <HexTool.h>

DEFINE_LOG_CATEGORY(LogFire);

UFireSystem::UFireSystem()
{
	ArFireBuffer.Init({ 0,0,0 }, MapSize*MapSize);
}

void UFireSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	WeatherSys = GetWorld()->GetSubsystem<UWeatherController>();
	UE_LOG(LogFire, Display, TEXT("Fire System Initialised"));
	Super::Initialize(Collection);
}

void UFireSystem::Deinitialize()
{
	UE_LOG(LogFire, Warning, TEXT("Fire System DeInitialised!"));
}

void UFireSystem::QueueTile(int Index) {
	TileUpdateQ.Enqueue(Index);
	UE_LOGFMT(LogFire, Display, "Added Tile {0}", Index);
}

void UFireSystem::CalculateFireDelta(int Index)
{
}

//takes in a Fixed 7 Array of ints and an index for the centre tile, then Applies the calculated delta to the adjacent tiles if valid.
void UFireSystem::CacheTileDelta(const TArray<int, TFixedAllocator<7>>& InDelta, int Index)
{
	FHexPoint HexIndex = UHexTool::IndexToHex(Index, MapSize); //get the index as a hex so we can get the adjacent list
	const TArray<FHexPoint, TFixedAllocator<7>> Adj = { HexIndex.HexRadius1() }; //Get the hex Radius index list
	
	for (int i = 0; i < 7; i++) { //for each element in the adj list, check the cell is valid, then apply the heat delta
		if(Adj[i].InBounds(MapSize)) {
			//ArFireBuffer[Adj[i].Flatten(MapSize)].Y += InDelta[i];
		}
	}
}

void UFireSystem::Tick(float DeltaTime)
{
	if (GEngine) {
		if(Active) GEngine->AddOnScreenDebugMessage(1, 1, FColor::Yellow, TEXT("Fire System Active"));
		if (!TileUpdateQ.IsEmpty()) GEngine->AddOnScreenDebugMessage(1, 1, FColor::Yellow, FString::Printf(TEXT("Top Element: %d"), TileUpdateQ.Peek()) );
	}
	if (Active) {

		if (!TileUpdateQ.IsEmpty()) {
			int Tile;
			TileUpdateQ.Dequeue(Tile);
			UE_LOGFMT(LogFire, Display, "Processing Tile: {0}", Tile);
			FireSpreadFunction(Tile, Map->ArFuel[Tile], Map->ArHeat[Tile], Map->ArMoisture[Tile]);
		}
	}
}

//Apply the fire Delta buffer back to the main map 
void UFireSystem::ApplyFireDelta()
{
	for (int i = 0; i < MapSize * MapSize; i++) {
		if (ArFireBuffer[i] == FIntVector()) {
			UE_LOG(LogFire, Display, TEXT("Tile Empty, Skipped"));
		} 
		else {
			UE_LOG(LogFire, Display, TEXT("Tile Contains: F %d"), ArFireBuffer[i].X);
		}
	}
}

//calculate the fire spread for the given tile in the Queue, given its index and its Fhm values, 
//then, save the Delta into the internal fire buffer.
void UFireSystem::FireSpreadFunction(int Index, int F, int H, int M)
{
	if (F > 0) {
		const TArray<float, TFixedAllocator<7>>* WindEffectAdj = WeatherSys->GetWindEffect(); //Get the current Wind Effect
		TArray<int, TFixedAllocator<7>> dHAdj; //Empty array to hold the heat delta

		//int dH = FMath::Max(FMath::Max(H - M, 0), F); //Delta Heat is how much heat exceeds moisture, capped by available fuel
		
		int dH = 5; //TODO: CHANGE THIS BACK TO THE ACTUAL CALCULATION
		int dF = dH; //store the initial dH to reduce consumed fuel.

		dHAdj.Init(dH, 7);//fill Delta Adj with DH to start.

		ETerrainType TerrainType = Map->ArTerrainType[Index];
		FRealCurve* Gradient = FireGradientMaps->FindCurve(UEnum::GetValueAsName(TerrainType), "FireSystem::FireSpreadFunction", true);
		UE_LOG(LogFire, Display, TEXT("%s Gradient Function: 0 %f, 1 %f, 2 %f"), *UEnum::GetValueAsString(TerrainType), Gradient->Eval(0), Gradient->Eval(1), Gradient->Eval(2));

		for (int i = 0; i < 7; i++) {
			dHAdj[i] *= (*WindEffectAdj)[i]; //scale the heat spread by the wind function
			dHAdj[i] *= Map->ArGradient[Index][i]; //same but the gradient
		}

		CacheTileDelta(dHAdj, Index);
		ArFireBuffer[Index].X -= dF;
	}
}

//void UFireSystem::MakeCurvesFromTable(UCurveTable* InTable)
//{
//	auto CurveTable = FireGradientMaps->GetRowMap();
//	for (const auto C : CurveTable) {
//		CurveRows.Add(C.Value);
//		UE_LOG(LogFire, Display, TEXT("Add T.Type from Table: %s"), *C.Key.ToString());
//	}
//}


void UFireSystem::FireUpdateComplete()
{
	OnFireUpdateDelegate.Broadcast();
}