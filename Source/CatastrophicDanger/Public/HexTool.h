// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexPoint.h"
#include "HexTool.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FComparison, const FHexPoint&, Point);

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

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint HexMidpoint(const FHexPoint& A, const FHexPoint& B);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexRayOrthogonal(const FHexPoint& Start, int direction, int distance, TArray<FHexPoint>& OutRay);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexRay(const FHexPoint& Start, float direction, int distance, TArray<FHexPoint>& OutRay);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexLine(const FHexPoint& Start, const FHexPoint& End, TArray<FHexPoint>& OutLine, int sizeCap = 0);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools", meta = (input))
    static void RemoveOutOfBounds(UPARAM(ref) TSet<FHexPoint>& InSet, int MapSize);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexRandomWalk(const FHexPoint& Start, int distance, int direction, float randomness, float bias, TArray<FHexPoint>& OutPath);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexSetToInt(const TSet<FHexPoint>& InHexes, int size, TSet<int>& OutSet);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexArrayToInt(const TArray<FHexPoint>& InHexes, int size, TArray<int>& OutArray);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexArrayToIntSet(const TArray<FHexPoint>& InHexes, int size, TSet<int>& OutSet);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static FHexPoint HexVecRotate(const FHexPoint& Vec, int dir);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexAllAdjacent(const TSet<FHexPoint>& InHexes, TSet<FHexPoint>& OutSet);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Hex Tools")
    static void HexAdjacent(const FHexPoint& InHex, TArray<FHexPoint>& OutNeighbors);

    UFUNCTION(BlueprintCallable, Category = "Hex Tools")
    static void HexRadius(const FHexPoint& CentreHex, int radius, TArray<FHexPoint>& OutRadius);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math")
    static inline int ModuloWrap(int a, int b) {
        return (a % b + b) % b;
    }

    UFUNCTION(BlueprintCallable)
    static void FilterArrayByDelegate(FComparison Function, UPARAM(Ref) TArray<FHexPoint>& ToFilter, bool invert = false);

    UFUNCTION(BlueprintCallable)
    static void FilterSetByDelegate(FComparison Function, UPARAM(Ref) TSet<FHexPoint>& ToFilter, bool invert = false);


};