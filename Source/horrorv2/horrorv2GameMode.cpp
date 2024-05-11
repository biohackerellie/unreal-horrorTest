// Copyright Epic Games, Inc. All Rights Reserved.

#include "horrorv2GameMode.h"
#include "horrorv2Character.h"
#include "UObject/ConstructorHelpers.h"

Ahorrorv2GameMode::Ahorrorv2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
