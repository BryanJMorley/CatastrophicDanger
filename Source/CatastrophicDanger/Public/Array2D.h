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
	TArray<ElementType> Inner;

public:
	void Init(const ElementType& Element, int InXSize, int InYSize) {
		XSize = InXSize;
		YSize = InYSize;
		Inner.Init(Element, InXSize*InYSize);
	}

	void Init(int InXSize, int InYSize) {
		XSize = InXSize;
		YSize = InYSize;
		Inner.SetNumZeroed(InXSize * InYSize);
	}
	
	ElementType& operator[](int i) {
		return Inner[i];
	}
	ElementType& operator()(int iX, int iY) {
		return Inner[iX + iY*YSize];
	}

	//const ElementType& operator()(int iX, int iY) {
	//	return Inner[iX + iY * YSize];
	//}


	ElementType& operator()(FIntPoint i) {
		return Inner[i.X + i.Y * YSize];
	}

	//const ElementType& operator()(FIntPoint i) {
	//	return Inner[i.X + i.Y * YSize];
	//}

	ElementType& operator()(const FHexPoint& i) {
		FIntPoint coord = i.ToOffset();
		return Inner[coord.X+coord.Y*YSize];
	}
	
	/*
	const ElementType& operator()(FHexPoint i) {
		return Inner(i.ToOffset);
	}*/

	inline int FlatIndex(const FHexPoint& i) const {
		FIntPoint coord = i.ToOffset();
		return coord.X + coord.Y * YSize;
	}
	inline int FlatIndex(FIntPoint& i) const {
		return i.X + i.Y * YSize;
	}
	inline int FlatIndex(int ix, int iy) const {
		return ix + ix * YSize;
	}
};