// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModesAndInstances/CyberbowlGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/ThirdPersonPlayerController.h"

void UCyberbowlGameInstance::CheckAllCharactersSpawned() const
{
	if (SpawnedCharacters == TotalPlayers)
	{
		TArray<AActor*> playerControllers;
		UGameplayStatics::GetAllActorsOfClass(this, AThirdPersonPlayerController::StaticClass(), playerControllers);

		for (auto controller : playerControllers)
		{
			Cast<AThirdPersonPlayerController>(controller)->SetupNameTagWidgets();
		}
	}
}
