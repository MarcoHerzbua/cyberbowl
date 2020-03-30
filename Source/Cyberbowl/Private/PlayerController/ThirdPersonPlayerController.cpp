// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/ThirdPersonPlayerController.h"
#include "GameModesAndInstances/CyberbowlGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include "Character/CyberbowlCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include <string>

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetAsLocalPlayerController();
	SpawnActors();
}

void AThirdPersonPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateNameTagWidgetRotations();
}

void AThirdPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MenuNavigationDown", IE_Pressed, this, &AThirdPersonPlayerController::CallGameOverMenuNavigated);
	InputComponent->BindAction("MenuNavigationUp", IE_Pressed, this, &AThirdPersonPlayerController::CallGameOverMenuNavigated);
	InputComponent->BindAction("ToggleBallCam", IE_Pressed, this, &AThirdPersonPlayerController::CallToggledBallCam);
	InputComponent->BindAction("PauseGame", IE_Pressed, this, &AThirdPersonPlayerController::CallPlayerPausedGame);
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
	TArray<UWidgetComponent*, FDefaultAllocator> widgetComponents;
	character->GetComponents<UWidgetComponent, FDefaultAllocator>(widgetComponents);

	TArray<AActor*> playerControllers;
	UGameplayStatics::GetAllActorsOfClass(this, AThirdPersonPlayerController::StaticClass(), playerControllers);

	for (auto controller : playerControllers)
	{
		auto playerController = Cast<AThirdPersonPlayerController>(controller);

		if (playerController == this)
			continue;
		
		UWidgetComponent* widgetComponent = widgetComponents.Pop();
		UWNameTag* nameTagWidget = Cast<UWNameTag>(widgetComponent->GetUserWidgetObject());
		UButton* nameplate = Cast<UButton>(nameTagWidget->GetWidgetFromName("TeamColorButton"));
		nameTagWidget->CharacterName = ToCharacterName(currPlayerType);	

		if (currPlayerTeam == 1)
		{
			nameplate->SetBackgroundColor(FLinearColor(0.9, 0.3, 0, 0.5));
		}
		else
		{
			nameplate->SetBackgroundColor(FLinearColor(0, 0.15, 0.55, 0.5));
		}

		//if (playerController != this)
		//{
			widgetComponent->SetOwnerPlayer(playerController->GetLocalPlayer());
			nameTagWidget->SetOwningPlayer(playerController);
			nameTagWidget->SetOwningLocalPlayer(playerController->GetLocalPlayer());
			nameTagWidget->IsAssigned = true;
		//}
	}

	for (auto widgetComp : widgetComponents)
	{	
		bool isAssigned = Cast<UWNameTag>(widgetComp->GetUserWidgetObject())->IsAssigned;
		if (!isAssigned)
		{
			widgetComp->DestroyComponent();
			UKismetSystemLibrary::PrintString(this, "deleted widget component");
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

void AThirdPersonPlayerController::CallPlayerPausedGame()
{
	OnPlayerPausedGame.Broadcast(UGameplayStatics::GetPlayerControllerID(this));
}

void AThirdPersonPlayerController::UpdateNameTagWidgetRotations()
{
	
}
