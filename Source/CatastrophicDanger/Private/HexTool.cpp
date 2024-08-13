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

FHexPoint UHexTool::HexMidpoint(const FHexPoint& A, const FHexPoint& B)
{
    return HexLerp(A, B, .5);
}

void UHexTool::HexRayOrthogonal(const FHexPoint& Start, int direction, int distance, TArray<FHexPoint>& OutRay)
{
    FHexPoint Vec = { distance, -distance, 0 };
    Vec = HexVecRotate(Vec, direction);
    Vec = Start + Vec;
    HexLine(Start, Vec, OutRay);
}

void UHexTool::HexRay(const FHexPoint& Start, float direction, int distance, TArray<FHexPoint>& OutRay)
{
    FVector RayVec = { float(distance)*1.5, 0.0, 0.0 };
    RayVec = RayVec.RotateAngleAxis(direction, { 0,0,1});
    RayVec += HexToPos(Start, 1);
    HexLine(Start, PosToHex({ RayVec.X, RayVec.Y }, 250), OutRay, distance);
    
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

 void UHexTool::HexLine(const FHexPoint& Start, const FHexPoint& End, TArray<FHexPoint>& OutLine, int sizeCap)
 {
     float dist = HexDistance(Start, End);
     bool useSize = sizeCap>0;

     if (!useSize) {
         for (int i = 0; i <= dist; i++) {
             OutLine.Add(HexLerp(End, Start, i / dist));
         }
     }
     else
     {
         for (int i = 0; i <= dist && i <= sizeCap; i++) {
             OutLine.Add(HexLerp(End, Start, i / dist));
         }
     }
 }

 void UHexTool::HexRandomWalk(const FHexPoint& Start, int distance, int direction, float randomness, float bias, TArray<FHexPoint>& OutPath)
 {
     int wanderDir = direction;
     OutPath.Add(Start);

     for (int i = 0; i <= distance; i++) {
         OutPath.Add(OutPath.Last().HexDirection(wanderDir));
         float backBend = (wanderDir - direction)*bias;
         float offset = (FMath::FRand() - .5) * 2 * randomness - backBend;
         wanderDir += round(offset);
     }
 }

 void UHexTool::HexSetToInt(const TSet<FHexPoint>& InHexes, int size, TSet<int>& OutSet)
 {
     for (FHexPoint H : InHexes) {
         if(HexInBounds(H, size)) OutSet.Add(UHexPointStatic::HexToIndex(H, size));
     }
     return;
 }

 void UHexTool::HexArrayToInt(const TArray<FHexPoint>& InHexes, int size, TArray<int>& OutArray)
 {
     for (FHexPoint H : InHexes) {
         if (HexInBounds(H, size)) OutArray.Add(UHexPointStatic::HexToIndex(H, size));
     }
     return;
 }

 void UHexTool::HexArrayToIntSet(const TArray<FHexPoint>& InHexes, int size, TSet<int>& OutSet)
 {
     for (FHexPoint H : InHexes) {
         if (HexInBounds(H, size)) OutSet.Add(UHexPointStatic::HexToIndex(H, size));
     }
     return;
 }

 FHexPoint UHexTool::HexVecRotate(const FHexPoint& Vec, int dir)
 {
     dir = ModuloWrap(dir, 6);
     switch (dir) {
     case 0:
         return Vec;
     case 1:
         return { -Vec.Z, -Vec.X, -Vec.Y };
     case 2:
         return { Vec.Y, Vec.Z, Vec.X };
     case 3:
         return { -Vec.X, -Vec.Y, -Vec.Z };
     case 4:
         return { Vec.Z, Vec.X, Vec.Y };
     case 5:
         return { -Vec.Y, -Vec.Z, -Vec.X };
     default:
         return { 0,0,0 };
     }
 }

 void UHexTool::HexAllAdjacent(const TSet<FHexPoint>& InHexes, TSet<FHexPoint>& OutSet)
 {
     for (FHexPoint H : InHexes) {
         OutSet.Append(H.HexAdjacent());
     }
     OutSet = OutSet.Difference(InHexes);
 }
