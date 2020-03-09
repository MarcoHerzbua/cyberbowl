// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "MainMenuPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuPlayerController.h"
#include "WMainMenu.h"
#include "TimerManager.h"

void AMainMenuGameMode::BeginPlay()
{
	CreateMainMenu();
	
	SetupCharacterSelection();

	MainMenuWidget->LobbyEntered.AddDynamic(this, &AMainMenuGameMode::OnLobbyEntered);
}

void AMainMenuGameMode::Tick(float DeltaSeconds)
{
	if (bAllReady)
	{
		currentCooldown -= DeltaSeconds;
	}
}

void AMainMenuGameMode::OnLobbyEntered()
{
	CreatePlayers();

	TArray<AActor*> mainMenuPlayerControllers;
	UGameplayStatics::GetAllActorsOfClass(this, AMainMenuPlayerController::StaticClass(), mainMenuPlayerControllers);

	for (auto controller : mainMenuPlayerControllers)
	{
		auto mainMenuPlayerController = Cast<AMainMenuPlayerController>(controller);
		mainMenuPlayerController->GetLobbyWidget()->PlayerReadyUnready.AddDynamic(this, &AMainMenuGameMode::OnPlayerReadyUnready);
	}
}

void AMainMenuGameMode::OnPlayerReadyUnready()
{
	if (TotalPlayers != PlayersReady)
	{
		return;
	}

	bAllReady = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> countdownWidgetClassFinder(TEXT("/Game/UI/Countdown"));
	TSubclassOf<class UUserWidget> countdownWidgetClass = countdownWidgetClassFinder.Class;
	auto countdownWidget = CreateWidget(UGameplayStatics::GetPlayerControllerFromID(this, 0), countdownWidgetClass);
	countdownWidget->AddToViewport();

	GetWorld()->GetTimerManager().SetTimer(CountdownTimer, this, &AMainMenuGameMode::OpenArena , 3.f, false);
	UGameplayStatics::OpenLevel(this, FName(TEXT("CyberbowlArenaMap")));
}

void AMainMenuGameMode::OpenArena()
{
	UGameplayStatics::OpenLevel(this, FName(TEXT("CyberbowlArenaMap")));
}

void AMainMenuGameMode::CreateMainMenu()
{
	auto mainMenuControllerPlayer1 = Cast<AMainMenuPlayerController>(UGameplayStatics::GetPlayerControllerFromID(this, 0));

	// Background
	static ConstructorHelpers::FClassFinder<UUserWidget> mainMenuBackgroundWidgetClassFinder(TEXT("/Game/UI/W_MainMenuBackground"));
	TSubclassOf<class UUserWidget> mainMenuBackgroundWidgetClass = mainMenuBackgroundWidgetClassFinder.Class;
	CreateWidget(mainMenuControllerPlayer1, mainMenuBackgroundWidgetClass)->AddToViewport();

	// Main Menu
	MainMenuWidget = Cast<UWMainMenu>(CreateWidget(mainMenuControllerPlayer1, UWMainMenu::StaticClass()));
	MainMenuWidget->AddToViewport();

	mainMenuControllerPlayer1->PushToMenuStack(MainMenuWidget);
	FInputModeGameAndUI inputMode;
	inputMode.SetWidgetToFocus(MainMenuWidget->GetSlateWidgetFromName(FName(TEXT("Button_Start"))));
	mainMenuControllerPlayer1->SetInputMode(inputMode);
	mainMenuControllerPlayer1->bShowMouseCursor = false;
}

void AMainMenuGameMode::SetupCharacterSelection()
{
	auto mainMenuPlayerController = Cast<AMainMenuPlayerController>(UGameplayStatics::GetPlayerControllerFromID(this, 0));

	static ConstructorHelpers::FClassFinder<APawn> DummyCharacterFinder(TEXT("/Game/Characters/Dummy/DummyBase"));
	auto dummyClass = DummyCharacterFinder.Class;
	UGameplayStatics::GetAllActorsOfClass(this, dummyClass, CharacterPreviewDummies);

	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName(TEXT("SelectionCam")), CharacterSelectionCams);
}

void AMainMenuGameMode::CreatePlayers()
{
	for (int i = 1; i <=3; i++)
	{
		UGameplayStatics::CreatePlayer(this, i);
	}

	TArray<AActor*> playerControllers;
	UGameplayStatics::GetAllActorsOfClass(this, AMainMenuPlayerController::StaticClass(), playerControllers);

	static ConstructorHelpers::FClassFinder<UUserWidget> lobbyNotJoinedWidgetClassFinder(TEXT("/Game/UI/W_LobbyNotJoined"));
	TSubclassOf<class UUserWidget> lobbyNotJoinedWidgetClass = lobbyNotJoinedWidgetClassFinder.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> lobbyWidgetClassFinder(TEXT("/Game/UI/W_Lobby"));
	TSubclassOf<class UUserWidget> lobbyWidgetClass = lobbyWidgetClassFinder.Class;

	
	for (auto controller : playerControllers)
	{
		auto mainMenuController = Cast<AMainMenuPlayerController>(controller);
		
		auto lobbyNotJoinedWidget = CreateWidget(mainMenuController, lobbyNotJoinedWidgetClass);
		mainMenuController->SetLobbyNotJoinedWidget(lobbyNotJoinedWidget);
		lobbyNotJoinedWidget->AddToPlayerScreen();

		auto lobbyWidget = Cast<UWLobby>(CreateWidget(mainMenuController, lobbyWidgetClass));
		mainMenuController->SetLobbyWidget(lobbyWidget);
	}

	IndicesReady.Broadcast();
}

void AMainMenuGameMode::DeleteUnassignedPlayers()
{
	TArray<AActor*> playerControllers;
	UGameplayStatics::GetAllActorsOfClass(this, AMainMenuPlayerController::StaticClass(), playerControllers);

	for (auto controller : playerControllers)
	{
		auto mainMenuController = Cast<AMainMenuPlayerController>(controller);
		if (!mainMenuController->GetIsAssigned())
		{
			mainMenuController->GetLobbyNotJoinedWidget()->RemoveFromParent();
			UGameplayStatics::RemovePlayer(mainMenuController, true);
		}
	}
}

TArray<AActor*> AMainMenuGameMode::GetCharacterPreviewDummies() const
{
	return CharacterPreviewDummies;
}

TArray<AActor*> AMainMenuGameMode::GetCharacterSelectionCams() const
{
	return CharacterSelectionCams;
}
