// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ExtBaseCharacter.h"
#include "Input/InputDataConfig.h" 
#include "ExtPlayerCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class GAS_EXAMPLE_API AExtPlayerCharacter : public AExtBaseCharacter
{
	GENERATED_BODY()

    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* Mesh1P;
	
    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    /** Config for avaiable input actions - create custom InputDataConfig object to insert here. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputDataConfig* InputActions;
    
public:
    // Sets default values for this character's properties
    AExtPlayerCharacter();

    USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Ext|Character")
    float GetMovementSpeed() const;

	virtual USkeletalMeshComponent* GetPresentMesh();

	void BindActionAbility(
		UInputDataConfig* InputDataConfig,
		UInputMappingContext* Context = nullptr);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    /** Called for looking input */
    void Look(const FInputActionValue& Value);

    /** Called for movement input */
    void Move(const FInputActionValue& Value);

    virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    void Input_AbilityInputTagPressed(FGameplayTag InputTag);
    void Input_AbilityInputTagReleased(FGameplayTag InputTag);
};
