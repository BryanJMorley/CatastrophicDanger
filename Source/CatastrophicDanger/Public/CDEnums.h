// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Map Data Enums
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMapProgress : uint8
{
	NONE = 0			UMETA(Hidden),
	INITARRAYS = 1		UMETA(DisplayName = "Arrays Initialised"),
	FILLDATA = 2		UMETA(DisplayName = "Arrays Filled"),
	SPAWNTILES = 4		UMETA(DisplayName = "Tiles Spawned"),
};
ENUM_CLASS_FLAGS(EMapProgress);


//Terrain Data Enums
UENUM(BlueprintType)
enum class ETileVariable : uint8
{
	ELEVATION		UMETA(DisplayName = "ELEVATION"),
	FUEL			UMETA(DisplayName = "FUEL"),
	HEAT			UMETA(DisplayName = "HEAT"),
	MOISTURE		UMETA(DisplayName = "MOISTURE"),
};

UENUM(BlueprintType)
enum class ETerrainType : uint8
{
	NONE		UMETA(DisplayName = "None"),
	GRASSLAND	UMETA(DisplayName = "Grassland"),
	SHRUBLAND	UMETA(DisplayName = "Shrubland"),
	DRYFOREST	UMETA(DisplayName = "Dry Eucalypt Forest"),
	WETFOREST	UMETA(DisplayName = "Wet Eucalypt Forest"),
	PINEFOREST	UMETA(DisplayName = "Pine Forest"),
	TOWN		UMETA(DisplayName = "Suburbs"),
	RIVER		UMETA(DisplayName = "River"),
};
ENUM_RANGE_BY_COUNT(ETerrainType, ETerrainType::RIVER)


UENUM(BlueprintType)
enum class EFireState : uint8
{
	NONE		UMETA(DisplayName = "None"),
	BURNING		UMETA(DisplayName = "Burning"),
	BURNT		UMETA(DisplayName = "Burnt"),
};


//Game State Enums
UENUM(BlueprintType)
enum class EGameStates : uint8
{
	NONE		UMETA(DisplayName = "NONE"),
	LOADING		UMETA(DisplayName = "LOADING"),
	GENERATING	UMETA(DisplayName = "GENERATING"),
	PLAYING		UMETA(DisplayName = "PLAYING"),
	PAUSED		UMETA(DisplayName = "PAUSED"),
	GAME_END	UMETA(DisplayName = "GAME_END"),
	SAVING		UMETA(DisplayName = "SAVING"),
	EXITING		UMETA(DisplayName = "EXITING"),
};

UENUM(BlueprintType)
enum class ELevelState : uint8
{
	NONE		UMETA(DisplayName = "NONE"),
	LOADING		UMETA(DisplayName = "LOADING"),
	PREP_PHASE	UMETA(DisplayName = "PREP_PHASE"),
	FIRE_PHASE	UMETA(DisplayName = "FIRE_PHASE"),
	GAME_END	UMETA(DisplayName = "GAME_END"),
};

UENUM(BlueprintType)
enum class ETurnState : uint8
{
	NONE			UMETA(DisplayName = "NONE"),
	PLAYER_TURN		UMETA(DisplayName = "PLAYER_TURN"),
	FIRE_THINKING	UMETA(DisplayName = "FIRE_THINKING"),
	FIRE_ANIM		UMETA(DisplayName = "FIRE_ANIM"),
	TUTORIAL_TEXT	UMETA(DisplayName = "TUTORIAL_TEXT"),
};

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
	NONE			UMETA(DisplayName = "NONE"),
	MODERATE		UMETA(DisplayName = "Moderate"),
	HIGH			UMETA(DisplayName = "High"),
	EXTREME			UMETA(DisplayName = "Extreme"),
	CATASTROPHIC	UMETA(DisplayName = "Catastrophic"),
};


//Weather State Enums
UENUM(BlueprintType)
enum class EWindSpeed : uint8
{
	NONE		UMETA(DisplayName = "Still"),
	LIGHT		UMETA(DisplayName = "Light"),
	MEDIUM		UMETA(DisplayName = "Medium"),
	STRONG		UMETA(DisplayName = "Strong"),
};

UENUM(BlueprintType)
enum class EWindDirection : uint8
{
	NONE		UMETA(DisplayName = "None"),
	NE			UMETA(DisplayName = "N.E"),
	N			UMETA(DisplayName = "N"),
	NW			UMETA(DisplayName = "N.W"),
	SW			UMETA(DisplayName = "S.E"),
	S			UMETA(DisplayName = "S"),
	SE			UMETA(DisplayName = "S.W"),
};