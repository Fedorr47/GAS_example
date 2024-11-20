// Copyright Epic Games, Inc. All Rights Reserved.


#include "Controller/GAS_examplePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/ExtAbilitySystemComponent.h"
#include "Character/GAS_BaseCharacter.h"
#include "Engine/LocalPlayer.h"

void AGAS_examplePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

UExtAbilitySystemComponent* AGAS_examplePlayerController::GetExtAbilitySystemComponent() const
{
	const AGAS_BaseCharacter* character = CastChecked<AGAS_BaseCharacter>(GetCharacter(), ECastCheckedType::NullAllowed);
	if (IsValid(character))
	{
		return Cast<UExtAbilitySystemComponent>(character->GetAbilitySystemComponent());
	}
	return nullptr;
}

void AGAS_examplePlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AGAS_examplePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UExtAbilitySystemComponent* ExtASC = GetExtAbilitySystemComponent())
	{
		ExtASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}