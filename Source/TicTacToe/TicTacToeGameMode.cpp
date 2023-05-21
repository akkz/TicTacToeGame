// Copyright Epic Games, Inc. All Rights Reserved.

#include "TicTacToeGameMode.h"
#include "TicTacToePlayerController.h"
#include "TicTacToeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATicTacToeGameMode::ATicTacToeGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATicTacToePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Gameplay/Player/BP_ChessPlayer"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Gameplay/Player/BP_ChessController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}