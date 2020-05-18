// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/ThirdPersonPlayerController.h"

#include <string>

#include "GameModesAndInstances/CyberbowlGameInstance.h"
#include "Character/BallCamComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameModesAndInstances/InGameGameMode.h"
#include "Character/CyberbowlCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/Button.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/WNameTag.h"

void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetAsLocalPlayerController();
	SpawnActors();
	//Disable movement til the countdown when starting the game ends.
	OnPauseGamePlay();
}

void AThirdPersonPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateNametagPositions();
}

void AThirdPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MenuNavigationDown", IE_Pressed, this, &AThirdPersonPlayerController::CallGameOverMenuNavigated);
	InputComponent->BindAction("MenuNavigationUp", IE_Pressed, this, &AThirdPersonPlayerController::CallGameOverMenuNavigated);
	InputComponent->BindAction("PauseGame", IE_Pressed, this, &AThirdPersonPlayerController::CallPlayerPausedGame);
	InputComponent->BindAction("MenuNavigationDown", IE_Pressed, this, &AThirdPersonPlayerController::CallMenuNavigationDown);
	InputComponent->BindAction("MenuNavigationUp", IE_Pressed, this, &AThirdPersonPlayerController::CallMenuNavigationUp);
	InputComponent->BindAction("MenuNavigationRight", IE_Pressed, this, &AThirdPersonPlayerController::CallMenuNavigationRight);
	InputComponent->BindAction("MenuNavigationLeft", IE_Pressed, this, &AThirdPersonPlayerController::CallMenuNavigationLeft);
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
	savedPlayerStarts = playerStarts;
	
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
	FVector spawnTransform = playerSpawn->GetTransform().GetLocation();
	FRotator spawnRotation = playerSpawn->GetActorRotation();
	
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

	// We need to re-activate the components, since they are being deactivated based on the player count last round
	for (auto widgetComp : widgetComponents)
	{
		widgetComp->Activate();
	}

	TArray<AActor*> playerControllers;
	UGameplayStatics::GetAllActorsOfClass(this, AThirdPersonPlayerController::StaticClass(), playerControllers);

	for (auto controller : playerControllers)
	{
		const auto playerController = Cast<AThirdPersonPlayerController>(controller);

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

		widgetComponent->SetOwnerPlayer(playerController->GetLocalPlayer());
		playerController->AddNametagWidgetForPlayer(widgetComponent);
		nameTagWidget->SetOwningPlayer(playerController);
		nameTagWidget->SetOwningLocalPlayer(playerController->GetLocalPlayer());
		nameTagWidget->IsAssigned = true;
	}

	for (auto widgetComp : widgetComponents)
	{
		const bool isAssigned = Cast<UWNameTag>(widgetComp->GetUserWidgetObject())->IsAssigned;
		if (!isAssigned)
		{
			widgetComp->Deactivate();
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
	Algo::Reverse(savedPlayerStarts);
	TArray<AActor*> playerStarts = savedPlayerStarts;

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

	FVector spawnTransform = playerSpawn->GetTransform().GetLocation();
	FRotator spawnRotation = playerSpawn->GetActorRotation();
	character->SetActorLocation(spawnTransform);
	character->SetActorRotation(spawnRotation);
	UBallCamComponent* ballCam = Cast<UBallCamComponent>(character->FindComponentByClass<UBallCamComponent>());
	ballCam->DoNotFollow();

	character->GetController()->SetControlRotation(spawnRotation);
}

void AThirdPersonPlayerController::OnEndGame()
{
	character->DisableInput(this);
}

void AThirdPersonPlayerController::CallGameOverMenuNavigated()
{
	OnCallGameOverMenuNavigated.Broadcast();
}

void AThirdPersonPlayerController::CallPlayerPausedGame()
{
	OnPlayerPausedGame.Broadcast(UGameplayStatics::GetPlayerControllerID(this));
}

void AThirdPersonPlayerController::CallMenuNavigationDown()
{
	if (Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(this))->GetIsPaused())
	{
		OnMenuNavigatedDown.Broadcast();
	}
}

void AThirdPersonPlayerController::CallMenuNavigationUp()
{
	if (Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(this))->GetIsPaused())
	{
		OnMenuNavigatedUp.Broadcast();
	}
}

void AThirdPersonPlayerController::CallMenuNavigationLeft()
{
	if (Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(this))->GetIsPaused())
	{
		OnMenuNavigatedLeft.Broadcast();
	}
}

void AThirdPersonPlayerController::CallMenuNavigationRight()
{
	if (Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(this))->GetIsPaused())
	{
		OnMenuNavigatedRight.Broadcast();
	}
}

void AThirdPersonPlayerController::UpdateNametagPositions()
{
	for (auto nametag : otherPlayerNametags)
	{
		const auto distanceToOtherPlayer = UKismetMathLibrary::Vector_Distance(character->GetActorLocation(), nametag->GetComponentLocation());
		const float heightAdjustment = (distanceToOtherPlayer - MinPlayerDistance) * (MaxZWidgetPos - MinZWidgetPos) / (MaxPlayerDistance - MinPlayerDistance) + MinZWidgetPos;
		
		nametag->SetRelativeLocation(FVector(0, 0, heightAdjustment));
	}
}

void AThirdPersonPlayerController::CallMenuEnter()
{
	// Only Player0 can press buttons apparently, so we have to manually handle it for the others
	if (Cast<AInGameGameMode>(UGameplayStatics::GetGameMode(this))->GetIsPaused() && UGameplayStatics::GetPlayerControllerID(this) != 0)
	{
		OnMenuEnter.Broadcast();
	}
}

void AThirdPersonPlayerController::AddNametagWidgetForPlayer(UWidgetComponent* nametagWidget)
{
	otherPlayerNametags.AddUnique(nametagWidget);
}