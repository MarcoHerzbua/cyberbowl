// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CyberbowlCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CYBERBOWL_API UCyberbowlCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	public:

		virtual void NativeInitializeAnimation() override;

		UFUNCTION(blueprintCallable)
		void UpdateAnimationProperties();

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		float movementSpeed;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		bool bisInAir;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		class APawn* pawn;

		UPROPERTY(editAnywhere, BlueprintReadOnly)
		class ACyberbowlCharacter* main;
	};
