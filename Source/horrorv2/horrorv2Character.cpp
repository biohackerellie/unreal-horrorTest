
#include "horrorv2Character.h"
#include "horrorv2Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"



//////////////////////////////////////////////////////////////////////////
// Ahorrorv2Character

Ahorrorv2Character::Ahorrorv2Character()
{

	PrimaryActorTick.bCanEverTick = true;
	

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);


	
		
	// Create a CameraComponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetupAttachment(SpringArmComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	WalkSpeed = 600.f;
	SprintSpeed = 1000.f;
	WalkingFootstepDelay = .5f;
	RunningFootstepDelay = .3f;
	FootstepDelay = WalkingFootstepDelay;
	Pickups = 0;

	MaxHealth = 100;
	

}

void Ahorrorv2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	Health = MaxHealth;
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
}


void Ahorrorv2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateStamina();
}

//////////////////////////////////////////////////////////////////////////// Input

void Ahorrorv2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Ahorrorv2Character::Move);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &Ahorrorv2Character::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &Ahorrorv2Character::EndSprint);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Ahorrorv2Character::Look);


	}
	else
	{
		// log an error
		UE_LOG(LogTemp, Error, TEXT("Ahorrorv2Character::SetupPlayerInputComponent: PlayerInputComponent is not an EnhancedInputComponent"));
	}
}


void Ahorrorv2Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		FVector playerVelocity = GetVelocity();
		
		// play footstep sound
		if (playerVelocity.Z == 0.0f && playerVelocity.SizeSquared() > 4.0f)
		{
			float CurrentTime = GetWorld()->GetTimeSeconds();
			if (CurrentTime - LastFootstepTime > FootstepDelay)
			{
				LastFootstepTime = CurrentTime;
			PlayFootstepSound();
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(FootstepTimer);
			}
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(FootstepTimer);
		}
	}
}

void Ahorrorv2Character::Look(const FInputActionValue& Value)
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

void Ahorrorv2Character::Sprint(const FInputActionValue& Value)
{
	if(bHasStamina)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		if(GetVelocity().Size() >= 0.1f)
		{
			FootstepDelay = RunningFootstepDelay;
			bIsRunning = true;
		}
		else
		{
			bIsRunning = false;
		}
	}
}

void Ahorrorv2Character::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsRunning = false;
	FootstepDelay = WalkingFootstepDelay;
	
}




void Ahorrorv2Character::SetFootstepDelay()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if(!TimerManager.IsTimerActive(FootstepTimer))
	{
		TimerManager.SetTimer(FootstepTimer, this, &Ahorrorv2Character::PlayFootstepSound, FootstepDelay, false);
	}
}

void Ahorrorv2Character::PlayFootstepSound()
{
	if(FootstepSound)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		PlayerPawn->MakeNoise(1, this, GetActorLocation());
		FVector FootLocation = GetActorLocation();
		FootLocation.Z -= 44.f;
		UGameplayStatics::PlaySoundAtLocation(this, FootstepSound, FootLocation);
		
	}

}


void Ahorrorv2Character::UpdateStamina()
{
	//Drain Stamina
	if(bIsRunning)
	{
		CurrentStamina -= StaminaDrainRate;
		CurrentRefillDelayTime = DelayBeforeRefill;
	}
	if(!bIsRunning && CurrentStamina < MaxStamina)
	{
		CurrentRefillDelayTime--;
		if(CurrentRefillDelayTime <= 0)
		{
			CurrentStamina += StaminaRegenRate;
		}
	}
	if(CurrentStamina <= 0)
	{
		bHasStamina = false;
		EndSprint();
	}
	else
	{
		bHasStamina = true;
	}
}

void Ahorrorv2Character::OutOfStamina()
{

}

