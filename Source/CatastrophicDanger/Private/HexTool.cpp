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

int UHexTool::FlattenCoords(FIntPoint I, int Size)
{
    return I.X + I.Y*Size;
}

FIntPoint UHexTool::IndexToCoord(int I, int Size)
{
    return { (I % Size), (I / Size) };
}

TArray<int> UHexTool::TileNeighbors(const int& Index, int Size, bool IncludeSelf)
{
    TArray<FHexPoint> Adj; 
    if (IncludeSelf) {
        Adj = IndexToHex(Index, Size).HexRadius1();
    }
    else {
        Adj = IndexToHex(Index, Size).HexAdjacent();
    }

    TArray<int> intAdj;
    for (FHexPoint H : Adj) {
        if (HexInBounds(H, Size)) {
            intAdj.Add(H.Flatten(Size));
        }
    }
    return intAdj;
}

FHexPoint UHexTool::HexLerp(const FHexPoint& A, const FHexPoint& B, float alpha)
{
    FHexFrac Af = A;
    FHexFrac Bf = B;
    return RoundToHex({ FMath::Lerp(Af.X, Bf.X, alpha), FMath::Lerp(Af.Y, B.Y, alpha), FMath::Lerp(Af.Z, Bf.Z, alpha) });
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