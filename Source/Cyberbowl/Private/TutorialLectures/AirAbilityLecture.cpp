// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/AirAbilityLecture.h"
#include "Actors/PlayBall.h"
#include "Character/Abilities/AirAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Stadium/Goal_Collider.h"
#include "PlayerController/TutorialPlayerController.h"
#include "Character/CyberbowlCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void AAirAbilityLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAirAbilityLecture::Enter()
{
	Super::Enter();

	tutorialCharacter->SetActorLocation(FVector(playerStartLocation->GetActorLocation().X, playerStartLocation->GetActorLocation().Y, tutorialCharacter->GetActorLocation().Z));
	ball->SetActorLocation(ballLaunchStartLocation->GetActorLocation());

	tutorialCharacter = tutorialPlayerController->SwitchCharacterClass(airCharacterClass);
	auto airAbility = Cast<UAirAbility>(tutorialCharacter->GetComponentByClass(UAirAbility::StaticClass()));
	airAbility->OnGrabModeExitByPush.AddDynamic(this, &AAirAbilityLecture::OnBallSucced);

	const FRotator lookAtGoal = UKismetMathLibrary::FindLookAtRotation(tutorialCharacter->GetActorLocation(), goal->GetActorLocation());
	tutorialPlayerController->SetControlRotation(lookAtGoal);
}

void AAirAbilityLecture::Exit()
{
	Super::Exit();

	//ball->StopBall();
}

void AAirAbilityLecture::BeginPlay()
{
	Super::BeginPlay();

	ball = Cast<APlayBall>(UGameplayStatics::GetActorOfClass(this, APlayBall::StaticClass()));

	TArray<AActor*> actors;

	UGameplayStatics::GetAllActorsWithTag(this, "FireLecturePlayerStart", actors);
	playerStartLocation = actors[0];
	actors.Empty();

	UGameplayStatics::GetAllActorsWithTag(this, "FireLectureBallStart", actors);
	ballLaunchStartLocation = actors[0];
	actors.Empty();

	UGameplayStatics::GetAllActorsWithTag(this, "FireLectureGoal", actors);
	goal = Cast<AGoal_Collider>(actors[0]);
	goal->OnGoalScored.AddDynamic(this, &AAirAbilityLecture::OnGoalScored);
	actors.Empty();
}

void AAirAbilityLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskReadInstructions);
	lectureTasks.Enqueue(taskGrabAndThrow);
}

void AAirAbilityLecture::OnReadInstructions()
{
	AdvanceIfCurrentTask(taskReadInstructions);
}

void AAirAbilityLecture::OnBallSucced()
{
	AdvanceIfCurrentTask(taskGrabAndThrow, 1.f);
}

void AAirAbilityLecture::OnGoalScored(int teamIndex)
{
	LaunchBall();
}

void AAirAbilityLecture::LaunchBall() const
{
	ball->StopBall();
	ball->PlayBall();

	ball->SetActorLocation(ballLaunchStartLocation->GetActorLocation());
	ball->PushBall(2000.f, FVector(0, 1, 0));
}
