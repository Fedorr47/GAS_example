// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS_CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"

void UGAS_CharacterAttributeSet::ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const
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

void UGAS_CharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
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
}

void UGAS_CharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

UGAS_CharacterAttributeSet::UGAS_CharacterAttributeSet() :
	Health(60.0f), MaxHealth(60.0f), Shield(0.0f), MaxShield(50.0f), ShieldRegen(5.0f)
{
}
