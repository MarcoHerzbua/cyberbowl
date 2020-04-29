// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CBCharacterMovementComponent.h"

#include "Character/MovementStates/DashState.h"
#include "Character/MovementStates/WallrunState.h"
#include "Character/MovementStates/JumpState.h"
#include "Character/MovementStates/DoubleJumpState.h"
#include "Engine/Engine.h"
#include "Character/CyberbowlCharacterAnimInstance.h"

void UCBCharacterMovementComponent::SetCBMovementMode(ECBMovementMode mode)
{
	if(MovementStates.Num() == 0)
	{
        return;
	}
    MovementStates[CBMovementMode]->Deactivate();

    auto previousMode = CBMovementMode;
    CBMovementMode = mode;
	
	MovementStates[CBMovementMode]->Activate(previousMode);
	
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("MoveMode: %i"), CBMovementMode));
}

void UCBCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    MovementStates[CBMovementMode]->OnTick(DeltaTime);
}

void UCBCharacterMovementComponent::CallOnWallRunFinished(float timeOnWall, bool launchedAway)
{
    OnWallRunFinished.Broadcast(timeOnWall, launchedAway);
}

void UCBCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
    CBMovementMode = ECBMovementMode::CBMOVE_Running;

    MovementStates.Add(ECBMovementMode::CBMOVE_Running, NewObject<UBaseMovementState>());
    MovementStates.Add(ECBMovementMode::CBMOVE_Jump, NewObject<UJumpState>());
    MovementStates.Add(ECBMovementMode::CBMOVE_DoubleJump, NewObject<UDoubleJumpState>());

    auto dashState = NewObject<UDashState>();
    dashState->OnUpDash.AddDynamic(this, &UCBCharacterMovementComponent::CallOnVerticalDash);
    MovementStates.Add(ECBMovementMode::CBMOVE_Dash, dashState);

    auto wallrunState = NewObject<UWallrunState>();
    wallrunState->OnWallrunFinish.AddDynamic(this, &UCBCharacterMovementComponent::CallOnWallRunFinished);
    MovementStates.Add(ECBMovementMode::CBMOVE_Wallrun, wallrunState);

	for(auto state : MovementStates)
	{
        state.Value->InitializeState(this);
	}

    animinstance = Cast<UCyberbowlCharacterAnimInstance>(GetOwner()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance());
    if (animinstance)
    {
        animinstance->SetDashPlayRate(DashDuration);
    }
}

void UCBCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    UCharacterMovementComponent::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if(MovementMode == EMovementMode::MOVE_Walking)
	{
        SetCBMovementMode(ECBMovementMode::CBMOVE_Running);
	}
}

void UCBCharacterMovementComponent::CalcVelocity(float DeltaTime, float Friction, bool bFluid,
	float BrakingDeceleration)
{
    Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);

	//if(CBMovementMode == ECBMovementMode::CBMOVE_Running)
	//{
	//	if(Acceleration.IsZero())
	//	{
	//        Velocity = FVector::ZeroVector;
	//	}
	//    else
	//    {
	//        Velocity = GetMaxSpeed() * Acceleration.GetSafeNormal();
	//    }
	//	
	//}
}

/*
 *
 *
 * EXPERIMENTAL MOVEMENT
 * Experiment trying to add custom Movementmodes to the existing MovementMode System of Unreal
 * Is quite complex and makes my brain go ouchie
 * 
 */

void UCBCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch(CustomMovementMode)
	{
	case int(ECBMovementMode::CBMOVE_Wallrun) :
		PhysWallrun(deltaTime, Iterations);
		break;
	case int(ECBMovementMode::CBMOVE_Running) :
	default:
		break;
	}
}

void UCBCharacterMovementComponent::PhysWallrun(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

    if (deltaTime < MIN_TICK_TIME)
    {
        return;
    }

    RestorePreAdditiveRootMotionVelocity();

    if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
    {
        if (bCheatFlying && Acceleration.IsZero())
        {
            Velocity = FVector::ZeroVector;
        }
        const float Friction = GroundFriction;//0.5f * GetPhysicsVolume()->FluidFriction;
        CalcVelocity(deltaTime, Friction, true, GetMaxBrakingDeceleration());
    }

    ApplyRootMotionToVelocity(deltaTime);

    Iterations++;
    bJustTeleported = false;

    FVector OldLocation = UpdatedComponent->GetComponentLocation();
    const FVector Adjusted = Velocity * deltaTime;
    FHitResult Hit(1.f);
    SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

    if (Hit.Time < 1.f)
    {
        const FVector GravDir = FVector(0.f, 0.f, -1.f);
        const FVector VelDir = Velocity.GetSafeNormal();
        const float UpDown = GravDir | VelDir;

        bool bSteppedUp = false;
        if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f) && CanStepUp(Hit))
        {
            float stepZ = UpdatedComponent->GetComponentLocation().Z;
            bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
            if (bSteppedUp)
            {
                OldLocation.Z = UpdatedComponent->GetComponentLocation().Z + (OldLocation.Z - stepZ);
            }
        }

        if (!bSteppedUp)
        {
            //adjust and try again
            HandleImpact(Hit, deltaTime, Adjusted);
            SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
        }
    }

    //if(CurrentFloor.IsWalkableFloor() && CurrentFloor.HitResult.bStartPenetrating)
    //{
    //    SetMovementMode(MOVE_Walking);
    //}

    if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
    {
        Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
    }

}

void UCBCharacterMovementComponent::CallOnVerticalDash()
{
    OnVertDash.Broadcast();
}