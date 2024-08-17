

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

	FORCEINLINE static float Heuristic(const FHexPoint& A, const FHexPoint& B) {
		return UHexTool::HexDistance(A, B);
	}

	UFUNCTION(BlueprintCallable)
	static void ReachableTilesAll(const FHexPoint& Start, int distance, int MapSize, TSet<FHexPoint>& OutSet);
	
	UFUNCTION(BlueprintCallable)
	static void ReachableTiles(const FHexPoint& Start, int distance, const TSet<FHexPoint>& InPathMap, TSet<FHexPoint>& OutSet);
	
	UFUNCTION(BlueprintCallable)
	static void ShortestPath(const FHexPoint& Start, const FHexPoint& Goal, const TSet<FHexPoint>& InPathMap, TArray<FHexPoint>& OutPath, bool RemoveStart = true);

	static void BackTracePath(const FHexPoint& Start, const FHexPoint& Goal, const TMap<FHexPoint, FHexPoint>& InCameFromMap, TArray<FHexPoint>& OutPath, bool RemoveStart = true);

	UFUNCTION(BlueprintCallable)
	static FHexPoint NearestPoint(const FHexPoint& Goal, const TSet<FHexPoint>& InPathMap);




	//FPath A_Star_Search_All(FHexPoint Start, FHexPoint Goal, int MapSize) {
	//}

};