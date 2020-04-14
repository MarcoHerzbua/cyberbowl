// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TutorialState.generated.h"

UCLASS()
class CYBERBOWL_API UTutorialState : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void EnterState() {}
	
	UFUNCTION()
	virtual void UpdateState(float deltaSeconds) {}

	UFUNCTION()
	virtual void ExitState() {}

};
