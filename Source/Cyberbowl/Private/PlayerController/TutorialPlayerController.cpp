// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/TutorialPlayerController.h"

#include "Character/CyberbowlCharacter.h"
#include "Components/WidgetComponent.h"

void ATutorialPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("AdvanceTutorial", IE_Pressed, this, &ATutorialPlayerController::CallOnAdvanceTutorial);
}

void ATutorialPlayerController::CallOnAdvanceTutorial()
{
	OnAdvanceTutorial.Broadcast();
}

void ATutorialPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto widgets = Cast<ACyberbowlCharacter>(GetCharacter())->GetComponentsByClass(UWidgetComponent::StaticClass());

	for (auto widget : widgets)
	{
		widget->DestroyComponent();
	}
}
