// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/OutroLecture.h"

void AOutroLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOutroLecture::Enter()
{
	Super::Enter();
}

void AOutroLecture::Exit()
{
	Super::Exit();
}

void AOutroLecture::BeginPlay()
{
	Super::BeginPlay();

	SetupTasks();
}

void AOutroLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskPressedContinue);
}

void AOutroLecture::OnPressedContinue()
{
	if (currentTask != taskPressedContinue)
	{
		return;
	}

	AdvanceLecture();
}
