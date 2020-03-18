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
#include "Components/CapsuleComponent.h"
#include <string>

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnActors();
}

void AThirdPersonPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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

	gameInstance->SpawnedCharacters++;
	gameInstance->CheckAllCharactersSpawned();

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
	const int totalPlayers = Cast<UCyberbowlGameInstance>(GetGameInstance())->TotalPlayers;
	const int ownPlayerIndex = UGameplayStatics::GetPlayerControllerID(this);

	TArray<UWidgetComponent*, FDefaultAllocator> widgetComponents;
	character->GetComponents<UWidgetComponent, FDefaultAllocator>(widgetComponents);
	
	for (int i = 0; i < totalPlayers; i++)
	{
		if (i == ownPlayerIndex)
		{
			widgetComponents[i]->SetOwnerPlayer(this->GetLocalPlayer());
		}
		else
		{
			widgetComponents[i]->SetOwnerPlayer(UGameplayStatics::GetPlayerControllerFromID(this, 0)->GetLocalPlayer());
		}	
	}

	/*auto gameInstance = Cast<UCyberbowlGameInstance>(GetGameInstance());
	
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
	}*/
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
