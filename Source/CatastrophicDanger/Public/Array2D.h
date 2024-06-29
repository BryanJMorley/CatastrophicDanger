// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexPoint.h"
#include "UObject/Object.h"

template <typename ElementType>
class TArray2D
{
public:
	int XSize = 0;
	int YSize = 0;
	TArray<ElementType>* Inner;

public:
	TArray2D<ElementType>(TArray<ElementType>& TargetArray, int InXSize, int InYSize)
	{
		XSize = InXSize;
		YSize = InYSize;
		Inner = &TargetArray;
		Inner->SetNumZeroed(InXSize * InYSize);
	}

	TArray2D<ElementType>(TArray<ElementType>& TargetArray)
	{
		Inner = &TargetArray;
	}

	void Init(TArray<ElementType>& TargetArray, int InXSize, int InYSize) {
		XSize = InXSize;
		YSize = InYSize;
		Inner = &TargetArray;
		Inner->SetNumZeroed(InXSize * InYSize);
	}
	
	ElementType& operator()(int i) {
		return Inner->GetData()[i];
	}
	
	ElementType& operator()(int iX, int iY) {
		return Inner->GetData()[iX + iY*YSize];
	}

	//const ElementType& operator()(int iX, int iY) {
	//	return Inner[iX + iY * YSize];
	//}


	ElementType& operator()(FIntPoint i) {
		return Inner->GetData()[i.X + i.Y * YSize];
	}

	//const ElementType& operator()(FIntPoint i) {
	//	return Inner[i.X + i.Y * YSize];
	//}

	ElementType& operator()(const FHexPoint& i) {
		FIntPoint coord = i.ToOffset();
		return Inner->GetData()[coord.X+coord.Y*YSize];
	}
	
	/*
	const ElementType& operator()(FHexPoint i) {
		return Inner(i.ToOffset);
	}*/

	inline int FlatIndex(const FHexPoint& i) const {
		FIntPoint coord = i.ToOffset();
		return coord.X + coord.Y * XSize;
	}
	inline int FlatIndex(FIntPoint& i) const {
		return i.X + i.Y * XSize;
	}
	inline int FlatIndex(int ix, int iy) const {
		return ix + iy * XSize;
	}

};