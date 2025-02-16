// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssignmentFPSGameMode.h"
#include "AssignmentFPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAssignmentFPSGameMode::AAssignmentFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
