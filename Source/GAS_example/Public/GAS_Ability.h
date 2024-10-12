// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "../GAS_example.h"
#include "GAS_Ability.generated.h"

/**
 * 
 */
UCLASS()
class GAS_EXAMPLE_API UGAS_Ability : public UGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Ability")
	EAbilityInputID AbilityInputID{ EAbilityInputID::None };

public:
	EAbilityInputID GetAbilityInputID() { return AbilityInputID; }	
};
