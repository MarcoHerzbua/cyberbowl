// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Actors/IFreezeable.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/Abilities/AbilityBase.h"
#include "CyberbowlCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCallErrorFeedback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBallCamToggled);

UCLASS(config=Game)
class ACyberbowlCharacter : public ACharacter, public IFreezeable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ACyberbowlCharacter();
	ACyberbowlCharacter(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void CallMenuEnter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float TurnRateModifier = 1.f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float LookUpRateModifier = 1.f;

	UPROPERTY(BlueprintReadWrite)
	bool bTurretMode;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTargetingAbility;

	UPROPERTY()
	float DefaultGravityScale;
	
	UPROPERTY()
	float DefaultTimeDilation;

	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnBallCamToggled OnToggledBallCam;
	
protected:
	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnCallErrorFeedback forceFeedback;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Dash();

	void AbilityPressed();

	void AbilityCanceled();

	void CallOnBallCamToggled();
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	void Jump() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IFreezable")
	void Freeze_Implementation(AActor* instigtr) override;

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "IFreezable")
	void UnFreeze_Implementation() override;

	//UFUNCTION(BlueprintCallable, Category = "CyberbowlCharacter")
	//void ToggleAbilities(bool enable);
};

