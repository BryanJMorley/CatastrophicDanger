


#include "Pathfinder.h"

UPathfinder::UPathfinder()
{
}

UPathfinder::~UPathfinder()
{
}

void UPathfinder::ReachableTilesAll(const FHexPoint& Start, int distance, int MapSize, TSet<FHexPoint>& OutSet)
{
	typedef std::pair<FHexPoint, int> MapNode;

	TQueue<MapNode, EQueueMode::Spsc> frontier;
	frontier.Enqueue(MapNode(Start, 0));

	while (!frontier.IsEmpty()) {
		MapNode current;
		frontier.Dequeue(current);
		OutSet.Add(current.first);

		for (FHexPoint N : current.first.HexAdjacent()) {
			if (!OutSet.Contains(N) && N.InBounds(MapSize) && current.second < distance) {
				frontier.Enqueue(MapNode(N, current.second + 1));
			}
		}
	}
}

void UPathfinder::ReachableTiles(const FHexPoint& Start, int distance, const TSet<FHexPoint>& InPathMap, TSet<FHexPoint>& OutSet)
{
	typedef std::pair<FHexPoint, int> MapNode;

	TQueue<MapNode, EQueueMode::Spsc> frontier;
	frontier.Enqueue(MapNode(Start, 0));

	while (!frontier.IsEmpty()) {
		MapNode current;
		frontier.Dequeue(current);
		OutSet.Add(current.first);

		for (FHexPoint N : current.first.HexAdjacent()) {
			if (!OutSet.Contains(N) && InPathMap.Contains(N) && current.second < distance) {
				frontier.Enqueue(MapNode(N, current.second+1));
			}
		}
	}
}

void UPathfinder::ShortestPath(const FHexPoint& Start, const FHexPoint& Goal, const TSet<FHexPoint>& InPathMap, TArray<FHexPoint>& OutPath, bool RemoveStart)
{
	if (!InPathMap.Contains(Start)) return;
	FHexPoint Target = Goal;
	if (!InPathMap.Contains(Goal)) Target = NearestPoint(Goal, InPathMap);



	TMap<FHexPoint, FHexPoint> CameFrom;
	TMap<FHexPoint, int> CostSoFar;
	CameFrom.Add(Start, FHexPoint(-1,-1,-1));
	CostSoFar.Add(Start, 0);
	TPriorityQueue<FHexPoint> Frontier;
	Frontier.Push(Start, 0);

	while (!Frontier.IsEmpty()) {
		FHexPoint Current = Frontier.Pop();

		if (Current == Target) break;

		for (FHexPoint Next : Current.HexAdjacent()) {
			if (!InPathMap.Contains(Next)) continue;

			int NewCost = CostSoFar[Current] + 1;
			if (!CostSoFar.Contains(Next) or NewCost < *CostSoFar.Find(Next)) {
				CostSoFar.Add(Next, NewCost);
				Frontier.Push(Next, NewCost + Heuristic(Target, Next));
				CameFrom.Add(Next, Current);
			}

		}
	}
	BackTracePath(Start, Target, CameFrom, OutPath, RemoveStart);

}

void UPathfinder::BackTracePath(const FHexPoint& Start, const FHexPoint& Goal, const TMap<FHexPoint, FHexPoint>& InCameFromMap, TArray<FHexPoint>& OutPath, bool RemoveStart)
{
	FHexPoint Current = Goal;
	while (Current != Start) {
		OutPath.Add(Current);
		Current = InCameFromMap[Current];
	}
	if(!RemoveStart) OutPath.Add(Start);
	std::reverse(std::begin(OutPath), std::end(OutPath));
}

FHexPoint UPathfinder::NearestPoint(const FHexPoint& Goal, const TSet<FHexPoint>& InPathMap)
{
	if (InPathMap.Contains(Goal)) return Goal;

	int lowestDist = 9999;
	FHexPoint Nearest;

	for (FHexPoint H : InPathMap) {
		if (UHexTool::HexDistance(H, Goal) < lowestDist) {
			lowestDist = UHexTool::HexDistance(H, Goal);
			Nearest = H;
			if (lowestDist <= 1) break;
		}

	}
	return Nearest;
}