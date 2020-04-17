// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModesAndInstances/TutorialGameMode.h"
#include "TutorialLectures/TutorialLectureBase.h"

void ATutorialGameMode::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;

	AdvanceTutorial();
}

void ATutorialGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATutorialGameMode::AdvanceTutorial()
{
	if (lecturesList.Num() <= 0)
	{
		OnTutorialFinished.Broadcast();
		return;
	}

	if (currentLecture)
	{
		currentLecture->Exit();
	}
	
	currentLecture = Cast<ATutorialLectureBase>(GetWorld()->SpawnActor(lecturesList[0]));
	currentLecture->SetFolderPath("TutorialLectures");

	currentLecture->OnLectureFinished.AddDynamic(this, &ATutorialGameMode::AdvanceTutorial);
	currentLecture->Enter();
	
	lecturesList.RemoveAt(0);
}
