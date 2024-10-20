// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "GAS_Ability.h"
#include "GAS_CharacterAttributeSet.h"

// Sets default values
AGAS_BaseCharacter::AGAS_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");

	CharacterSet = CreateDefaultSubobject<UGAS_CharacterAttributeSet>("HealthSet");

}

// Called when the game starts or when spawned
void AGAS_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterSet->OnDamageTaken.AddUObject(this, &AGAS_BaseCharacter::OnDamageTakenChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterSet->GetHealthAttribute()).AddUObject(this, &AGAS_BaseCharacter::OnHealthAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterSet->GetShieldAttribute()).AddUObject(this, &AGAS_BaseCharacter::OnShieldAttributeChanged);
	
}

UAbilitySystemComponent* AGAS_BaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGAS_BaseCharacter::InitializeAbilities()
{
    // Give Abilities, Server only
    if (!HasAuthority() || !AbilitySystemComponent)
        return;

    for (TSubclassOf<UGAS_Ability>& Ability : DefaultAbilities)
    {
        FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetAbilityInputID()), this));
    }
}

void AGAS_BaseCharacter::InitializeAttributes()
{
}

void AGAS_BaseCharacter::InitializeEffects()
{
    if (!AbilitySystemComponent)
        return;

    FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
    {
        FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
        if (SpecHandle.IsValid())
        {
            FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void AGAS_BaseCharacter::OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage)
{
    OnDamageTaken(DamageInstigator, DamageCauser, GameplayTagContainer, Damage);
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

    if (!AbilitySystemComponent)
        return;

    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    InitializeEffects();
    InitializeAbilities();
}


