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

    static ConstructorHelpers::FClassFinder<AHUD> HudBP(TEXT("Blueprint'/Game/MyContent/UI/MyCharacterHud_BP.MyCharacterHud_BP_C'"));
    check(HudBP.Class != nullptr);
    HUDClass = HudBP.Class;
}