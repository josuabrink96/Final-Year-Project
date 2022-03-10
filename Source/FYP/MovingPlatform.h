// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class FYP_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	AMovingPlatform();

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
		UMaterialInstanceDynamic* PlatformMaterial;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* PlatformMesh;

private:
	int thisPlatform;
	static int platformCount;
};
