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
	fireState = InMap->ArInnerFireState[tileIndex];
	terrainData = InMap->GetTerrainData(Index);
	map->OnTerrainUpdate.AddUObject(this, &AHexTile::OnTerrainUpdate);
}

#pragma region Updates
void AHexTile::UpdateTransform()
{
	terrainData.elevation = map->ArInnerElevation[tileIndex];
	FTransform newTransform = GetActorTransform();
	FVector CurrentPos = newTransform.GetTranslation();
	newTransform.SetLocation({CurrentPos.X, CurrentPos.Y, terrainData.elevation });
	SetActorTransform(newTransform);
}

void AHexTile::OnTerrainUpdate(bool Transform) {
	GetTerrain();
	if (Transform) UpdateTransform();
}


void AHexTile::GetTerrain()
{
	fireState = map->ArInnerFireState[tileIndex];
	terrainData = map->GetTerrainData(tileCoords);

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