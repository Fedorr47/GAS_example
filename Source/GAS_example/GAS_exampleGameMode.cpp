// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_exampleGameMode.h"
#include "Character/Player/GAS_PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAS_exampleGameMode::AGAS_exampleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/GAS/Character/BP_TestCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
