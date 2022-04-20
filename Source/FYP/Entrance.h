// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Entrance.generated.h"

UCLASS()
class FYP_API AEntrance : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntrance();
	int setOpen(const bool);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		UMaterialInstanceDynamic* EntranceMaterial;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* EntranceMesh;

private:
	FVector dimensions;
	bool open;
	int positionCounter;
};
