// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtEnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ExtEnhancedInputComponent)

class UInputDataConfig;

UExtEnhancedInputComponent::UExtEnhancedInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UExtEnhancedInputComponent::AddInputMappings(
	const UInputDataConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UExtEnhancedInputComponent::RemoveInputMappings(
	const UInputDataConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UExtEnhancedInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
