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
		dimensions = EntranceMesh->GetStaticMesh()->GetBounds().BoxExtent * EntranceMesh->GetRelativeScale3D();
	}

	open = false;
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
	
	if (open) {
		if ((positionCounter < (dimensions.Z * 2) + 10)) {
			positionCounter++;
			doorLocation.Z--;
			SetActorLocation(doorLocation);
		}
	}
	else {
		if (positionCounter > 0) {
			positionCounter--;
			doorLocation.Z++;
			SetActorLocation(doorLocation);
		}
	}
}

int AEntrance::setOpen(const bool isOpen)
{
	open = isOpen;
	return 0;
}
