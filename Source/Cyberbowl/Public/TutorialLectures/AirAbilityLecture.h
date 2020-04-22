// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutorialLectures/TutorialLectureBase.h"
#include "AirAbilityLecture.generated.h"

class AGoal_Collider;
class APlayBall;

UCLASS()
class CYBERBOWL_API AAirAbilityLecture : public ATutorialLectureBase
{
	GENERATED_BODY()
	
public:
	void Tick(float DeltaTime) override;
	void Enter() override;
	void Exit() override;
	
protected:
	void BeginPlay() override;
	void SetupTasks() override;

	// Lecture tasks
	FString taskReadInstructions = "taskReadInstructions";
	FString taskGrabAndThrow = "taskGrabAndThrow";

	UFUNCTION(BlueprintCallable)
	void OnReadInstructions();

	UFUNCTION()
	void OnBallSucced();

	UFUNCTION()
	void OnGoalScored(int teamIndex);

	UFUNCTION(BlueprintCallable)
	void LaunchBall() const;

	APlayBall* ball;

	AActor* ballLaunchStartLocation;

	AActor* playerStartLocation;

	AGoal_Collider* goal;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACyberbowlCharacter> airCharacterClass;
};
