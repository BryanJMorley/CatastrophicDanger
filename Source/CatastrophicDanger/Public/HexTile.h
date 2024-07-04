// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerrainStructs.h"
#include "HexTile.generated.h"

class AHexMap;

UCLASS()
class CATASTROPHICDANGER_API AHexTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// All the Grid Map Info required.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Map Data")
	AHexMap* map; //store the responsible map controller

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Map Data")
	FHexPoint tileCoords; //keep a copy of the coords

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Map Data")
	int tileIndex;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Map Data")
	EFireState fireState; //firestate for displayStuff, keep a copy indepent for synchronizing

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Map Data")
	FTerrainData terrainData;


	//Constructor Functions
	AHexTile();

	//Initialiser Functions to do the same
	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "InData"))
	void Initialise(AHexMap* inMap, FHexPoint Index);

	UFUNCTION(BlueprintCallable, Category = "Tile Update")
	void UpdateTransform();
	UFUNCTION(BlueprintCallable, Category = "Tile Update")
	void GetTerrain();

	UFUNCTION(BlueprintCallable, Category = "Tile Update")
	void OnTerrainUpdateDelegate(bool Transform);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	////defining these here because they are fast and small.
	//UFUNCTION(BlueprintCallable, Category = "Map Data")
	//inline FFireRef GetFire() const {
	//	return *fireData;
	//}
	//
	//UFUNCTION(BlueprintCallable, Category = "Map Data")
	//inline FTerrainRef GetTerrain() const{
	//	return *terrainData;
	//}
};
