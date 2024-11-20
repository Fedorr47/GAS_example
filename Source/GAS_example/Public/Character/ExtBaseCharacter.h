// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "ExtBaseCharacter.generated.h"

class UExtGameplayAbility;
class UExtCharacterAttributeSet;
class UExtAbilitySystemComponent;
class UExtAbilitySet;

UCLASS()
class GAS_EXAMPLE_API AExtBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AExtBaseCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category="Ext|Abilities")
	UExtAbilitySystemComponent* ExtAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<const UExtCharacterAttributeSet> CharacterAttributesSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ext|Abilities")
	TArray<TObjectPtr<UExtAbilitySet>> AbilitySets;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultEffects;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void InitializeAbilitySystem();

	virtual void PostInitializeComponents() override;

	virtual void OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);
	virtual void OnAccelerationSpeedChanged(AActor* DamageInstigator, AActor* DamageCauser,
	                                const FGameplayTagContainer& GameplayTagContainer, float AcceleratePower);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnDamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnAcceleratePerformed(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float AcceleratePower);

	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnHealthChanged(float OldValue, float NewValue);

	virtual void OnShieldAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnShieldChanged(float OldValue, float NewValue);
};
