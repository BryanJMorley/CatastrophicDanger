// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexPoint.h"
#include "UObject/Object.h"

template <typename ElementType>
class TArray2D
{
public:
	int Size = 0;
	TArray<ElementType>* Inner;

public:
	TArray2D<ElementType>(TArray<ElementType>& TargetArray, int InSize)
	{
		Size = InSize;
		Inner = &TargetArray;
		Inner->SetNumZeroed(InSize*InSize);
	}

	TArray2D<ElementType>(TArray<ElementType>& TargetArray)
	{
		Inner = &TargetArray;
	}

	void Init(TArray<ElementType>& TargetArray, int InSize) {
		Size = InSize;
		Inner = &TargetArray;
		Inner->SetNumZeroed(InSize*InSize);
	}
	
	ElementType& operator()(int i) {
		return Inner->GetData()[i];
	}
	
	ElementType& operator()(int iX, int iY) {
		return Inner->GetData()[iX + iY*Size];
	}

	ElementType& operator()(FIntPoint i) {
		return Inner->GetData()[i.X + i.Y * Size];
	}

	ElementType& operator()(const FHexPoint& i) {
		FIntPoint coord = i.ToOffset();
		return Inner->GetData()[coord.X+coord.Y*Size];
	}
	
	/*
	const ElementType& operator()(FHexPoint i) {
		return Inner(i.ToOffset);
	}*/

	inline int FlatIndex(const FHexPoint& i) const {
		FIntPoint coord = i.ToOffset();
		return coord.X + coord.Y * Size;
	}
	inline int FlatIndex(FIntPoint& i) const {
		return i.X + i.Y * Size;
	}
	inline int FlatIndex(int ix, int iy) const {
		return ix + iy * Size;
	}

};