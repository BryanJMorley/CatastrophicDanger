// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/IntPoint.h" 
#include "HexPoint.generated.h"


USTRUCT(BlueprintType)
struct FHexPoint : public FIntVector {

	GENERATED_BODY()

public:
	using FIntVector::FIntVector;

	FHexPoint(FIntVector Point) {
		X = Point.X;
		Y = Point.Y;
		Z = Point.Z;
	}

	FHexPoint(FHexFrac Point);

	FHexPoint(int Q, int R, int S) {
		X = Q;
		Y = R;
		Z = S;
	}

	//constructs cubic coords from an int point
	FHexPoint(FIntPoint Point) {
		X = Point.X;
		Y = Point.Y - (X - (X & 1)) / 2;
		Z = -X - Y;
	}

	//constructs cubic coords from 2 ints
	FHexPoint(int Q, int R) {
		X = Q;
		Y = R - (Q - (Q & 1)) / 2;
		Z = -X - Y;
	}

	FHexPoint(const FHexPoint& Point) {
		X = Point.X;
		Y = Point.Y;
		Z = Point.Z;
	}

	void operator = (FVector V) {
		X = V.X;
		Y = V.Y;
		Z = V.Z;
	}

	void operator = (FIntVector V) {
		X = V.X;
		Y = V.Y;
		Z = V.Z;
	}

	FORCEINLINE FIntPoint ToOffset() const {
		return { X, Y + (X - (X & 1)) / 2 };
	}

	//returns an array of all the adjacent hex tiles, starting from NorthEast going CounterClockwise
	//where north is asummed to be -Y
	FORCEINLINE TArray <FHexPoint, TFixedAllocator<6>> HexAdjacent() const {
		return { {X + 1, Y - 1, Z}, {X, Y - 1, Z + 1}, {X - 1, Y, Z + 1},
				 {X - 1, Y + 1, Z}, {X, Y + 1, Z - 1}, {X + 1, Y, Z - 1} };
	}

	FORCEINLINE FHexPoint HexDirection(int InDir) {
		return HexAdjacent()[(InDir % 6 + 6) % 6]; //using positive define modulo function
	}
	
	//returns an array of all the adjacent hex tiles, starting from the current hex, then NorthEast going CounterClockwise
	//where north is asummed to be -Y
	FORCEINLINE TArray <FHexPoint, TFixedAllocator<7>> HexRadius1() const {
		return { {X,Y,Z}, {X + 1, Y - 1, Z}, {X, Y - 1, Z + 1}, {X - 1, Y, Z + 1}, 
						  {X - 1, Y + 1, Z}, {X, Y + 1, Z - 1}, {X + 1, Y, Z - 1}};
	}

	FORCEINLINE int Flatten(const int Size) const {
		return X + (Y + (X - (X & 1)) / 2)*Size;
	}

	FORCEINLINE bool InBounds(const int size) const {
		FIntPoint coord = ToOffset();
		return (coord.X >= 0 && coord.X < size && coord.Y >= 0 && coord.Y < size);
	}
	
	/*inline FIntPoint ToOffset(FHexPoint inH) {
		return { inH.X, inH.Y + (inH.X - (inH.X & 1)) / 2 };
	}*/

};


USTRUCT(BlueprintType)
struct FHexFrac : public FVector {

	GENERATED_BODY()

public:
	using FVector::FVector;

	FHexFrac(const FVector& Point) {
		X = Point.X;
		Y = Point.Y;
		Z = -X - Y;
	}

	FHexFrac(double Q, double R, double S) {
		X = Q;
		Y = R;
		Z = S;
	}

	FHexFrac(const FVector2D& Point) {
		X = Point.X;
		Y = Point.Y;
		Z = -X - Y;
	}

	FHexFrac(const FHexFrac& Point) {
		X = Point.X;
		Y = Point.Y;
		Z = Point.Z;
	}

	FHexFrac(const FHexPoint& Point) {
		X = Point.X;
		Y = Point.Y;
		Z = Point.Z;
	}

	inline const FVector2D ToOffset() {
		return { X, Y = (Y - (X - (int(X) & 1)) / 2.0)};
	}
};

/*
UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Coord Conversion")
int const Q();

UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Coord Conversion")
int const R();

UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Coord Conversion")
int const S();

UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Coord Conversion")
FHexPoint const QR();

UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Coord Conversion")
FIntVector3 const QRS();
*/


UCLASS(Abstract)
class CATASTROPHICDANGER_API UHexPointStatic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "HexPoint To String", CompactNodeTitle = "->", BlueprintAutocast), Category = "HexTools")
	static FString Conv_HexToString(FHexPoint Hpoint);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Index to HexPoint", CompactNodeTitle = "I2H"), Category = "HexTools")
	static FHexPoint IndexToHex(int Index, int InSize) {
		return { (Index % InSize), (Index / InSize) };
	}
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Hex to Index", CompactNodeTitle = "H2I"), Category = "HexTools")
	static int HexToIndex(const FHexPoint& Index, int InSize) {
		FIntPoint coord = Index.ToOffset();
		return coord.X + coord.Y * InSize;
	}

	UFUNCTION(BlueprintPure, Category = "HexTools", meta = (DisplayName = "Print Adjacent Hex Coords"))
	static FString PrintAdjacentTiles(FHexPoint Hpoint, int MapSize = 0);
};