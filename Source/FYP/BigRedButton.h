// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Entrance.h"
#include "BigRedButton.generated.h"

UCLASS()
class FYP_API ABigRedButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABigRedButton();
	int openDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		UMaterialInstanceDynamic* BRBMaterial;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BRBMesh;

	UPROPERTY(EditAnywhere)
		AEntrance* entrance;


};
