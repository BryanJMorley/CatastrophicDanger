// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTile.h"
#include "HexMap.h"



AHexTile::AHexTile()
{
	map = nullptr;
	tileCoords = FHexPoint{ -1, -1 , -1 };
	fireState = EFireState::NONE;
	terrainData;
	//tileMesh = nullptr;
}

void AHexTile::Initialise(AHexMap* InMap, FHexPoint Index)
{
	map = InMap;
	tileCoords = Index;
	tileIndex = InMap->HexToIndex(Index);
	map->OnTerrainUpdateDelegate.AddUObject(this, &AHexTile::OnTerrainUpdate);
}

#pragma region Updates
void AHexTile::UpdateTransform()
{
	terrainData.elevation = map->ArElevation[tileIndex];
	FTransform newTransform = GetActorTransform();
	FVector CurrentPos = newTransform.GetTranslation();
	newTransform.SetLocation({ CurrentPos.X, CurrentPos.Y, (terrainData.elevation * map->ElevationHeightSeperation) });
	SetActorTransform(newTransform);
}

void AHexTile::OnTerrainUpdate(bool Transform, bool MapSetupTrigger) {
	GetTerrain();
	if (Transform) UpdateTransform();

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

void AHexTile::GetTerrain()
{
	terrainData = map->GetTerrainData(tileCoords);
}

FFireData AHexTile::GetFire() const
{
	return map->GetFireData(tileCoords);
}

#pragma endregion Updates


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