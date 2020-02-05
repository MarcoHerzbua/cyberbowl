#include "Character/MovementStates/WallrunState.h"
#include "Character/CBCharacterMovementComponent.h"
#include "Components/InputComponent.h"

WallrunState::WallrunState()
{
}

void WallrunState::InitializeState(UCBCharacterMovementComponent* moveComponent)
{
	BaseMovementState::InitializeState(moveComponent);

	DefaultGravityScale = MovementComponent->GravityScale;
	InputComponent = MovementComponent->GetOwner()->InputComponent;
}

void WallrunState::Activate()
{
	BaseMovementState::Activate();

	MovementComponent->GravityScale = 0.f;

	//InputComponent->BindAction("Jump", IE_Pressed, this, &WallrunState::LaunchCharacter);
}

void WallrunState::Deactivate()
{
	BaseMovementState::Deactivate();

	MovementComponent->GravityScale = DefaultGravityScale;

	//InputComponent->("Jump", IE_Pressed, this, &WallrunState::LaunchCharacter);
}

void WallrunState::OnTick(float DeltaTime)
{
	MovementComponent->Velocity.Z = 0;

	float length;
	MovementComponent->Velocity.ToDirectionAndLength(WallrunDirection, length);

	MovementComponent->Velocity = WallrunDirection * (MovementComponent->MaxCustomMovementSpeed * MovementComponent->WallrunSpeedModifier);
}

void WallrunState::LaunchCharacter()
{
}
