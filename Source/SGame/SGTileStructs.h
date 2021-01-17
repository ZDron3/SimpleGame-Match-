#pragma once

#include "SGame.h"
#include "SGTileStructs.generated.h"

/** Types of tile base type. */
UENUM(BlueprintType)
enum class ESGTileType : uint8
{
	ETT_Sword = 0,					// Sword
	ETT_Shield = 1,					// Shield
	ETT_Potion = 2,					// Potion
	ETT_Coin = 3,					// Coin
	ETT_Mana = 4,					// Mana
	ETT_Arrow = 5,					// Arrow
	ETT_Soldier = 6					// Soldier
};

/** Types of resource type. */
UENUM(BlueprintType)
enum class ESGResourceType : uint8
{
	ETR_HP = 0,						// HP of the player
	ETT_Armor = 1,					// Armor of the player
	ETT_Money = 2,					// Money
	ETT_XP = 3,						// In game player xp
	ETT_MAX = 4,					// Max resource type
};

/** The tile resource unit, which can be collected */
USTRUCT(BlueprintType)
struct FTileResourceUnit
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESGResourceType ResourceType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float					ResourceAmount;
};


/** Types of every possible tile state flag that the tile will be in, note it can be in multiple state */
UENUM(BlueprintType)
enum class ESGTileStatusFlag : uint8
{
	ESF_STATUS_CHANGED,		// Tile status has changed
	ESF_POISONED,			// Tile is posioned
	ESF_BURNING,			// Tile is burning
	ESF_PATH_TYPE,			// Tile is the same type as path
	ESF_DEAD,				// Tile is dead
	ESF_SELECTABLE,			// Tile is linkable
	ESF_MATCHABLE,			// Tile is matchable to the current path
	ESF_BROKEN,				// Tile is broken
	ESF_FROZEN,				// Tile is frozen
	ESF_LINKED,				// Tile is linked
};