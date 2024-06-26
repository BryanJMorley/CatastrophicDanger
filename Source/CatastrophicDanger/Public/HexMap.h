// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainStructs.h"
#include "Array2D.h"
#include "FastNoiseWrapper.h"
#include "HexMap.generated.h"

class AHexTile;


UCLASS(Blueprintable, Placeable)
class CATASTROPHICDANGER_API AHexMap : public AActor
{
	GENERATED_BODY()

public:	
	
	//Map Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties")
	int width;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties")
	int height;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties")
	float tileSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Map Properties")
	TSubclassOf<AHexTile> tileClass = nullptr;

	//Arrays to hold all the data. Structs are made to reference to it weakly, but it all stays here, which makes certain 
	TArray2D<float> ar_fuel;
	TArray2D<float> ar_heat;
	TArray2D<float> ar_moisture;
	TArray2D<float> ar_elevation;
	TArray2D<ETerrainType> ar_terrainType;
	TArray2D<FVector2D> ar_gradient;
	TArray2D<TSubclassOf<AHexTile>> ar_tiles;

	//Finally, an array of reference Structs that hold pointers to the data in the arrays.
	TArray2D<FTileRef> tilesAll;

	AHexMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	AHexMap* InitialiseGridData();

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	AHexMap* SpawnGrid();

	UFUNCTION(BlueprintCallable, Category = "Terrain Gen", meta = (AutoCreateRefTerm = "InTileData"))
	AHexTile* SpawnTile(FVector pos, const FTileData& InTileData);

	UFUNCTION(BlueprintCallable, Category = "TerrainGen")
	void NoiseToTileElevation(UFastNoiseWrapper* Noise, bool accuratePos = false);

	/*UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FTileData GetTileData(const FHexPoint& Index);*/

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	bool SetTileData(const FHexPoint& Index, const FTileData& data);

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FTileRef GetTileRef(const FHexPoint& Index);	

};
