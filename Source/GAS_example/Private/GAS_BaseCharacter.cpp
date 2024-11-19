// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "ExtAbilitySystemComponent.h"
#include "ExtGameplayAbility.h"
#include "ExtCharacterAttributeSet.h"
#include "ExtAbilitySet.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AGAS_BaseCharacter::AGAS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
    ExtAbilitySystemComponent = CreateDefaultSubobject<UExtAbilitySystemComponent>(TEXT("ExtAbilitySystemComponent"));
	CharacterAttributesSet = CreateDefaultSubobject<UExtCharacterAttributeSet>("CharacterAttributeSet");
}

// Called when the game starts or when spawned
void AGAS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAttributesSet->OnDamageTaken.AddUObject(this, &AGAS_BaseCharacter::OnDamageTakenChanged);
    CharacterAttributesSet->OnAccelerationSpeed.AddUObject(this, &AGAS_BaseCharacter::OnAccelerationSpeedChanged);

	ExtAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributesSet->GetHealthAttribute()).AddUObject(this, &AGAS_BaseCharacter::OnHealthAttributeChanged);
	ExtAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributesSet->GetShieldAttribute()).AddUObject(this, &AGAS_BaseCharacter::OnShieldAttributeChanged);
	
}

UAbilitySystemComponent* AGAS_BaseCharacter::GetAbilitySystemComponent() const
{
	return ExtAbilitySystemComponent;
}

void AGAS_BaseCharacter::InitializeAbilitySystem()
{
    // Give Abilities, Server only
    if (!HasAuthority() || !ExtAbilitySystemComponent)
        return;

    for (const UExtAbilitySet* AbilitySet : AbilitySets)
    {
        if (AbilitySet)
        {
            AbilitySet->GiveToAbilitySystem(ExtAbilitySystemComponent, nullptr);
        }
    }
}

void AGAS_BaseCharacter::OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
    OnDamageTaken(DamageInstigator, DamageCauser, GameplayTagContainer, Damage);
}

void AGAS_BaseCharacter::OnAccelerationSpeedChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float AcceleratePower)
{
    OnAcceleratePerformed(DamageInstigator, DamageCauser, GameplayTagContainer, AcceleratePower);
}

void AGAS_BaseCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
    OnHealthChanged(Data.OldValue, Data.NewValue);
}

void AGAS_BaseCharacter::OnShieldAttributeChanged(const FOnAttributeChangeData& Data)
{
    OnShieldChanged(Data.OldValue, Data.NewValue);
}

void AGAS_BaseCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (!ExtAbilitySystemComponent)
        return;

    ExtAbilitySystemComponent->InitAbilityActorInfo(this, this);
    
    InitializeAbilitySystem();
}


