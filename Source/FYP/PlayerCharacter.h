// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Projectile.h"
#include "PathPoint.h"
#include "CharacterHud.h"
#include "MySaveGame.h"
#include "GravityMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FYP_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	enum fireStatus {
		Idle,
		Aiming,
		Fired,
	};

	TArray<APathPoint*> trajectory;
	TArray<FVector> pos;
	FVector CameraLocation;
	FRotator CameraRotation;
	AProjectile* Projectile;
	APathPoint* PathPoint;
	fireStatus status = Idle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;	

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
		UGravityMovementComponent* GetGravityMovementComponent();

public:	
	APlayerCharacter();

	APlayerCharacter(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer.SetDefaultSubobjectClass<UGravityMovementComponent>(ACharacter::CharacterMovementComponentName)) {};

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
		void Recall();	
	
	UFUNCTION()
		void Pause();

	UFUNCTION()
		void QuickSave();

	UFUNCTION()
		void QuickLoad();

	UFUNCTION()
		void TestRotate();

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FPMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector AimOffset;

};
