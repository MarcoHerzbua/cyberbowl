// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/JumpLecture.h"
#include "Character/CyberbowlCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void AJumpLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJumpLecture::Enter()
{
	Super::Enter();
}

void AJumpLecture::Exit()
{
	Super::Exit();
}

void AJumpLecture::BeginPlay()
{
	Super::BeginPlay();

	tutorialCharacter->OnJump.AddDynamic(this, &AJumpLecture::OnJump);
	tutorialCharacter->OnDoubleJump.AddDynamic(this, &AJumpLecture::OnDoubleJump);
}

void AJumpLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskJump);
	lectureTasks.Enqueue(taskDoubleJump);
}

void AJumpLecture::OnJump()
{
	AdvanceIfCurrentTask(taskJump);
}

void AJumpLecture::OnDoubleJump()
{
	AdvanceIfCurrentTask(taskDoubleJump);
}
