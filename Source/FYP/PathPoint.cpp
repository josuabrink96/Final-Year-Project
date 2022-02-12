// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPoint.h"

// Sets default values
APathPoint::APathPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphereAsset(TEXT("/Game/MyContent/Meshes/TrajectoryPoint.TrajectoryPoint"));
	if (sphereAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(sphereAsset.Object);
		VisualMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Game/MyContent/Materials/Trajectory.Trajectory"));
	if (Material.Succeeded()) 
	{
		PathPointMaterial = UMaterialInstanceDynamic::Create(Material.Object, VisualMesh);
		VisualMesh->SetMaterial(0, PathPointMaterial);
	}

}

// Called when the game starts or when spawned
void APathPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

