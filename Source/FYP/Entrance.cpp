// Fill out your copyright notice in the Description page of Project Settings.


#include "Entrance.h"

// Sets default values
AEntrance::AEntrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EntranceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	EntranceMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> entranceAsset(TEXT("/Game/MyContent/Meshes/Entrance.Entrance"));
	if (entranceAsset.Succeeded()) {
		EntranceMesh->SetStaticMesh(entranceAsset.Object);
		EntranceMesh->SetWorldScale3D(FVector(1.5f, 5.5f, 5.0f));
	}
}

// Called when the game starts or when spawned
void AEntrance::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector doorLocation = GetActorLocation();
	if (doorLocation.Z < 0) {
		float RunningTime = GetGameTimeSinceCreation();
		doorLocation.Z += RunningTime;
		SetActorLocation(doorLocation);
	}
}

