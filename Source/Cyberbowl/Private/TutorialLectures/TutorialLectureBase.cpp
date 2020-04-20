// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialLectures/TutorialLectureBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/TutorialPlayerController.h"
#include "Character/CyberbowlCharacter.h"
#include "GameModesAndInstances/TutorialGameMode.h"

ATutorialLectureBase::ATutorialLectureBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATutorialLectureBase::BeginPlay()
{
	Super::BeginPlay();

	SetupTasks();

	tutorialPlayerController = Cast<ATutorialPlayerController>(UGameplayStatics::GetPlayerControllerFromID(this, 0));
	tutorialCharacter = Cast<ACyberbowlCharacter>(tutorialPlayerController->GetPawn());
	tutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(this));

	// Debug purposes
	// ToDo: Remove
	tutorialPlayerController->OnAdvanceTutorial.AddDynamic(this, &ATutorialLectureBase::AdvanceLecture);
}

void ATutorialLectureBase::SetupTasks()
{
}

void ATutorialLectureBase::AdvanceIfCurrentTask(const FString& performedTask)
{
	if (currentTask == performedTask)
	{
		AdvanceLecture();
	}
}

void ATutorialLectureBase::Enter()
{
	AdvanceLecture();
}

void ATutorialLectureBase::Exit()
{
	currentWidget->RemoveFromParent();
	widgetsList.Empty();
}

void ATutorialLectureBase::AdvanceLecture()
{
	if (lectureTasks.IsEmpty())
	{
		OnLectureFinished.Broadcast();
		return;
	}

	lectureTasks.Dequeue(currentTask);
	
	if (currentWidget)
	{
		currentWidget->RemoveFromParent();
	}
	
	currentWidget = Cast<UTutorialWidgetBase>(CreateWidget(tutorialPlayerController, widgetsList[0]));
	currentWidget->SetTutorialLecture(this);
	currentWidget->AddToViewport();

	widgetsList.RemoveAt(0);
}

void ATutorialLectureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
