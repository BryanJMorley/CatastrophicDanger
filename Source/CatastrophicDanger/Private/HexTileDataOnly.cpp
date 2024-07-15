


#include "HexTileDataOnly.h"
#include "HexMap.h"


UHexTileDataOnly::UHexTileDataOnly()
{
	map = nullptr;
	tileCoords = FHexPoint{ -1, -1 , -1 };
	fireState = EFireState::NONE;
	terrainData;
	//tileMesh = nullptr;
}

void UHexTileDataOnly::Initialise(AHexMap* InMap, FHexPoint Index)
{
	map = InMap;
	tileCoords = Index;
	tileIndex = InMap->HexToIndex(Index);
	map->OnTerrainUpdateDelegate.AddDynamic(this, &UHexTileDataOnly::OnTerrainUpdate);
}

#pragma region Updates

void UHexTileDataOnly::OnTerrainUpdate(bool Transform, bool MapSetupTrigger) {
	GetTerrain();

	if (MapSetupTrigger) ReceiveSetupUpdate();
	else ReceiveTerrainUpdate();

	EFireState UpdatedFire = map->ArFireState[tileIndex];
	if (UpdatedFire != fireState) {

		if (UpdatedFire == EFireState::BURNING) {
			fireState = UpdatedFire;
			ReceiveStartBurning();
		}
		if (UpdatedFire == EFireState::BURNT) {
			fireState = UpdatedFire;
			ReceiveStopBurning();
		}
	}
}

void UHexTileDataOnly::GetTerrain()
{
	terrainData = map->GetTerrainData(tileCoords);
}

FFireData UHexTileDataOnly::GetFire() const
{
	return map->GetFireData(tileCoords);
}

#pragma endregion Updates
