// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h" 
#include "PauseWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHud.generated.h"

UCLASS()
class FYP_API ACharacterHud : public AHUD
{
	GENERATED_BODY()

protected:
	// This will be drawn at the center of the screen.
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* CrosshairTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UUserWidget> PauseMenuWidgetClass;


public:
	ACharacterHud();
	virtual void DrawHUD() override;
	void openPauseMenu();
};
