// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/IntroLecture.h"
#include "PlayerController/TutorialPlayerController.h"

void AIntroLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIntroLecture::Enter()
{
	Super::Enter();

	tutorialPlayerController->SetInputMode(FInputModeUIOnly());
}

void AIntroLecture::Exit()
{
	Super::Exit();

	tutorialPlayerController->SetInputMode(FInputModeGameAndUI());
}

void AIntroLecture::BeginPlay()
{
	Super::BeginPlay();

	SetupTasks();
}

void AIntroLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskPressedContinue);
}

void AIntroLecture::OnPressedContinue()
{
	if (currentTask != taskPressedContinue)
	{
		return;
	}

	AdvanceLecture();
}
