// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIndicesReady);

class ACyberbowlCharacter;
class AMainMenuPlayerController;
class UWMainMenu;

UCLASS()
class CYBERBOWL_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AMainMenuGameMode(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnLobbyEntered();

	UFUNCTION()
	void OnPlayerReadyUnready();

	UFUNCTION()
	void OpenArena();

	UPROPERTY()
	FTimerHandle CountdownTimer;

	UPROPERTY()
	TArray<AActor*> CharacterSelectionCams;

	UPROPERTY()
	TArray<AActor*> CharacterPreviewDummies;

	UPROPERTY(BlueprintReadWrite)
	TArray<AMainMenuPlayerController*> ConnectedPlayers;
	
	UPROPERTY()
	UWMainMenu* MainMenuWidget;

	UPROPERTY(BlueprintReadWrite)
	int TotalPlayers;

	UPROPERTY(BlueprintReadWrite)
	int PlayersReady;

	UPROPERTY(BlueprintReadWrite)
	int CurrentPlayersTeam1;

	UPROPERTY(BlueprintReadWrite)
	int CurrentPlayersTeam2;

	bool bAllReady;

	UPROPERTY(BlueprintReadWrite)
	float totalCooldown = 3.f;

	UPROPERTY(BlueprintReadWrite)
	float currentCooldown = 3.f;
	
	void CreateMainMenu();

	void SetupCharacterSelection();
	
	void CreatePlayers();

	void DeleteUnassignedPlayers();
	
public:
	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnIndicesReady IndicesReady;

	UFUNCTION()
	TArray<AActor*> GetCharacterPreviewDummies() const;

	UFUNCTION()
	TArray<AActor*> GetCharacterSelectionCams() const;
	
};
