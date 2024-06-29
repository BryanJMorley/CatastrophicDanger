// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexPoint.h"
#include "HexTool.generated.h"

/**
 * 
 */
UCLASS()
class CATASTROPHICDANGER_API UHexTool : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FVector HexToPos(const FHexPoint& Coords, float Spacing, float Height = 0);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint PosToHex(FVector2D Pos, float Spacing);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint makeHexPoint(FVector Pos);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FIntPoint HexToOffset(FHexPoint Coords);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint OffsetToHex(FIntPoint Coords);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint RoundToHex(FHexFrac H);
};



