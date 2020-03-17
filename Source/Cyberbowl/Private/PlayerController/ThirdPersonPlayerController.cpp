// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ThirdPersonPlayerController.h"
#include "GameModesAndInstances/CyberbowlGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include "Widgets/WNameTag.h"
#include "Character/CyberbowlCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnActors();
	
	SetupNameTagWidgets();
}

void AThirdPersonPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateNameTagPositions();
}

void AThirdPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MenuNavigationDown", IE_Pressed, this, &AThirdPersonPlayerController::CallGameOverMenuNavigated);
	InputComponent->BindAction("MenuNavigationUp", IE_Pressed, this, &AThirdPersonPlayerController::CallGameOverMenuNavigated);
	InputComponent->BindAction("ToggleBallCam", IE_Pressed, this, &AThirdPersonPlayerController::CallToggledBallCam);
}

void AThirdPersonPlayerController::SpawnActors()
{
	UCyberbowlGameInstance* gameInstance = Cast<UCyberbowlGameInstance>(GetGameInstance());
	TMap<int, FPlayerInfo> playerInfo = gameInstance->PlayerInfo;
	FPlayerInfo* currPlayerInfo = playerInfo.Find(UGameplayStatics::GetPlayerControllerID(this));

	//For Debug Purposes Only
	//Todo: Delete later on!
	if (!currPlayerInfo)
	{
		FPlayerInfo currPlayerInfoStruct;
		currPlayerInfo = &currPlayerInfoStruct;
		currPlayerInfo->Team = 1;
		currPlayerInfo->CharacterType = ECBCharacterType::CBCHRACTERTYPE_FIRE;
	}
	
	currPlayerTeam = currPlayerInfo->Team;
	currPlayerType = currPlayerInfo->CharacterType;
	
	TArray<AActor*> playerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);
	APlayerStart* playerSpawn = nullptr;
	
	for (AActor* currPlayerStart : playerStarts)
	{
		playerSpawn = Cast<APlayerStart>(currPlayerStart);
		int playerTeam = FCString::Atoi(*playerSpawn->PlayerStartTag.ToString());
		if (playerTeam == currPlayerTeam)
		{
			int idx = playerStarts.Find(currPlayerStart);
			playerStarts.RemoveAt(idx);
			break;
		}
	}

	character = nullptr;
	spawnTransform = playerSpawn->GetTransform().GetLocation();
	spawnRotation = playerSpawn->GetActorRotation();
	
	switch (currPlayerType)
	{
	case ECBCharacterType::CBCHRACTERTYPE_FIRE:
		character = GetWorld()->SpawnActor<ACyberbowlCharacter>(fireClass, spawnTransform, spawnRotation);
		break;

	case ECBCharacterType::CBCHRACTERTYPE_EARTH:
		character = GetWorld()->SpawnActor<ACyberbowlCharacter>(earthClass, spawnTransform, spawnRotation);
		break;

	case ECBCharacterType::CBCHRACTERTYPE_ICE:
		character = GetWorld()->SpawnActor<ACyberbowlCharacter>(iceClass, spawnTransform, spawnRotation);
		break;

	case ECBCharacterType::CBCHRACTERTYPE_AIR:
		character = GetWorld()->SpawnActor<ACyberbowlCharacter>(airClass, spawnTransform, spawnRotation);
		break;
	}

	Possess(character);

	UUserWidget* baseWidget = CreateWidget<UUserWidget>(this, baseHudClass);
	baseWidget->AddToPlayerScreen();

	AInGameGameMode* gameMode = Cast<AInGameGameMode>(GetWorld()->GetAuthGameMode());

	gameMode->StartGamePlay.AddDynamic(this, &AThirdPersonPlayerController::OnStartGamePlay);
	gameMode->PauseGamePlay.AddDynamic(this, &AThirdPersonPlayerController::OnPauseGamePlay);
	gameMode->Regroup.AddDynamic(this, &AThirdPersonPlayerController::OnRegroup);
	gameMode->EndGame.AddDynamic(this, &AThirdPersonPlayerController::OnEndGame);
}

void AThirdPersonPlayerController::SetupNameTagWidgets()
{
	const int ownPlayerIndex = UGameplayStatics::GetPlayerControllerID(this);
	TArray<AActor*> playerCharacters;
	UGameplayStatics::GetAllActorsOfClass(this, ACyberbowlCharacter::StaticClass(), playerCharacters);

	for (auto playerCharacter : playerCharacters)
	{
		const auto cyberbowlCharacter = Cast<ACyberbowlCharacter>(playerCharacter);
		const int playerIndex = UGameplayStatics::GetPlayerControllerID(Cast<AThirdPersonPlayerController>(cyberbowlCharacter->GetController()));

		if (playerIndex != ownPlayerIndex)
		{
			charactersMap.Add(playerIndex, cyberbowlCharacter);
		}
	}

	auto gameInstance = Cast<UCyberbowlGameInstance>(GetGameInstance());
	
	for(const auto indexCharacterPair : charactersMap)
	{
		const auto nameTagWidget = Cast<UWNameTag>(CreateWidget(this, nameTagWidgetClass));
		const auto playerInfo = gameInstance->PlayerInfo.Find(indexCharacterPair.Key);
		
		nameTagWidget->CharacterName = ToCharacterName(playerInfo->CharacterType);
		nameTagWidget->PlayerIndex = indexCharacterPair.Key;

		auto background = Cast<UButton>(nameTagWidget->GetWidgetFromName("TeamColorButton"));
		if (playerInfo->Team == 1)
		{
			background->SetBackgroundColor(FLinearColor(0, 0.15, 0.55, 0.5));
		}
		else
		{
			background->SetBackgroundColor(FLinearColor(0.9, 0.3, 0, 0.5));
		}
		
		nameTagWidgets.AddUnique(nameTagWidget);
		nameTagWidget->AddToPlayerScreen();
	}
}

void AThirdPersonPlayerController::UpdateNameTagPositions()
{
	for (auto nameTagWidget : nameTagWidgets)
	{
		auto playerCharacter = charactersMap[nameTagWidget->PlayerIndex];
		const int playerIndex = UGameplayStatics::GetPlayerControllerID(Cast<AThirdPersonPlayerController>(playerCharacter->GetController()));

		if (playerIndex == nameTagWidget->PlayerIndex)
		{
			auto widgetComponent = Cast<UWidgetComponent>(playerCharacter->GetComponentByClass(UWidgetComponent::StaticClass()));
			FVector2D screenCoords;
			ProjectWorldLocationToScreen(widgetComponent->GetComponentLocation(), screenCoords);

			nameTagWidget->SetPositionInViewport(screenCoords);
		}
	}
}

void AThirdPersonPlayerController::OnStartGamePlay()
{
	character->EnableInput(this);
}

void AThirdPersonPlayerController::OnPauseGamePlay()
{
	character->DisableInput(this);
}

void AThirdPersonPlayerController::OnRegroup()
{
	character->SetActorLocation(spawnTransform);
	character->SetActorRotation(spawnRotation);
}

void AThirdPersonPlayerController::OnEndGame()
{
	character->DisableInput(this);
}

void AThirdPersonPlayerController::CallGameOverMenuNavigated()
{
	OnCallGameOverMenuNavigated.Broadcast();
}

void AThirdPersonPlayerController::CallToggledBallCam()
{
	OnCallToggledBallCam.Broadcast();
}
