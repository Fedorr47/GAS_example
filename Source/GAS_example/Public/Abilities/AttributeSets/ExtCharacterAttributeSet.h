// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExtBaseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ExtCharacterAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FDamageTakenEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayTagContainer& /*GameplayTagContainer*/, float /*Damage*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAccelerationSpeedEvent, AActor*  /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayTagContainer& /*GameplayTagContainer*/, float /*NewAccelerationSpeed*/);

/**
 * 
 */
UCLASS()
class GAS_EXAMPLE_API UExtCharacterAttributeSet : public UExtBaseAttributeSet
{
	GENERATED_BODY()
public:

    UExtCharacterAttributeSet();

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, Health);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, MaxHealth);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Shield;
    ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, Shield);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxShield;
    ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, MaxShield);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData ShieldRegen;
    ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, ShieldRegen);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData InDamage;
    ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, InDamage);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, Stamina);

    UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMovementSpeed;
	ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, MaxMovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AccelerationSpeed;
	ATTRIBUTE_ACCESSORS(UExtCharacterAttributeSet, AccelerationSpeed);
	

    mutable FDamageTakenEvent OnDamageTaken;
	mutable FAccelerationSpeedEvent OnAccelerationSpeed;

protected:
    virtual void ClampAttributeOnChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue);
	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
};
