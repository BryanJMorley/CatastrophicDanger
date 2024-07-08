// Fill out your copyright notice in the Description page of Project Settings.


#include "HexPoint.h"
#include "CDEnums.h"

FString UHexPointStatic::Conv_HexToString(FHexPoint Hpoint)
{
	return FString::Printf(TEXT("Hex| Q: %d R: %d S: %d Axial| X: %d Y: %d"), Hpoint.X, Hpoint.Y, Hpoint.Z, Hpoint.ToOffset().X, Hpoint.ToOffset().Y);
}

FString UHexPointStatic::PrintAdjacentTiles(FHexPoint Hpoint, int MapSize) {
	FString out = "";
	TArray <FHexPoint, TFixedAllocator<7>> Adj = Hpoint.HexRadius1();
	if (!MapSize) {
		for (int i = 0; i < 7; i++) {
			out.Append(UEnum::GetValueAsName(EWindDirection(i)).ToString());
			out.Append(": ");
			out.Append(Conv_HexToString(Adj[i]));
			out.Append("\n");
		}
	}
	else {
		for (int i = 0; i < 7; i++) {
			out.Append(UEnum::GetValueAsName(EWindDirection(i)).ToString());
			out.Append(": ");
			if (Adj[i].InBounds(MapSize)) {
				out.Append(Conv_HexToString(Adj[i]));
				out.Append("\n");
			}
			else {
				out.Append("Out of Bounds\n");
			}
		}
	}
	return out;
}
