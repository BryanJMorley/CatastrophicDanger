// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainStructs.h"
#include "HexMap.generated.h"

class AHexTile;


UCLASS(Blueprintable, Placeable)
class CATASTROPHICDANGER_API AHexMap : public AActor
{
	GENERATED_BODY()

private:
	FActorSpawnParameters spawnP;

public:	
	FVector defaultVec;
	
	//Map Properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties")
	uint8 width;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties")
	uint8 height;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map Properties")
	float tileSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Map Properties")
	TSubclassOf<AHexTile> tileClass;

	UPROPERTY(BlueprintReadOnly, Category = "Map Data")
	TArray<FTileData> tilesAll;

	AHexMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Map Setup")
	AHexMap* SpawnGrid();

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	FTileData GetTileData(const FHexPoint& Index);
	
	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	bool SetTileData(const FHexPoint& Index, const FTileData& data);

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	bool AddTileData(const FHexPoint& Index, const FTileData& TileData = FTileData(), bool createTile = true);

	UFUNCTION(BlueprintCallable, Category = "Tile Actors", meta = (AutoCreateRefTerm = "InTileData"))
	AHexTile* SpawnTile(const FVector& pos, const FTileData& InTileData, FName Name = "");

	UFUNCTION(BlueprintCallable, Category = "Tile Data", meta = (AutoCreateRefTerm = "Index"))
	int FlattenIndex(const FHexPoint& Index);

	int FlattenIndex(const int ix, const int iy);
};
