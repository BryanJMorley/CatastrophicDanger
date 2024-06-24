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

	FHexPoint(const FIntVector& Point) {
			X = Point.X;
			Y = Point.Y;
			Z = Point.Z;
	}

	FHexPoint(int Q, int R, int S) {
		X = Q;
		Y = R;
		Z = S;
	}

	FHexPoint(const FIntPoint& Point) {
		X = Point.X;
		Y = Point.Y - (X - (X & 1)) / 2;
		Z = -X-Y;
	}

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

	inline FIntPoint ToOffset() {
		return { X, Y + (X - (X&1)) / 2};
	}

	inline FIntPoint ToOffset(FHexPoint inH) {
		return { inH.X, inH.Y + (inH.X - (inH.X & 1)) / 2 };
	}

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