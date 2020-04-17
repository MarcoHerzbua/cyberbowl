// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialLectures/TutorialLectureBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/TutorialPlayerController.h"
#include "GameModesAndInstances/TutorialGameMode.h"

ATutorialLectureBase::ATutorialLectureBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATutorialLectureBase::BeginPlay()
{
	Super::BeginPlay();

	tutorialPlayerController = Cast<ATutorialPlayerController>(UGameplayStatics::GetPlayerControllerFromID(this, 0));
	tutorialGameMode = Cast<ATutorialGameMode>(UGameplayStatics::GetGameMode(this));

	tutorialPlayerController->OnAdvanceTutorial.AddDynamic(this, &ATutorialLectureBase::AdvanceLecture);
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
	if (widgetsList. Num() <= 0)
	{
		OnLectureFinished.Broadcast();
		return;
	}

	if (currentWidget)
	{
		currentWidget->RemoveFromParent();
	}
	
	currentWidget = CreateWidget(tutorialPlayerController, widgetsList[0]);
	currentWidget->AddToViewport();

	widgetsList.RemoveAt(0);
}

void ATutorialLectureBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
