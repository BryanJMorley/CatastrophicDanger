// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/IntPoint.h" 
#include "HexPoint.generated.h"


USTRUCT(BlueprintType)
struct FHexPoint : public FIntPoint {
	
	GENERATED_BODY()

public: 
	using FIntPoint::FIntPoint;

	FHexPoint(const FIntPoint &Point) {
		X = Point.X;
		Y = Point.Y;
	}

	int const Q() {
		return X;
	}

	int const R() {
		return (Y - (X - (X & 1)) / 2);
	}

	int const S() {
		return (-X - Y - (X - (X & 1)) / 2);
	}

	FHexPoint  QR() const {
		return { X , (Y - (X - (X & 1)) / 2) };
	}

	FIntVector3  QRS() const {
		int r = (Y - (X - (X & 1)));
		return { X , r, -X-r };
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