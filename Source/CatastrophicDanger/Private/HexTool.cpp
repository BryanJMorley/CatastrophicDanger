// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTool.h"

static const float SQRT3 = sqrtf(3);

const FMatrix2x2 hexToPointMatrix = { 1.5f, (SQRT3 / 2.0f), 0.0f, SQRT3 }; //Magic matrix to transform Axial gridCoords to CartesianCoords

const FMatrix2x2 pointToHexMatrix = { 2.0f / 3.0f, -1.0f / 3.0f, 0.0f, SQRT3 / 3.0f }; //Inverse Matrix to do the opposite

FVector UHexTool::HexToPos(const FHexPoint &Coords, float Spacing, float Height) {
    FVector2D pos;
    pos.X = Coords.X;
    pos.Y = Coords.Y;
    pos = TransformPoint(hexToPointMatrix, pos) * Spacing;
    return { pos.X, pos.Y, Height };
}

FHexPoint UHexTool::PosToHex(FVector2D Pos, float Spacing) {
    Pos = Pos/Spacing;
    Pos = TransformPoint(pointToHexMatrix, Pos);
    FHexFrac hexFrac(Pos.X, Pos.Y, -Pos.X-Pos.Y);
    return RoundToHex(hexFrac);
}

FHexPoint UHexTool::RoundToHex(FHexFrac H) {
    FVector hI = { round(H.X), round(H.Y) , round(H.Z) };
    FVector qrsDif = { abs(hI.X - H.X), abs(hI.Y - H.Y), abs(hI.Z - H.Z) };

    if (qrsDif.X > qrsDif.Y && qrsDif.X > qrsDif.Z) {
        hI.X = -hI.Y - hI.Z; //if X is the biggest change, reset it.
    }
    else if (qrsDif.Y > qrsDif.Z) {
        hI.Y = -hI.X - hI.Z; //if Y is the biggest change, reset it.
    } else {
        hI.Z = -hI.X - hI.Y; //if Z is the biggest change, reset it.
    }
    FHexPoint outHex;
    outHex = hI;
    return outHex;
}

FHexPoint UHexTool::makeHexPoint(FVector Pos) {
    return FIntVector(Pos);
}

 FIntPoint UHexTool::HexToOffset(FHexPoint Coords) {
    return Coords.ToOffset();
}

 FHexPoint UHexTool::OffsetToHex(FIntPoint Coords) {
     FHexPoint h;
     h.X = Coords.X;
     h.Y = Coords.Y - (h.X - (h.X & 1)) / 2;
     h.Z = -h.X - h.Y;
     return h;
 }












//
//FHexPoint UHexTool::AxialToOffset(FHexPoint Coords) {
//    FHexPoint offset = { 0, 0 };
//    offset.X = Coords.X;
//    offset.Y = Coords.Y + (Coords.X - (Coords.X & 1)) / 2;
//    return offset;
//}
//
//FHexPoint UHexTool::OffsetToAxial(FHexPoint Coords) {
//    FHexPoint offset = { 0,0 };
//    offset.X = Coords.X;
//    offset.Y = Coords.Y - (Coords.X - (Coords.X & 1)) / 2; //Use &1 (bitwise &) instead of modulo to handle negatives
//    return offset;
//}