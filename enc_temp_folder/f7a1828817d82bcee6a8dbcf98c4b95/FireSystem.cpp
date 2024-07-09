// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSystem.h"
#include "HexMap.h"
#include "HexTile.h"
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
	TileUpdateQ.Push(Index);
}

float UFireSystem::CalculateFireDelta(const int& F, const int& H, const int& M) const
{
	return FMath::Min(FMath::Max(H - M, 1), F); //Delta Heat is how much heat exceeds moisture, capped by available fuel
}

//takes in a Fixed 7 Array of ints and an index for the centre tile, then Applies the calculated delta to the adjacent tiles if valid.
void UFireSystem::CacheTileDelta(const TArray<float, TFixedAllocator<7>>& InDelta, int Index)
{
	FHexPoint HexIndex = UHexTool::IndexToHex(Index, MapSize); //get the index as a hex so we can get the adjacent list
	const TArray<FHexPoint, TFixedAllocator<7>> Adj = { HexIndex.HexRadius1() }; //Get the hex Radius index list
	
	for (int i = 0; i < 7; i++) { //for each element in the adj list, check the cell is valid, then apply the heat delta
		if(Adj[i].InBounds(MapSize)) {
			int flat = Adj[i].Flatten(MapSize);
			if (ArFireBuffer.IsValidIndex(flat)) {
				ArFireBuffer[flat].Y += InDelta[i];
				//UE_LOGFMT(LogFire, Display, "Cached Tile: {0} Delta {1}", flat, InDelta[i]);
			}
			else {
				UE_LOGFMT(LogFire, Error, "ARRAY INDEX OUT OF BOUNDS\nFireBuffer is Size {0}, trying to add tile: {1} | {2}!", ArFireBuffer.Num(), flat, Adj[i].ToString());
			}
		}
	}
}

void UFireSystem::Tick(float DeltaTime)
{
	if (Active) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Red, FString::Printf(TEXT("Fire Queue Size: %d tiles"), TileUpdateQ.Num()));
		}
		for (int i = 0; i < 3; i++) { //change this to make it process more tiles per tick. TODO: Come up with a more elegant soloution than this.
			if (!TileUpdateQ.IsEmpty()) {
				int Tile = TileUpdateQ.Pop();
				UE_LOGFMT(LogFire, Display, "Processing Tile: {0}", Tile);
				FireSpreadFunction(Tile, Map->ArFuel[Tile], Map->ArHeat[Tile], Map->ArMoisture[Tile]);
			}
			else {
				if (GameState->TurnState == ETurnState::FIRE_THINKING) {
					ApplyFireDelta();
					FireUpdateComplete();
				}
			}
		}
	}
}

//Apply the fire Delta buffer back to the main map 
void UFireSystem::ApplyFireDelta()
{
	for (int i = 0; i < MapSize * MapSize; i++) {
		if (!ArFireBuffer[i].IsNearlyZero()) {
			float& H = ArFireBuffer[i].Y;//setup some refs for readability and accessing
			int& MapM = Map->ArMoisture[i];
			int& MapH = Map->ArHeat[i];
			
			Map->ArFuel[i] += ArFireBuffer[i].X; //Directly Affect The fuel

			if (MapM>0) {
				if ((H*2.0f) > MapM) {
					H -= MapM; //if heat exceeds twice moisture, remove all moisture and apply the remaining heat.
					MapM = 0;
					MapH += ceil(H);
				}
				else {
					MapM -= floor(H / 2.0f); //if heat doesn't exceed moisture, split it between the two, favouring heat.
					MapH += ceil(H / 2.0f);
				}
			}
			else {
				MapH += H;
			}
			IgnitionCheck(i);
		}
	}
	Map->TriggerTerrainUpdate();
	ArFireBuffer.SetNumZeroed(MapSize * MapSize);
}

//calculate the fire spread for the given tile in the Queue, given its index and its Fhm values, 
//then, save the Delta into the internal fire buffer.
void UFireSystem::FireSpreadFunction(int Index, int F, int H, int M)
{
	if (F > 0) {
		ETerrainType TerrainType = Map->ArTerrainType[Index];
		FRealCurve* Gradient = FireGradientMaps->FindCurve(UEnum::GetValueAsName(TerrainType), "FireSystem::FireSpreadFunction", true);
		UE_LOG(LogFire, Display, TEXT("%s Gradient Function: 0 %f, 1 %f, 2 %f"), *UEnum::GetValueAsString(TerrainType), Gradient->Eval(0), Gradient->Eval(1), Gradient->Eval(2));

		const TArray<float, TFixedAllocator<7>>* WindEffectAdj = WeatherSys->GetWindEffect(TerrainType); //Get the current Wind Effect
		TArray<float, TFixedAllocator<7>> dHAdj; //Empty array to hold the heat delta
		
		float dH = CalculateFireDelta(F, H, M);
		UE_LOGFMT(LogFire, Display, "Calculated heat delta of: {0}", dH);
		float dF = dH; //store the initial dH to reduce consumed fuel by.

		dHAdj.Init(dH, 7);//fill Delta Adj with DH to start.

		for (int i = 0; i < 7; i++) {
			dHAdj[i] *= (*WindEffectAdj)[i]; //scale the heat spread by the wind function
			dHAdj[i] *= Gradient->Eval(Map->ArGradient[Index][i]); //same but the gradient
		}

		CacheTileDelta(dHAdj, Index); //store the spread heat
		ArFireBuffer[Index].X -= dF; //same for fuel
	}
}

void UFireSystem::IgnitionCheck(const int& Index)
{
	if (Map->ArHeat[Index] > (Map->ArMoisture[Index] * 2)) {
		Map->ArFireState[Index] = EFireState::BURNING;
	}
	if (Map->ArFuel[Index] <= 1) {
		Map->ArFireState[Index] = EFireState::BURNT;
	}

}

void UFireSystem::FireUpdateComplete()
{
	OnFireUpdateDelegate.Broadcast();
}