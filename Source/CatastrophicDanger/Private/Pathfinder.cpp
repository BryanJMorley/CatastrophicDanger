


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