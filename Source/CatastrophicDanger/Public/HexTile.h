// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainStructs.h"
#include "HexTile.generated.h"

class AHexMap;

UCLASS()
class CATASTROPHICDANGER_API AHexTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// All the Grid Map Info required.
	UPROPERTY(BlueprintReadWrite, Category = "Map Data")
	AHexMap* map; //store the responsible map controller

	UPROPERTY(BlueprintReadWrite, Category = "Map Data")
	FHexPoint tileCoords; //keep a copy of the coords

	UPROPERTY(BlueprintReadWrite, Category = "Map Data")
	EFireState fireState; //firestate for displayStuff

	UPROPERTY(BlueprintReadWrite, Category = "Map Data")
	FTerrainData terrainData; //All the Terrain Data for display stuff

	//Actual Mesh Data Stuff
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Map Data")
	UStaticMeshComponent* tileMesh; //All the Terrain Data for display stuff

	//Constructor Functions
	AHexTile();
	AHexTile(AHexMap* inMap);
	AHexTile(AHexMap* inMap, const FHexPoint& Coords, const EFireState inFireState, const FTerrainData& inData);

	//Initialiser Functions to do the same
	AHexTile* Initialise(AHexMap* inMap);
	AHexTile* Initialise(AHexMap* inMap, const FHexPoint& Coords, const EFireState inFireState, const FTerrainData& inData);
	
	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "InData"))
	AHexTile* Initialise(AHexMap* inMap, const FTileData& InData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
