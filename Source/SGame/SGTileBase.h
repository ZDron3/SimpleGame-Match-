// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperSpriteActor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "MessageEndpoint.h"
#include "MessageEndpointBuilder.h"
#include "SGameMessages.h"
#include "iTween/iTInterface.h"

#include "SGTileBase.generated.h"

class ASGGrid;

#define  FILTER_MESSAGE \
	if (FilterMessage(Message.TileID) == false) \
	return;

USTRUCT(BlueprintType)
struct FSGTileData
{
	GENERATED_USTRUCT_BODY();

public:
	/** The base type of the current tile*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESGTileType TileType;

	/** The current tile status flag, should not be accessed anywhere, for test convenient now*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ESGTileStatusFlag> TileStatusArray;

	/** The current tile resource info*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTileResourceUnit> TileResourceArray;

};

USTRUCT(BlueprintType)
struct FSGTileType
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASGTileBase> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OverrideBaseAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OverrideBaseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSGTileData Data;

	FSGTileType()
	{
		Probability = 1;
		TileClass = nullptr;
	}
};

UCLASS()
class SGAME_API ASGTileBase : public APaperSpriteActor, public IiTInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGTileBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** When a tile is touched. */
	UFUNCTION()
	void TilePress(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	/** When the user's finger moves over a tile. */
	UFUNCTION()
	void TileEnter(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	/** When the user's finger release a tile*/
	UFUNCTION()
	void TileRelease(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	/** Mouse surrogate for TilePress. */
	UFUNCTION()
	void TilePress_Mouse(AActor* TouchedActor, FKey ButtonPressed);

	/** Mouse surrogate for TileEnter. */
	UFUNCTION()
	void TileEnter_Mouse(AActor* TouchedActor);

	/** Mouse surrogate for TileRelease*/
	UFUNCTION()
	void TileRelease_Mouse(AActor* TouchedActor, FKey ButtonPressed);

	/** Is current tile selecatable*/
	bool IsSelectable() const;

	void SetGridAddress(int32 NewLocation);
	int32 GetGridAddress() const;

	int32 GetTileID() const { return TileID; }
	void SetTileID(int32 val) { TileID = val; }

	int32 GetSpawnedRound() const { return SpawnedRound; }
	void SetSpawnedRound(int32 val) { SpawnedRound = val; }

	UPROPERTY(BlueprintReadOnly)
	int32 TileTypeID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSGTileData Data;

	// Falling functions
	UFUNCTION(BlueprintImplementableEvent)
	void StartFalling();
	void FinishFalling();

	// Currently all the tile can be collect, even the enemy tile, because it can 
	// be part of the XP resouces
	virtual void OnTileCollected();

	// Currenttly only the enemy tile can take damage
	virtual void OnTileTakeDamage();

	/** Return the tile resource that can be collect */
	virtual TArray<FTileResourceUnit> GetTileResource() const;

	virtual void OnTweenCompleteNative(AiTweenEvent* eventOperator, AActor* actorTweening, USceneComponent* componentTweening, UWidget* widgetTweening, FName tweenName, FHitResult sweepHitResultForMoveEvents, bool successfulTransform) override;

protected:
	/** Location on the grid as a 1D key/value. To find neighbors, ask the grid. */
	UPROPERTY(BlueprintReadOnly, Category = Tile)
	int32 GridAddress;

	/** How quickly tiles slide into place. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	float FallingSpeed;

	/** Location where we will land on the grid as a 1D key/value. Used while falling. */
	int32 LandingGridAddress;

	// The sprite asset for link corners 45 degree
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Sprite_Selected;

	// The sprite asset for link corners 45 degree
	UPROPERTY(Category = Sprite, EditAnywhere, BlueprintReadOnly, meta = (DisplayThumbnail = "true"))
	UPaperSprite* Sprite_Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FVector FallingStartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FVector FallingEndLocation;

	/** Current tile's Id */
	int32 TileID;

	/** Spawned turn */
	int32 SpawnedRound;

	/** Keep a weak reference to the owner*/
	ASGGrid* Grid;

	/** If the Message send to me */
	bool FilterMessage(int32 inTileID)
	{
		if (inTileID == -1)
		{
			// It send to all tiles
			return true;
		}

		return inTileID == TileID;	
	}

private:

	// Holds the messaging endpoint.
	// Note that we don't want the sub class inherited this member,
	// because every message handler should explicitly handled by itself class
	FMessageEndpointPtr MessageEndpoint;

	/** Handles tile become selectalbe */
	void HandleSelectableStatusChange(const FMessage_Gameplay_TileSelectableStatusChange& Message, const IMessageContextRef& Context);

	/** Handles tile become selectalbe */
	void HandleLinkStatusChange(const FMessage_Gameplay_TileLinkedStatusChange& Message, const IMessageContextRef& Context);

	/** Handles tile become selectalbe */
	void HandleTileMove(const FMessage_Gameplay_TileBeginMove& Message, const IMessageContextRef& Context);

	/** Handle tile collected */
	void HandleTileCollected(const FMessage_Gameplay_TileCollect& Message, const IMessageContextRef& Context);

	/** Handle tile linked */
	void HandleTileLinked(const FMessage_Gameplay_TileCollect& Message, const IMessageContextRef& Context);

};
