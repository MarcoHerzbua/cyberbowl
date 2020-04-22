// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/TutorialPlayerController.h"

#include "Character/CyberbowlCharacter.h"
#include "Components/WidgetComponent.h"

ACyberbowlCharacter* ATutorialPlayerController::SwitchCharacterClass(TSubclassOf<ACyberbowlCharacter> newCharacterClass)
{
	auto newCharacter = Cast<ACyberbowlCharacter>(GetWorld()->SpawnActor<AActor>(newCharacterClass, GetCharacter()->GetTransform()));

	GetWorld()->DestroyActor(GetCharacter());

	Possess(newCharacter);

	TArray<UActorComponent*> widgetComponents;
	Cast<ACyberbowlCharacter>(GetCharacter())->GetComponents(UWidgetComponent::StaticClass(), widgetComponents);

	for (auto widget : widgetComponents)
	{
		widget->DestroyComponent();
	}

	return newCharacter;
}

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

	TArray<UActorComponent*> widgetComponents;
	Cast<ACyberbowlCharacter>(GetCharacter())->GetComponents(UWidgetComponent::StaticClass(), widgetComponents);

	for (auto widget : widgetComponents)
	{
		widget->DestroyComponent();
	}
}
