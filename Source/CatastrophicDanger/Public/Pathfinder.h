

#pragma once

#include "CoreMinimal.h"
#include "HexTool.h"
#include "PathStructs.h"
#include "PriorityQueue.h"
#include "Pathfinder.generated.h"




/**
 * 
 */
UCLASS()
class CATASTROPHICDANGER_API UPathfinder : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UPathfinder();
	~UPathfinder();

	inline float Heuristic(FHexPoint A, FHexPoint B) {
		return UHexTool::HexDistance(A, B);
	}


	UFUNCTION(BlueprintCallable)
	static void ReachableTilesAll(const FHexPoint& Start, int distance, int MapSize, TSet<FHexPoint>& OutSet);
	
	UFUNCTION(BlueprintCallable)
	static void ReachableTiles(const FHexPoint& Start, int distance, const TSet<FHexPoint>& InPathMap, TSet<FHexPoint>& OutSet);
	
	//FPath A_Star_Search_All(FHexPoint Start, FHexPoint Goal, int MapSize) {
	//}

	UFUNCTION(BlueprintCallable)
	static void HexLine(const FHexPoint& Start, const FHexPoint& End, TArray<FHexPoint>& OutLine);

	UFUNCTION(BlueprintCallable)
	static void HexSetToInt(const TSet<FHexPoint>& InHexes, int size, TSet<int>& OutSet);



};