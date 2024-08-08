

#pragma once

#include "CoreMinimal.h"
#include "HexTool.h"

/**
 * 
 */
class CATASTROPHICDANGER_API Pathfinder
{
public:
	Pathfinder();
	~Pathfinder();
	
	inline float Heuristic(FHexPoint A, FHexPoint B) {
		return UHexTool::HexDistance(A, B);
	}
	


};
