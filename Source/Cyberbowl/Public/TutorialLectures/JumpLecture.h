// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TutorialLectures/TutorialLectureBase.h"
#include "JumpLecture.generated.h"

/**
 * 
 */
UCLASS()
class CYBERBOWL_API AJumpLecture : public ATutorialLectureBase
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
	FString taskJump = "taskJump";
	FString taskDoubleJump = "taskDoubleJump";
	
	UFUNCTION()
	void OnJump();

	UFUNCTION()
	void OnDoubleJump();
};
