// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/GoalLecture.h"

#include "Kismet/GameplayStatics.h"
#include "Stadium/Goal_Collider.h"

void AGoalLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoalLecture::Enter()
{
	Super::Enter();
}

void AGoalLecture::Exit()
{
	Super::Exit();
}

void AGoalLecture::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> goalColliders;
	UGameplayStatics::GetAllActorsOfClass(this, AGoal_Collider::StaticClass(), goalColliders);

	for (auto goal : goalColliders)
	{
		Cast<AGoal_Collider>(goal)->OnGoalScored.AddDynamic(this, &AGoalLecture::OnGoalScored);
	}
}

void AGoalLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskScoredGoal);
}

void AGoalLecture::OnGoalScored(int teamIndex)
{
	AdvanceIfCurrentTask(taskScoredGoal);
}
