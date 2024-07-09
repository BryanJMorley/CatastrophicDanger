// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainStructs.h"
#include "Hextile.h"
//forward Declare hex tile
class AHexTile;

FString UTileStructs::Conv_GradientToString(FGradientMap GData)
{
	return FString::Printf(TEXT("C: %d NE: %d N: %d NW: %d SW: %d S: %d SE: %d "), GData[0], GData[1], GData[2], GData[3], GData[4], GData[5], GData[6]);
}

FString UTileStructs::Conv_TerrainToString(FTerrainData TData)
{ 
	return FString::Printf(TEXT("Biome: %s Elevation: %d \nG: %s"), *UEnum::GetValueAsName(TData.terrainType).ToString(), TData.elevation, *Conv_GradientToString(TData.Gradient));
}

FString UTileStructs::Conv_FireToString(FFireData FData)
{
	return FString::Printf(TEXT("Fuel: %f Heat: %f Moisture: %f State: %s Update: %s"), FData.fuel, FData.heat, FData.moisture, *UEnum::GetValueAsName(FData.fireState).ToString(), FData.update ? "true" : "false");
}

FString UTileStructs::Conv_TileToString(FTileData HData)
{
	const FIntPoint coords = HData.tileCoords.ToOffset();
	return FString::Printf(TEXT("Tile|  Grid: X-%d Y-%d Cubic: Q-%d R-%d S-%d Tile: %s \nTerrain|  %s \nFire|  "), coords.X, coords.Y, HData.tileCoords.X, HData.tileCoords.Y, HData.tileCoords.Z, *HData.tile->GetActorNameOrLabel(), *Conv_TerrainToString(HData.terrainData), *Conv_FireToString(HData.fireData));
}

