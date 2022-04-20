// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Entrance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PressurePlate.generated.h"

UCLASS()
class FYP_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
private:
	FVector dimensions;

public:	
	// Sets default values for this actor's properties
	APressurePlate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int closeDoor();

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		UMaterialInstanceDynamic* PPMaterial;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* PPMesh;

	UPROPERTY(EditAnywhere)
		AEntrance* entrance;

};
