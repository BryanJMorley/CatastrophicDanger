

#pragma once

#include "CoreMinimal.h"
#include "HexPoint.h"
#include "PathStructs.generated.h"



/**
 * 
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "CatastrophicDanger.PathStatic.MakePathStruct", 
							   HasNativeBreak = "CatastrophicDanger.PathStatic.BreakPathStruct"))
struct CATASTROPHICDANGER_API FPath
{
GENERATED_BODY()
public:

	TArray<FHexPoint> PathCoords;
	FName PathName;

	FPath();
	
	FPath(const TArray<FHexPoint>& InPathCoords, FName InName);

	~FPath();

};

struct CATASTROPHICDANGER_API FPathNode : public FHexPoint
{

	TArray<bool, TFixedAllocator<6>> Edges;
	FHexPoint Coords;

	FPathNode();
	~FPathNode();


};


UCLASS(Abstract)
class CATASTROPHICDANGER_API UPathStatic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Map Paths", meta = (BlueprintThreadSafe))
	static FPath MakePathStruct(const TArray<FHexPoint>& InPathCoords, FName InName);

	UFUNCTION(BlueprintPure, Category = "Map Paths", meta = (BlueprintThreadSafe))
	static void BreakPathStruct(const FPath& Path, FName& OutName, TArray<FHexPoint>& OutPathCoords);


};