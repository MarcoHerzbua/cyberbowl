// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/BallCamLecture.h"
#include "Character/CyberbowlCharacter.h"
#include "PlayerController/TutorialPlayerController.h"

void ABallCamLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABallCamLecture::Enter()
{
	Super::Enter();
}

void ABallCamLecture::Exit()
{
	Super::Exit();
}

void ABallCamLecture::BeginPlay()
{
	Super::BeginPlay();

	tutorialCharacter->OnToggledBallCam.AddDynamic(this, &ABallCamLecture::OnBallCamToggled);
}

void ABallCamLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskBallCamToggled);
}

void ABallCamLecture::OnBallCamToggled()
{
	AdvanceIfCurrentTask(taskBallCamToggled);
}
