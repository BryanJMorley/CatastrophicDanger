// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTool.h"

const FVector2f hexV = { 1.5f, sqrtf(3) }; //Magic numbers to go from hex Coords to Cartesian

const FMatrix2x2 hexM = { 1.5f, (sqrtf(3) / 2.0f), 0.0f, sqrtf(3) }; //Bigger Magic matrix to do the same

FVector2f UHexTool::HexToPos2(const FHexPoint &Coords, float Spacing, bool Axial) {
    FVector2f pos;
    if (Axial) { pos = Coords; }
    else { pos = Coords.QR(); } //convert offset to axial coords if need be
    pos = TransformPoint(hexM, pos) * Spacing; // X * 1.5, Y * sqrt(3)
	return pos;
}

FVector UHexTool::HexToPos(const FHexPoint &Coords, float Spacing, bool Axial) {
    FVector2f pos;
    if (Axial) { pos = Coords; } else { pos = Coords.QR(); } //convert offset to axial coords if need be
    pos = TransformPoint(hexM, pos) * Spacing; // X * 1.5, Y * sqrt(3)
    return { pos.X, pos.Y, 0 };
}



FIntPoint UHexTool::AxialToOffset(FIntPoint Coords) {
    FIntPoint offset = { 0, 0 };
    offset.X = Coords.X;
    offset.Y = Coords.Y + (Coords.X - (Coords.X & 1)) / 2;
    return offset;
}

FIntPoint UHexTool::OffsetToAxial(FIntPoint Coords) {
    FIntPoint offset = { 0,0 };
    offset.X = Coords.X;
    offset.Y = Coords.Y - (Coords.X - (Coords.X & 1)) / 2; //Use &1 (bitwise &) instead of modulo to handle negatives
    return offset;
}