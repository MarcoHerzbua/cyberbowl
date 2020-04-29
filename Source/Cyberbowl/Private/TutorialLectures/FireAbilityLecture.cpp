// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLectures/FireAbilityLecture.h"
#include "Actors/PlayBall.h"
#include "Character/Abilities/Firewall.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerController/TutorialPlayerController.h"
#include "Stadium/Goal_Collider.h"
#include "Character/CyberbowlCharacter.h"

void AFireAbilityLecture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireAbilityLecture::Enter()
{
	Super::Enter();

	tutorialCharacter->SetActorLocation(FVector(playerStartLocation->GetActorLocation().X, playerStartLocation->GetActorLocation().Y, tutorialCharacter->GetActorLocation().Z));
	ball->SetActorLocation(ballLaunchStartLocation->GetActorLocation());

	tutorialCharacter = tutorialPlayerController->SwitchCharacterClass(fireCharacterClass);
	
	const FRotator lookAtGoal = UKismetMathLibrary::FindLookAtRotation(tutorialCharacter->GetActorLocation(), goal->GetActorLocation());
	tutorialPlayerController->SetControlRotation(lookAtGoal);
}

void AFireAbilityLecture::Exit()
{
	Super::Exit();

	ball->StopBall();
}

void AFireAbilityLecture::BeginPlay()
{
	Super::BeginPlay();

	ball = Cast<APlayBall>(UGameplayStatics::GetActorOfClass(this, APlayBall::StaticClass()));
	ball->OnActorHit.AddDynamic(this, &AFireAbilityLecture::OnBallBlocked);

	TArray<AActor*> actors;
	
	UGameplayStatics::GetAllActorsWithTag(this, "FireLecturePlayerStart", actors);
	playerStartLocation = actors[0];
	actors.Empty();

	UGameplayStatics::GetAllActorsWithTag(this, "FireLectureBallStart", actors);
	ballLaunchStartLocation = actors[0];
	actors.Empty();

	UGameplayStatics::GetAllActorsWithTag(this, "FireLectureGoal", actors);
	goal = Cast<AGoal_Collider>(actors[0]);
	goal->OnGoalScored.AddDynamic(this, &AFireAbilityLecture::OnGoalScored);
	actors.Empty();
}

void AFireAbilityLecture::SetupTasks()
{
	lectureTasks.Enqueue(taskReadInstructions);
	lectureTasks.Enqueue(taskBlockBall);
}

void AFireAbilityLecture::OnReadInstructions()
{
	AdvanceIfCurrentTask(taskReadInstructions);
}

void AFireAbilityLecture::OnBallBlocked(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AFirewall>(OtherActor))
	{
		AdvanceIfCurrentTask(taskBlockBall, 1.f);
	}
}

void AFireAbilityLecture::OnGoalScored(int teamIndex)
{
	LaunchBall();
}

void AFireAbilityLecture::LaunchBall() const
{
	ball->StopBall();
	ball->PlayBall();
	
	ball->SetActorLocation(ballLaunchStartLocation->GetActorLocation());
	ball->PushBall(2000.f, FVector(0, 1, 0));
}
