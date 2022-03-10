// Fill out your copyright notice in the Description page of Project Settings.


#include "BigRedButton.h"

// Sets default values
ABigRedButton::ABigRedButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BRBMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BRBMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BRBAsset(TEXT("/Game/MyContent/Meshes/BigRedButton.BigRedButton"));
	if (BRBAsset.Succeeded()) {
		BRBMesh->SetStaticMesh(BRBAsset.Object);
		BRBMesh->SetWorldScale3D(FVector(1.0f, 0.5f, 1.0f));
	}
}

// Called when the game starts or when spawned
void ABigRedButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABigRedButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

