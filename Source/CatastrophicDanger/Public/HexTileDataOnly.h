

#pragma once

#include "CoreMinimal.h"
#include "TerrainStructs.h"
#include "HexTileDataOnly.generated.h"

class AHexMap;

/**
 * 
 */

UCLASS()
class CATASTROPHICDANGER_API UHexTileDataOnly : public UObject
{
	GENERATED_BODY()

public:
	UHexTileDataOnly();

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

	//Initialiser Functions to do the same
	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "InData"))
	void Initialise(AHexMap* inMap, FHexPoint Index);

	UFUNCTION(BlueprintCallable, Category = "Tile Update")
	void GetTerrain();

	UFUNCTION(BlueprintCallable, Category = "Tile Update")
	FFireData GetFire() const;

	UFUNCTION(BlueprintCallable, Category = "Tile Update")
	void OnTerrainUpdate(bool Transform = false, bool MapSetupTrigger = false);

	UFUNCTION(BlueprintImplementableEvent, Category = "Tile Update", meta = (DisplayName = "OnTerrainUpdate"))
	void ReceiveTerrainUpdate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Tile Update", meta = (DisplayName = "OnTerrainSetup"))
	void ReceiveSetupUpdate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Tile Update", meta = (DisplayName = "StartBurning"))
	void ReceiveStartBurning();

	UFUNCTION(BlueprintImplementableEvent, Category = "Tile Update", meta = (DisplayName = "StopBurning"))
	void ReceiveStopBurning();




protected:
	// Called when the game starts or when spawned


public:
	// Called every frame
};
