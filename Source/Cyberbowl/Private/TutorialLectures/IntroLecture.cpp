// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/IntroLecture.h"

void AIntroLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIntroLecture::Enter()
{
	Super::Enter();
}

void AIntroLecture::Exit()
{
	Super::Exit();
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
