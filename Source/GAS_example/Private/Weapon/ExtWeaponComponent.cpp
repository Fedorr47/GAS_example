// Copyright Epic Games, Inc. All Rights Reserved.


#include "Weapon/ExtWeaponComponent.h"
#include "Character/Player/ExtPlayerCharacter.h"
#include "GAS_exampleProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UExtWeaponComponent::UExtWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UExtWeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr && Character->GetPresentMesh() != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetPresentMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UExtWeaponComponent::GetMuzzlePosition(FVector& Location, FRotator& Rotation)
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		Rotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		Location = GetOwner()->GetActorLocation() + Rotation.RotateVector(MuzzleOffset);
	}
}

bool UExtWeaponComponent::AttachWeapon(AExtBaseCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (!IsValid(Character))
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	if (Character->GetPresentMesh() != nullptr)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Character->GetPresentMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	}

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	if (auto ExtPlayer = Cast<AExtPlayerCharacter>(Character))
	{
		ExtPlayer->BindActionAbility(InputActions, FireMappingContext);
	}
	
	for (int32 AbilityIndex = 0; AbilityIndex < ProvidedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FExtAbilitySet_GameplayAbility& AbilityToGrant = ProvidedGameplayAbilities[AbilityIndex];
		Character->AddAbility(&AbilityToGrant, this);
	}
	
	Character->AddWeapon(this);

	return true;
}

void UExtWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}