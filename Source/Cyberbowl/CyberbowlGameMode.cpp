// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CyberbowlGameMode.h"
#include "CyberbowlCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACyberbowlGameMode::ACyberbowlGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
