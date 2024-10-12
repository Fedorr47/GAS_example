// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_exampleGameMode.h"
#include "GAS_exampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAS_exampleGameMode::AGAS_exampleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/GAS/Character/BP_GAS_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
