// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/ExtPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "Abilities/ExtAbilitySystemComponent.h"
#include "Input//ExtEnhancedInputComponent.h"
#include "ExtGameplayTags.h"
#include "Abilities/AttributeSets/ExtCharacterAttributeSet.h"

AExtPlayerCharacter::AExtPlayerCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // ... at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 200.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

float AExtPlayerCharacter::GetMovementSpeed() const
{
    return IsValid(CharacterAttributesSet) ? CharacterAttributesSet->GetMovementSpeed() : 0.0f;
}

USkeletalMeshComponent* AExtPlayerCharacter::GetPresentMesh()
{
    return GetMesh1P();
}

void AExtPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AExtPlayerCharacter::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AExtPlayerCharacter::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AExtPlayerCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID)
{
    if (!ExtAbilitySystemComponent)
        return;

    if (Value.Get<bool>())
    {
        //ExtAbilitySystemComponent->AbilityLocalInputPressed(InputID);
    }
    else
    {
        //ExtAbilitySystemComponent->AbilityLocalInputReleased(InputID);
    }
}

void AExtPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Add Input Mapping Context
    UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (IsValid(Subsystem))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // Set up action bindings
    if (UExtEnhancedInputComponent* ExtIMC = Cast<UExtEnhancedInputComponent>(PlayerInputComponent))
    {
        if (!InputActions) return;
        
        if (ensureMsgf(ExtIMC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UExtEnhancedInputComponent or a subclass of it.")))
        {
            // Add the key mappings that may have been set by the player
            ExtIMC->AddInputMappings(InputActions, Subsystem);
            
            TArray<uint32> BindHandles;
            ExtIMC->BindAbilityActions(InputActions, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
            TArray<uint32> AdvancedBindHandles;
            ExtIMC->BindAdvancedAbilityActions(InputActions, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ AdvancedBindHandles);

            ExtIMC->BindNativeAction(InputActions, ExtGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Move, /*bLogIfNotFound=*/ false);
            ExtIMC->BindNativeAction(InputActions, ExtGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Look, /*bLogIfNotFound=*/ false);
        }
    }
}

void AExtPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (UExtAbilitySystemComponent* ExtASC = Cast<UExtAbilitySystemComponent>(GetAbilitySystemComponent()))
    {
        ExtASC->AbilityInputTagPressed(InputTag);
    }
}

void AExtPlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (UExtAbilitySystemComponent* ExtASC = Cast<UExtAbilitySystemComponent>(GetAbilitySystemComponent()))
    {
        ExtASC->AbilityInputTagReleased(InputTag);
    }
}

void AExtPlayerCharacter::BindActionAbility(
    FGameplayTag InputTag,
    UInputAction* Action,
    UInputMappingContext* Context)
{
    UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (IsValid(Subsystem) && Context != nullptr)
        {
            Subsystem->AddMappingContext(Context, 0);
        }
    }

    TArray<uint32> BindHandles;
    if (UExtEnhancedInputComponent* ExtIMC = Cast<UExtEnhancedInputComponent>(InputComponent))
    {
        ExtIMC->BindAbilityAction(
            Action,
            InputTag,
            this,
            &ThisClass::Input_AbilityInputTagPressed,
            &ThisClass::Input_AbilityInputTagReleased,
            BindHandles);
    }

    
}
