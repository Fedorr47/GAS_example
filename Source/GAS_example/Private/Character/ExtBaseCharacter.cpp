// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ExtBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Abilities/ExtAbilitySystemComponent.h"
#include "Abilities/AttributeSets/ExtCharacterAttributeSet.h"
#include "Abilities/AttributeSets/ExtAbilitySet.h"

// Sets default values
AExtBaseCharacter::AExtBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
    ExtAbilitySystemComponent = CreateDefaultSubobject<UExtAbilitySystemComponent>(TEXT("ExtAbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AExtBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (IsValid(ExtAbilitySystemComponent) && IsValid(CharacterAttributesSet))
    {
        CharacterAttributesSet->OnDamageTaken.AddUObject(this, &AExtBaseCharacter::OnDamageTakenChanged);
        CharacterAttributesSet->OnAccelerationSpeed.AddUObject(this, &AExtBaseCharacter::OnAccelerationSpeedChanged);

        ExtAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributesSet->GetHealthAttribute()).AddUObject(this, &AExtBaseCharacter::OnHealthAttributeChanged);
        ExtAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributesSet->GetShieldAttribute()).AddUObject(this, &AExtBaseCharacter::OnShieldAttributeChanged);
    }
    
}

UAbilitySystemComponent* AExtBaseCharacter::GetAbilitySystemComponent() const
{
	return ExtAbilitySystemComponent;
}

void AExtBaseCharacter::InitializeAbilitySystem()
{
    // Give Abilities, Server only
    if (!HasAuthority() || !ExtAbilitySystemComponent)
        return;

    CharacterAttributesSet = ExtAbilitySystemComponent->GetSet<UExtCharacterAttributeSet>();

    for (const UExtAbilitySet* AbilitySet : AbilitySets)
    {
        if (AbilitySet)
        {
            AbilitySet->GiveToAbilitySystem(ExtAbilitySystemComponent, nullptr);
        }
    }
}

void AExtBaseCharacter::OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
    OnDamageTaken(DamageInstigator, DamageCauser, GameplayTagContainer, Damage);
}

void AExtBaseCharacter::OnAccelerationSpeedChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float AcceleratePower)
{
    OnAcceleratePerformed(DamageInstigator, DamageCauser, GameplayTagContainer, AcceleratePower);
}

void AExtBaseCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
    OnHealthChanged(Data.OldValue, Data.NewValue);
}

void AExtBaseCharacter::OnShieldAttributeChanged(const FOnAttributeChangeData& Data)
{
    OnShieldChanged(Data.OldValue, Data.NewValue);
}

void AExtBaseCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (!ExtAbilitySystemComponent)
        return;

    ExtAbilitySystemComponent->InitAbilityActorInfo(this, this);
    
    InitializeAbilitySystem();
}


