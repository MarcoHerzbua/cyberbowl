// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/DashLecture.h"

#include "Character/CBCharacterMovementComponent.h"
#include "Character/CyberbowlCharacter.h"
#include "Character/CyberbowlCharacterAnimInstance.h"

void ADashLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADashLecture::Enter()
{
	Super::Enter();
}

void ADashLecture::Exit()
{
	Super::Exit();
}

void ADashLecture::BeginPlay()
{
	Super::BeginPlay();

	tutorialCharacter->OnDash.AddDynamic(this, &ADashLecture::OnRegularDash);
	tutorialCharacter->OnVerticalDash.AddDynamic(this, &ADashLecture::OnVerticalDash);

	animInstance = Cast<UCBCharacterMovementComponent>(tutorialCharacter->GetComponentByClass(UCBCharacterMovementComponent::StaticClass()))->animinstance;
}

void ADashLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskRegularDash);
	lectureTasks.Enqueue(taskJumpDash);
	lectureTasks.Enqueue(taskVerticalDash);
}

void ADashLecture::OnRegularDash()
{
	AdvanceIfCurrentTask(taskRegularDash);

	if (animInstance->bisInAir)
	{
		AdvanceIfCurrentTask(taskJumpDash);
	}
}

void ADashLecture::OnVerticalDash()
{
	AdvanceIfCurrentTask(taskVerticalDash);
}
