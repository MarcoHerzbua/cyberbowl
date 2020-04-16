// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/TutorialPlayerController.h"

void ATutorialPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("AdvanceTutorial", IE_Pressed, this, &ATutorialPlayerController::CallOnAdvanceTutorial);
}

void ATutorialPlayerController::CallOnAdvanceTutorial()
{
	OnAdvanceTutorial.Broadcast();
}
