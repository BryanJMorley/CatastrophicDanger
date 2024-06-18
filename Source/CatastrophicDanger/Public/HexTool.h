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
	static FVector HexToPos(const FHexPoint& Coords, float Spacing, bool Axial = false);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FVector2f HexToPos2(const FHexPoint& Coords, float Spacing, bool Axial = false);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FIntPoint AxialToOffset(FIntPoint Coords);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FIntPoint OffsetToAxial(FIntPoint Coords);
};


