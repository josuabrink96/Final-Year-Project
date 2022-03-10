#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPCameraComponent != nullptr);
	FPCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	FPCameraComponent->bUsePawnControlRotation = true;

	FPMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPMesh != nullptr);
	FPMesh->SetOnlyOwnerSee(true);
	FPMesh->SetupAttachment(FPCameraComponent);
	FPMesh->bCastDynamicShadow = false;
	FPMesh->CastShadow = false;

	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (status == Aiming) {
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FPredictProjectilePathParams params;
		FPredictProjectilePathResult result;

		AimOffset.Set(100.0f, 100.0f, 0.0f);
		FVector AimLocation = CameraLocation + FTransform(CameraRotation).TransformVector(AimOffset);
		FRotator AimRotation = CameraRotation;
		AimRotation.Pitch += 10.0f;

		params.StartLocation = AimLocation;
		params.LaunchVelocity = AimRotation.Vector() * 2000.0f;
		params.MaxSimTime = 2.0f;

		UWorld* World = GetWorld();
		if (World) 
		{
			UGameplayStatics::PredictProjectilePath(World, params, result);
			pos.clear();
			for (const auto& p : result.PathData) {
				pos.emplace_back(p.Location);
			}

			int i = 0;
			for (const auto& t : trajectory) {
				t->SetActorLocation(pos.at(i));
				i++;
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForwardBack", this, &APlayerCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis("MoveRightLeft", this, &APlayerCharacter::MoveRightLeft);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpDown", this, &APlayerCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Aim);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::Fire);

	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &APlayerCharacter::Teleport);
	PlayerInputComponent->BindAction("Recall", IE_Pressed, this, &APlayerCharacter::Recall);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::Pause);
}

void APlayerCharacter::MoveForwardBack(float val)
{	
	//this->GetMesh()->SetAnimation()
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, val);
}

void APlayerCharacter::MoveRightLeft(float val)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, val);
}

void APlayerCharacter::StartJump() 
{
	bPressedJump = true;
}

void APlayerCharacter::StopJump() 
{
	bPressedJump = false;
}

void APlayerCharacter::Aim() {
	if (status == Idle) {
		status = Aiming;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Aiming..."));

		UWorld* World = GetWorld();
		if (World) {
			for (auto& p : pos) {
				PathPoint = World->SpawnActor<APathPoint>(p, CameraRotation);
				trajectory.emplace_back(PathPoint);
			}
		}
	}
}

void APlayerCharacter::Fire()
{
	if (ProjectileClass && status == Aiming)
	{	
		status = Fired;
		for (auto& t : trajectory) {
			t->Destroy();
		}
		trajectory.clear();

		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		AimOffset.Set(100.0f, 100.0f, 0.0f);
		FVector AimLocation = CameraLocation + FTransform(CameraRotation).TransformVector(AimOffset);
		FRotator AimRotation = CameraRotation;
		AimRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			Projectile = World->SpawnActor<AProjectile>(ProjectileClass, AimLocation, AimRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = AimRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void APlayerCharacter::Teleport() {
	if (status == Fired) {
		this->SetActorLocation(Projectile->GetActorLocation());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Teleported to ball."));
		this->Recall();
	}
}

void APlayerCharacter::Recall() {
	if (status == Fired) {
		status = Idle;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Ball recalled."));
		Projectile->Destroy();
	}
}

void APlayerCharacter::Pause() {
	ACharacterHud* myHud = Cast<ACharacterHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	myHud->openPauseMenu();
}

