// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Character/CyberbowlCharacter.h"
#include "Blueprint/UserWidget.h"
#include "FPlayerInfo.h"
#include "Widgets/WNameTag.h"
#include "Components/WidgetComponent.h"
#include "ThirdPersonPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCallGameOverMenuNavigated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCallToggledBallCam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPausedGame, int, playerIndexInitiator);

UCLASS()
class CYBERBOWL_API AThirdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void SetupNameTagWidgets();

private:
	void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void SpawnActors();

	UFUNCTION()
	void OnStartGamePlay();

	UFUNCTION()
	void OnPauseGamePlay();

	UFUNCTION()
	void OnRegroup();

	UFUNCTION()
	void OnEndGame();

	UPROPERTY(Editanywhere)
	TSubclassOf<ACyberbowlCharacter> fireClass;

	UPROPERTY(Editanywhere)
	TSubclassOf<ACyberbowlCharacter> earthClass;

	UPROPERTY(Editanywhere)
	TSubclassOf<ACyberbowlCharacter> iceClass;

	UPROPERTY(Editanywhere)
	TSubclassOf<ACyberbowlCharacter> airClass;

	UPROPERTY(Editanywhere)
	TSubclassOf<UUserWidget> baseHudClass;

	UFUNCTION()
	void CallGameOverMenuNavigated();

	UFUNCTION()
	void CallToggledBallCam();

	UFUNCTION()
	void CallPlayerPausedGame();

	UFUNCTION()
	void UpdateNameTagWidgetRotations();

public:
	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnCallGameOverMenuNavigated OnCallGameOverMenuNavigated;

	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnCallGameOverMenuNavigated OnCallToggledBallCam;

	UPROPERTY(BlueprintAssignable, category = "EventDispatchers")
	FOnPlayerPausedGame OnPlayerPausedGame;

	UPROPERTY(BlueprintReadOnly)
	int currPlayerTeam;

	UPROPERTY(BlueprintReadOnly)
	ECBCharacterType currPlayerType;

	
protected:
	ACyberbowlCharacter* character;
	FVector spawnTransform;
	FRotator spawnRotation;

};
