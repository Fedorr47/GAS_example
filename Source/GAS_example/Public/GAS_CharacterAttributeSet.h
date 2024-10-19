// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS_BaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GAS_CharacterAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FDamageTakenEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayTagContainer& /*GameplayTagContainer*/, float /*Damage*/);

/**
 * 
 */
UCLASS()
class GAS_EXAMPLE_API UGAS_CharacterAttributeSet : public UGAS_BaseAttributeSet
{
	GENERATED_BODY()
public:

    UGAS_CharacterAttributeSet();

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UGAS_CharacterAttributeSet, Health);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UGAS_CharacterAttributeSet, MaxHealth);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Shield;
    ATTRIBUTE_ACCESSORS(UGAS_CharacterAttributeSet, Shield);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxShield;
    ATTRIBUTE_ACCESSORS(UGAS_CharacterAttributeSet, MaxShield);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData ShieldRegen;
    ATTRIBUTE_ACCESSORS(UGAS_CharacterAttributeSet, ShieldRegen);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData InDamage;
    ATTRIBUTE_ACCESSORS(UGAS_CharacterAttributeSet, InDamage);

    mutable FDamageTakenEvent OnDamageTaken;

protected:
    virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

};
