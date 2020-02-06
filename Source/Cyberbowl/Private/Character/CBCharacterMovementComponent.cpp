// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CBCharacterMovementComponent.h"
#include "Character/MovementStates/WallrunState.h"
#include "Character/MovementStates/JumpState.h"
#include "Character/MovementStates/DoubleJumpState.h"
#include "Engine/Engine.h"

void UCBCharacterMovementComponent::SetCBMovementMode(ECBMovementMode mode)
{
    MovementStates[CBMovementMode]->Deactivate();
	
    CBMovementMode = mode;

    MovementStates[CBMovementMode]->Activate();

    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("MoveMode: %i"), CBMovementMode));
}

void UCBCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    MovementStates[CBMovementMode]->OnTick(DeltaTime);
}

void UCBCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
    CBMovementMode = ECBMovementMode::CBMOVE_Running;
	
    MovementStates.Add(ECBMovementMode::CBMOVE_Running, NewObject<UBaseMovementState>());
    MovementStates.Add(ECBMovementMode::CBMOVE_Wallrun, NewObject<UWallrunState>());
    MovementStates.Add(ECBMovementMode::CBMOVE_Jump, NewObject<UJumpState>());
    MovementStates.Add(ECBMovementMode::CBMOVE_DoubleJump, NewObject<UDoubleJumpState>());

	for(auto state : MovementStates)
	{
        state.Value->InitializeState(this);
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
