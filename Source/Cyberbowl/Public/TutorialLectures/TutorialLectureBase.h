// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/TutorialWidgetBase.h"
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

	UFUNCTION(BlueprintCallable)
	void AdvanceLecture();

	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnLectureFinished OnLectureFinished;

protected:
	virtual void BeginPlay() override;

	virtual void SetupTasks();

	virtual void AdvanceIfCurrentTask(const FString& performedTask);

	TQueue<FString> lectureTasks;
	FString currentTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UTutorialWidgetBase>> widgetsList;

	UPROPERTY()
	UTutorialWidgetBase* currentWidget;

	ATutorialPlayerController* tutorialPlayerController;

	ATutorialGameMode* tutorialGameMode;

};
