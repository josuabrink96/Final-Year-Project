// Copyright Epic Games, Inc. All Rights Reserved.


#include "FYPGameModeBase.h"
#include "PlayerCharacter.h"

AFYPGameModeBase::AFYPGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;

    DefaultPawnClass = APlayerCharacter::StaticClass();
}