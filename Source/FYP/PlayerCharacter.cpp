#include "PlayerCharacter.h"
#include "GravityMovementComponent.h"

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

UGravityMovementComponent* APlayerCharacter::GetGravityMovementComponent()
{
	return Cast<UGravityMovementComponent>(GetMovementComponent());
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
			pos.Empty();
			for (const auto& p : result.PathData) {
				pos.Add(p.Location);
			}
			int i = 0;
			for (const auto& t : trajectory) {
				t->SetActorLocation(pos[i]);
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

	PlayerInputComponent->BindAction("TestRotate", IE_Pressed, this, &APlayerCharacter::TestRotate);

	PlayerInputComponent->BindAction("Recall", IE_Pressed, this, &APlayerCharacter::Recall);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::Pause);

	PlayerInputComponent->BindAction("QuickSave", IE_Pressed, this, &APlayerCharacter::QuickSave);
	PlayerInputComponent->BindAction("QuickLoad", IE_Pressed, this, &APlayerCharacter::QuickLoad);
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
				trajectory.Add(PathPoint);
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
		trajectory.Empty();

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

void APlayerCharacter::QuickSave() {
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	SaveGameInstance->PlayerLocation = this->GetActorLocation();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Quick Save"), 0);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Game Saved."));
}

void APlayerCharacter::QuickLoad() {
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Quick Save"), 0));
	this->SetActorLocation(SaveGameInstance->PlayerLocation);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Game Loaded."));
}

void APlayerCharacter::TestRotate() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetClass()->GetName());
}