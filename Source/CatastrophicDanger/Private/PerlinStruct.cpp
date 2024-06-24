// Fill out your copyright notice in the Description page of Project Settings.


// YOU NEED TO INITIALISE THE RANDOM NUMBER GENERATOR BEFORE YOU CAN INVOKE PERLIN FUNCTIONS

#include "PerlinStruct.h"

const UPerlin* UPerlin::SetupPerlin(int InSeed = 0, FVector InOffset = {0,0,0}, FVector InFrequency = {1,1,1}, int InOctaves = 1, double InLacunarity = 2, double InPersistence = 1, FVector2D InRemap = {-1,1}, FVector2D InClamp = {0,0}) {
    UPerlin* P = NewObject<UPerlin>();
    P->seed = InSeed;
    P->offset = InOffset;
    P->frequency = InFrequency;
    P->octaves = InOctaves;
    P->lacunarity = InLacunarity;
    P->persistence = InPersistence;
    P->remap = InRemap;
    P->clamp = InClamp;
    return P;
}

double UPerlin::SamplePerlin1D(double x) {
    FMath::Initial
    double totalVal = 0;
    double max_amp = 0;

    for (int i = 0; i < octaves; i++) {
        double posx = x * frequency.X * pow(lacunarity, i) + offset.X;
        totalVal += FMath::PerlinNoise1D(posx) * pow(persistence, i);
        max_amp += 1 * pow(persistence, i);
    }
    totalVal /= max_amp;
    totalVal = remap.X + (totalVal - -1) * (remap.Y - remap.X) / 2.0; //remap to fit range
    //if (clamp != FVector2D(0,0)) totalVal = FMath::Clamp(totalVal, clamp.X, clamp.Y);
    return totalVal;
}

double UPerlin::SamplePerlin2D(FVector2D uv) {
    double totalVal = 0;
    double max_amp = 0;
    FVector2D freq = { frequency.X, frequency.Y };
    FVector2D off = { offset.X, offset.Y };

    for (int i = 0; i < octaves; i++) {
        FVector2D posUV = uv * freq * pow(lacunarity, i) + off;
        totalVal += FMath::PerlinNoise2D(posUV) * pow(persistence, i);
        max_amp += 1 * pow(persistence, i);
    }
    totalVal /= max_amp;
    totalVal = remap.X + (totalVal - -1) * (remap.Y - remap.X) / 2.0; //remap to fit range
    //if (clamp != FVector2D(0, 0)) totalVal = FMath::Clamp(totalVal, clamp.X, clamp.Y);
    return totalVal;
}

double UPerlin::SamplePerlin3D(FVector xyz) {
    double totalVal = 0;
    double max_amp = 0;

    for (int i = 0; i < octaves; i++) {
        FVector posXYZ = xyz * frequency * pow(lacunarity, i) + offset;
        totalVal += FMath::PerlinNoise3D(posXYZ) * pow(persistence, i);
        max_amp += 1 * pow(persistence, i);
    }
    totalVal /= max_amp;
    totalVal = remap.X + (totalVal - -1) * (remap.Y - remap.X) / 2.0; //remap to fit range
    //if (clamp != FVector2D(0, 0)) totalVal = FMath::Clamp(totalVal, clamp.X, clamp.Y);
    return totalVal;
}


TArray<double> UPerlin::ArrayPerlin2D(const TArray<FVector2D>& aUV) {
    
    TArray<double> outNoise;
    outNoise.SetNumZeroed(aUV.Num());

    double max_amp = 0;
    FVector2D freq = { frequency.X, frequency.Y };
    FVector2D off = { offset.X, offset.Y };

    for (int i = 0; i < octaves; i++) { //calculate Max_Amp ahead of time so we dont have to loop over the elements twice
        max_amp += 1 * pow(persistence, i);
    }

    for (int i = 0; i < octaves; i++) {
        FVector2D fScale = freq * pow(lacunarity, i); //setup the loop variables for each fractal layer
        double aScale = pow(persistence, i);
        max_amp += 1 * aScale;

        for (int a = 0; i < aUV.Num(); a++) { //apply per element in the array
            outNoise[a] = (FMath::PerlinNoise2D(aUV[a] * fScale + off) * aScale) / max_amp;
            outNoise[a] = remap.X + (outNoise[a] - -1) * (remap.Y - remap.X) / 2.0; //remap to fit range
        }
    }
    //if (clamp != FVector2D(0, 0)) totalVal = FMath::Clamp(totalVal, clamp.X, clamp.Y);
    return outNoise;
}