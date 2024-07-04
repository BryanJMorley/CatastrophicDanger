// Fill out your copyright notice in the Description page of Project Settings.


#include "FireSystem.h"

void UFireSystem::Tick(float DeltaTime)
{
}

UFireSystem::UFireSystem()
{
	MapSize = 0;
	ArFireBuffer.Init(ArFireBufferInner, 1);
	WeatherSys = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("FIRE INIT"));

}

void UFireSystem::FireUpdateComplete()
{
	OnFireUpdateDelegate.Broadcast();
}

void UFireSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	WeatherSys = GetWorld()->GetSubsystem<UWeatherController>();
}

void UFireSystem::Deinitialize()
{
}

void UFireSystem::SetupMap(int Size) {
	ArFireBuffer.Init(ArFireBufferInner, Size);
}

void UFireSystem::QueueTile(FHexPoint Index) {
	StUpdateQ.Add(Index);
}

void UFireSystem::CalculateFireDelta(FHexPoint Index)
{
}

void UFireSystem::CacheTileDelta(const TArray<int, TFixedAllocator<7>>& InDelta, const FHexPoint& Index)
{
	const TArray<FHexPoint, TFixedAllocator<7>> Adj = { Index.HexRadius1() }; //Get the hex Radius index list
	
	for (int i = 0; i < 7; i++) {
		int flat = Adj[i].Flatten(MapSize);
		if (ArFireBufferInner.IsValidIndex(flat)) {
			ArFireBufferInner[i].Y += InDelta[i];
		}
	}
}


//GetWorld()->GetSubsystem<UWeatherController>();
//void UFireSystem::ApplyFireDelta()
//{
//}

void UFireSystem::FSH_Grassland(const FHexPoint& Index, int8 F, int8 H, int8 M)
{
	if (F > 0) {
		const TArray<float, TFixedAllocator<7>>* WindEffectAdj = WeatherSys->GetWindEffect(); //Get the current Wind Effect
		TArray<int, TFixedAllocator<7>> dHAdj; //Empty array to hold the heat delta

		int dH = FMath::Max(FMath::Max(H - M, 0), F); //Delta Heat is how much heat exceeds moisture, capped by available fuel
		int dF = dH; //store the initial dH to reduce consumed fuel.

		dHAdj.Init(dH, 7);

		for (int i = 0; i < 7; i++) {
			dHAdj[i] *= (*WindEffectAdj)[i]; //scale the heat spread by the wind function
			//TODO: Do the same with gradient.
		}

		CacheTileDelta(dHAdj, Index);
		ArFireBufferInner[Index.Flatten(MapSize)].X -= dF;
	}
}
