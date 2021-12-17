// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "PathPoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FYP_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	std::vector<APathPoint*> trajectory;
	AProjectile* Projectile;
	APathPoint* PathPoint;
	bool hasFired = false;
	bool aiming = false;
	bool pathSpawned = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Actor)
		TSubclassOf<class APathPoint> PathPointClass;

public:	
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector getMuzzleLocation();

	UFUNCTION()
		void MoveForwardBack(float val);

	UFUNCTION()
		void MoveRightLeft(float val);

	UFUNCTION()
		void StartJump();

	UFUNCTION()
		void StopJump();

	UFUNCTION()
		void Fire();

	UFUNCTION()
		void Aim();

	UFUNCTION()
		void Teleport();

	UFUNCTION()
		void Recall();

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FPMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

};
