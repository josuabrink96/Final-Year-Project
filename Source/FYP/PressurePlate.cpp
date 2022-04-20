// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"

// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PPMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PPMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PPAsset(TEXT("/Game/MyContent/Meshes/PressurePlate.PressurePlate"));
	if (PPAsset.Succeeded()) {
		PPMesh->SetStaticMesh(PPAsset.Object);
		dimensions = PPMesh->GetStaticMesh()->GetBounds().BoxExtent * PPMesh->GetRelativeScale3D();
	}
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn() != nullptr) {
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (controller->GetPawn()->GetActorLocation().Y >= this->GetActorLocation().Y && controller->GetPawn()->GetActorLocation().Y <= (this->GetActorLocation().Y + dimensions.Y)) {
			if (controller->GetPawn()->GetActorLocation().X >= this->GetActorLocation().X && controller->GetPawn()->GetActorLocation().X <= (this->GetActorLocation().X + dimensions.X)) {
				closeDoor();
			}
		}
	}
}

int APressurePlate::closeDoor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Closing Door."));
	if (entrance != nullptr) {
		entrance->setOpen(false);
	}
	return 0;
}

