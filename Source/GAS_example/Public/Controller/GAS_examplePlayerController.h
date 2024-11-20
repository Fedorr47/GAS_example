// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GAS_examplePlayerController.generated.h"

class UInputMappingContext;
class UExtAbilitySystemComponent;

/**
 *
 */
UCLASS()
class GAS_EXAMPLE_API AGAS_examplePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UExtAbilitySystemComponent* GetExtAbilitySystemComponent() const;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface
};
