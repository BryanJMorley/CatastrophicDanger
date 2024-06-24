// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerlinStruct.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CATASTROPHICDANGER_API UPerlin : public UObject
{
public:

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Noise Data")
	int seed = 0;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Noise Data")
	FVector offset = { 0,0,0 };

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Noise Data")
	FVector frequency = { 0,0,0 };

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Noise Data")
	int octaves = 1;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Noise Data")
	double lacunarity = 2.0;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Noise Data")
	double persistence = 1.0;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Noise Data")
	FVector2D remap = { -1, 1 };

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "Noise Data")
	FVector2D clamp = { 0, 0 };

	UFUNCTION(BlueprintCallable, Category = "Perlin Sample")
	double SamplePerlin1D(double x);

	UFUNCTION(BlueprintCallable, Category = "Perlin Sample")
	double SamplePerlin2D(FVector2D uv);

	UFUNCTION(BlueprintCallable, Category = "Perlin Sample")
	double SamplePerlin3D(FVector xyz);

	UFUNCTION(BlueprintCallable, Category = "Perlin Sample")
	TArray<double> ArrayPerlin2D(const TArray<FVector2D>& aUV);

	//UFUNCTION(BlueprintCallable, Category = "Perlin Sample")
	//TArray<double> ArrayPerlin3D(const TArray<FVector>& aXYZ);

	//UFUNCTION(BlueprintCallable, Category = "Perlin Sample")
	//UPerlin* SetRange(FVector2D xy);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Perlin Sample")
	static const UPerlin* SetupPerlin(int InSeed, FVector InOffset, FVector InFrequency, int InOctaves, double InLacunarity, double InPersistence, FVector2D InRemap, FVector2D InClamp);

	UPerlin() {
	}
};
