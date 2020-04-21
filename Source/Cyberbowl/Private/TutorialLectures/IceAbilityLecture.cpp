// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/IceAbilityLecture.h"
#include "Actors/PlayBall.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Stadium/Goal_Collider.h"
#include "PlayerController/TutorialPlayerController.h"
#include "Character/CyberbowlCharacter.h"
#include "Components/WidgetComponent.h"

void AIceAbilityLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIceAbilityLecture::Enter()
{
	Super::Enter();

	tutorialPlayerController->SetInputMode(FInputModeUIOnly());
	
	tutorialCharacter->SetActorLocation(FVector(playerStartLocation->GetActorLocation().X, playerStartLocation->GetActorLocation().Y, tutorialCharacter->GetActorLocation().Z));
	ball->SetActorLocation(ballLaunchStartLocation->GetActorLocation());
	
	ChangeToIceCharacter();
	
	const FRotator lookAtGoal = UKismetMathLibrary::FindLookAtRotation(tutorialCharacter->GetActorLocation(), goal->GetActorLocation());
	tutorialPlayerController->SetControlRotation(lookAtGoal);
}

void AIceAbilityLecture::Exit()
{
	Super::Exit();

	ball->StopBall();
}

void AIceAbilityLecture::BeginPlay()
{
	Super::BeginPlay();

	ball = Cast<APlayBall>(UGameplayStatics::GetActorOfClass(this, APlayBall::StaticClass()));
	ball->OnBallFrozen.AddDynamic(this, &AIceAbilityLecture::OnBallFrozen);

	TArray<AActor*> actors;

	UGameplayStatics::GetAllActorsWithTag(this, "FireLecturePlayerStart", actors);
	playerStartLocation = actors[0];
	actors.Empty();

	UGameplayStatics::GetAllActorsWithTag(this, "FireLectureBallStart", actors);
	ballLaunchStartLocation = actors[0];
	actors.Empty();

	UGameplayStatics::GetAllActorsWithTag(this, "FireLectureGoal", actors);
	goal = Cast<AGoal_Collider>(actors[0]);
	goal->OnGoalScored.AddDynamic(this, &AIceAbilityLecture::OnGoalScored);
	actors.Empty();
}

void AIceAbilityLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskReadInstructions);
	lectureTasks.Enqueue(taskFreezeBall);
}

void AIceAbilityLecture::OnReadInstructions()
{
	AdvanceIfCurrentTask(taskReadInstructions);
}

void AIceAbilityLecture::OnBallFrozen()
{
	AdvanceIfCurrentTask(taskFreezeBall);
}

void AIceAbilityLecture::OnGoalScored(int teamIndex)
{
	LaunchBall();
}

void AIceAbilityLecture::LaunchBall() const
{
	ball->StopBall();
	ball->PlayBall();

	ball->SetActorLocation(ballLaunchStartLocation->GetActorLocation());
	ball->PushBall(2000.f, FVector(0, 1, 0));
}

void AIceAbilityLecture::ChangeToIceCharacter()
{
	auto iceCharacter = Cast<ACyberbowlCharacter>(GetWorld()->SpawnActor<AActor>(iceCharacterClass, tutorialCharacter->GetTransform()));

	GetWorld()->DestroyActor(tutorialCharacter);
	
	tutorialCharacter = iceCharacter;
	tutorialPlayerController->Possess(tutorialCharacter);

	TArray<UActorComponent*> widgetComponents;
	Cast<ACyberbowlCharacter>(tutorialCharacter)->GetComponents(UWidgetComponent::StaticClass(), widgetComponents);

	for (auto widget : widgetComponents)
	{
		widget->DestroyComponent();
	}
}
