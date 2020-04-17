// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TutorialPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAdvanceTutorial);

UCLASS()
class CYBERBOWL_API ATutorialPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnAdvanceTutorial OnAdvanceTutorial;
	
protected:
	void SetupInputComponent() override;

	UFUNCTION()
	void CallOnAdvanceTutorial();
};
