// Copyright Epic Games, Inc. All Rights Reserved.


#include "FYPGameModeBase.h"


AFYPGameModeBase::AFYPGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;

    static ConstructorHelpers::FClassFinder<APawn> PlayerCharacterBP(TEXT("Blueprint'/Game/MyContent/Meshes/Player/PlayerCharacter_BP.PlayerCharacter_BP_C'"));
    check(PlayerCharacterBP.Class != nullptr);
    DefaultPawnClass = PlayerCharacterBP.Class;

    HUDClass = ACharacterHud::StaticClass();
    
}