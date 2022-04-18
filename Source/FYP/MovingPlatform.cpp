#include "MovingPlatform.h"

int AMovingPlatform::platformCount = 0;

AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PlatformMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlatformAsset(TEXT("/Game/MyContent/Meshes/platform.Platform"));
	if (PlatformAsset.Succeeded()) {
		PlatformMesh->SetStaticMesh(PlatformAsset.Object);
		PlatformMesh->SetWorldScale3D(FVector(1.0f, 0.5f, 0.5f));
	}

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	platformCount++;
	thisPlatform = platformCount % 2;
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = GetActorLocation();
	float time = GetGameTimeSinceCreation();
	float posX = FMath::Sin(time + DeltaTime) * 2;

	if (thisPlatform) {
		location.X += posX;
	}
	else {
		location.X -= posX;
	}

	SetActorLocation(location);
}

