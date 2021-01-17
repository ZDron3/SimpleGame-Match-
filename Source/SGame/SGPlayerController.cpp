// Fill out your copyright notice in the Description page of Project Settings.

#include "SGame.h"
#include "SGPlayerController.h"
#include "SGGameMode.h"
#include "SGCheatManager.h"

ASGPlayerController::ASGPlayerController(const FObjectInitializer& ObjectInitializer)
{
	// We want the mouse cursor to show immediately on startup, without having to click in the window.
	bShowMouseCursor = true;

	// We need click/touch events to interact with our tiles.
	bEnableTouchEvents = bEnableClickEvents = true;
	bEnableTouchOverEvents = bEnableMouseOverEvents = true;

	// Cheat manager allow us to do some fast debugging
	CheatClass = USGCheatManager::StaticClass();
}

void ASGPlayerController::BeginPlay()
{
	MessageEndpoint = FMessageEndpoint::Builder("Gameplay_PC")
		.Handling<FMessage_Gameplay_PlayerBeginInput>(this, &ASGPlayerController::HandlePlayerBeginInput);
	
	if (MessageEndpoint.IsValid() == true)
	{
		// Subscribe the begin input event to allow the player input
		MessageEndpoint->Subscribe<FMessage_Gameplay_PlayerBeginInput>();
	}

	
}

void ASGPlayerController::HandlePlayerBeginInput(const FMessage_Gameplay_PlayerBeginInput& Message, const IMessageContextRef& Context)
{
	UE_LOG(LogSGame, Log, TEXT("Player begin input"));
}
