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

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static int FlattenCoords(FIntPoint I, int Size);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FIntPoint IndexToCoord(int I, int Size);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint IndexToHex(const int& Index, int Size) {
        FHexPoint coord = { (Index % Size), (Index / Size) };
        return coord;
    }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static TArray<int> TileNeighbors(const int& Index, int Size, bool IncludeSelf);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static bool HexInBounds(const FHexPoint& Index, int Size) {
        FIntPoint coord = Index.ToOffset();
        return (coord.X >= 0 && coord.X < Size && coord.Y >= 0 && coord.Y < Size);
    }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint HexSubtract(const FHexPoint& A, const FHexPoint& B) {
        return FHexPoint(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
    }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static int HexDistance(const FHexPoint& A, const FHexPoint& B) {
        FHexPoint vec = HexSubtract(A, B);
        return (abs(vec.X) + abs(vec.Y) + abs(vec.Z)) / 2;
    }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint HexLerp(const FHexPoint& A, const FHexPoint& B, float alpha);

};