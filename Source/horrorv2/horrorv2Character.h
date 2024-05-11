

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "horrorv2Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class USoundBase;



UCLASS(config=Game)
class Ahorrorv2Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Spring arm Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	/** First person camera */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	
public:
	Ahorrorv2Character();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;



	UPROPERTY(EditAnywhere, Blueprintable, Category = Pickups)
	int Pickups;



protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for sprinting input */
	void Sprint(const FInputActionValue& Value);
	void EndSprint();

	/** Movement speed for walking and sprinting */
	UPROPERTY(EditAnywhere,  Category = Sprint)
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere,  Category = Sprint)
	float SprintSpeed;

	bool bIsRunning;


	//* footsteps */
	// metasound
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* FootstepSound;

	FTimerHandle FootstepTimer;
	void SetFootstepDelay();
	UPROPERTY(EditAnywhere, Category = Sound)
	float WalkingFootstepDelay;
	UPROPERTY(EditAnywhere, Category = Sound)
	float RunningFootstepDelay;
	
	float FootstepDelay;
	bool bCanPlayFootstepSound;
	void PlayFootstepSound();
	float LastFootstepTime;
	/** Stamina */
	void UpdateStamina();
	void OutOfStamina();

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Sprint)
	float MaxStamina;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Sprint)
	float CurrentStamina;
	UPROPERTY(EditAnywhere, Category = Sprint)
	float StaminaDrainRate;
	UPROPERTY(EditAnywhere, Category = Sprint)
	float StaminaRegenRate;
	UPROPERTY(EditAnywhere, Category = Sprint)
	float DelayBeforeRefill;

	float CurrentRefillDelayTime;
	bool bHasStamina;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

