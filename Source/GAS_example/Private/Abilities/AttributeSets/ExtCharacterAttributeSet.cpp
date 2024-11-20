// Fill out your copyright notice in the Description page of Project Settings.
#include "Abilities/AttributeSets/ExtCharacterAttributeSet.h"

#include "Abilities/ExtAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"

void UExtCharacterAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    else if (Attribute == GetShieldAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
    }
}

bool UExtCharacterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData &Data)
{
    if (!Super::PreGameplayEffectExecute(Data))
    {
        return false;
    }

    // Handle modifying incoming normal damage
    if (Data.EvaluatedData.Attribute == GetInDamageAttribute())
    {
        if (Data.EvaluatedData.Magnitude > 0.0f)
        {
        }
    }
    
    return true;
}

void UExtCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetInDamageAttribute())
    {
        float InDamageDone = GetInDamage();
        SetInDamage(0.0f);
        if (InDamageDone > 0.0f)
        {
            if (OnDamageTaken.IsBound())
            {
                const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
                AActor* Instigator = EffectContext.GetOriginalInstigator();
                AActor* Causer = EffectContext.GetEffectCauser();

                OnDamageTaken.Broadcast(Instigator, Causer, Data.EffectSpec.CapturedSourceTags.GetSpecTags(), Data.EvaluatedData.Magnitude);
            }

            if (GetShield() > 0.0f)
            {
                const float NewShield = GetShield() - InDamageDone;
                InDamageDone = GetShield();
                SetShield(FMath::Clamp(NewShield, 0.0f, GetMaxShield()));
            }

            if (InDamageDone > 0.0f && GetHealth() > 0.0f)
            {
                const float NewHealth = GetHealth() - InDamageDone;
                SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
            }
        }
    }
    else if (Data.EvaluatedData.Attribute == GetMovementSpeedAttribute())
    {
        SetMovementSpeed(FMath::Clamp(GetMovementSpeed(), 0.0f, GetMaxMovementSpeed()));
    }
    
}

void UExtCharacterAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    Super::PreAttributeBaseChange(Attribute, NewValue);

    ClampAttribute(Attribute, NewValue);
}

void UExtCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    ClampAttribute(Attribute, NewValue);
}

void UExtCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    if (Attribute == GetMaxHealthAttribute())
    {
        // Make sure current health is not greater than the new max health.
        if (GetHealth() > NewValue)
        {
            UExtAbilitySystemComponent* ExtASC = Cast<UExtAbilitySystemComponent>(GetOwningAbilitySystemComponent());
            check(ExtASC);

            ExtASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
        }
    }
}

void UExtCharacterAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
    if (Attribute == GetHealthAttribute())
    {
        // Do not allow health to go negative or above max health.
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    else if (Attribute == GetMaxHealthAttribute())
    {
        // Do not allow max health to drop below 1.
        NewValue = FMath::Max(NewValue, 1.0f);
    }
}

UExtCharacterAttributeSet::UExtCharacterAttributeSet() :
	Health(60.0f),
    MaxHealth(60.0f),
    Shield(0.0f),
    MaxShield(50.0f),
    ShieldRegen(5.0f),
    InDamage(10.0f),
    Stamina(100.0f),
    MaxStamina(100.0f),
    MovementSpeed(400.0f),
    MaxMovementSpeed(1200.0f),
    AccelerationSpeed(2.0f)
{

}
