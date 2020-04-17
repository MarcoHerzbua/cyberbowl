// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialLectureBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLectureFinished);

class ATutorialPlayerController;
class ATutorialGameMode;

UCLASS(Abstract)
class CYBERBOWL_API ATutorialLectureBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATutorialLectureBase();

	virtual void Tick(float DeltaTime) override;

	virtual void Enter();

	virtual void Exit();

	UFUNCTION()
	virtual void AdvanceLecture();

	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnLectureFinished OnLectureFinished;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UUserWidget>> widgetsList;

	UPROPERTY()
	UUserWidget* currentWidget;

	ATutorialPlayerController* tutorialPlayerController;

	ATutorialGameMode* tutorialGameMode;
};
