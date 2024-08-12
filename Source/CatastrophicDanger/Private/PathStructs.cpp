


#include "PathStructs.h"

FPath::FPath(const TArray<FHexPoint>& InPathCoords, FName InName)
{
	PathCoords = InPathCoords;
	PathName = InName;
}

FPath::FPath()
{
	PathCoords.SetNumZeroed(0);
	PathName = "";
}

FPath::~FPath()
{
}

FPath UPathStatic::MakePathStruct(const TArray<FHexPoint>& InPathCoords, FName InName)
{
	return FPath(InPathCoords, InName);
}

void UPathStatic::BreakPathStruct(const FPath& Path, FName& OutName, TArray<FHexPoint>& OutPathCoords)
{
	OutName = Path.PathName;
	OutPathCoords = Path.PathCoords;
}

FPathNode::FPathNode()
{
}

FPathNode::~FPathNode()
{
}