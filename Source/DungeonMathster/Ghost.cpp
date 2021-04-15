#include "Ghost.h"

#define OUT
#define NULLGUARD

#pragma region Main Methods
UGhost::UGhost()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGhost::BeginPlay()
{
	Super::BeginPlay();
	SetupPlayer();
	SetupSound();

	Speed = InitialSpeed;
}

// Called every frame
void UGhost::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PlayerCaught == false)
	{
		TryCatchingPlayer();
		VerifyTarget();
		if (TargetingActive) GoTowardsTarget(DeltaTime);
	}
	else ProgressEndingGame(DeltaTime);
	LookTowardsPlayer();

}
#pragma endregion Main Methods

#pragma region Setup
void UGhost::SetupPlayer()
{
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (NULLGUARD !Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'Player'."), *(GetOwner()->GetName()));
		return;
	}
	PlayerManager = Player->FindComponentByClass<UPlayerManager>();
	if (NULLGUARD !PlayerManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'PlayerManager'."), *(GetOwner()->GetName()));
	}
}

void UGhost::SetupSound()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (NULLGUARD !AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing component 'UAudioComponent'."), *(GetOwner()->GetName()));
	}
}
#pragma endregion Setup

//Moves towards target ignoring height (z)
void UGhost::GoTowardsTarget(float DeltaTime)
{
	FVector ToMove = Target - GetOwner()->GetActorLocation();
	ToMove.Z = 0;
	ToMove = ToMove.GetSafeNormal() * Speed * DeltaTime;
	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + ToMove);

}

//Rotates yaw to look at player
void UGhost::LookTowardsPlayer()
{
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Player->GetActorLocation());
	NewRotation.Pitch = 0.f;
	NewRotation.Roll = 0.f;
	NewRotation.Yaw += ModelYawOffset; //Used model has incorrect forward vector
	GetOwner()->SetActorRotation(NewRotation);
}

//Increases speed
void UGhost::IncreaseSpeed()
{
	SpeedIncrement++;
	if (TargetPlayer) Speed = InitialSpeed + (SpeedIncrement * SpeedIncreasePerIncrement);
}

void UGhost::PlaySound()
{
	if (NULLGUARD AudioComponent) AudioComponent->Play();
}

//Forces player to look at ghost then resets game
void UGhost::ProgressEndingGame(float DeltaTime)
{
	EndGameTimer -= DeltaTime;
	if (NULLGUARD PlayerManager && EndGameTimer <= 0) PlayerManager->KillPlayer(); //Delay reseting game

	//Calculate look at ghost rotation for player
	FVector OffsetOwnerLocation = GetOwner()->GetActorLocation();
	OffsetOwnerLocation.Z += ModelZOffset;
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), OffsetOwnerLocation);

	//Get current player rotation
	FRotator ToRotate;
	FVector PlayerViewPos;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPos,
		OUT ToRotate
	);

	//Interpolate and set players rotation
	ToRotate.Yaw = FMath::FInterpTo(ToRotate.Yaw, TargetRotation.Yaw, DeltaTime, 2.f) - ToRotate.Yaw;
	ToRotate.Pitch = ToRotate.Pitch - FMath::FInterpTo(ToRotate.Pitch, TargetRotation.Pitch, DeltaTime, 2.f);
	if (NULLGUARD Player)
	{
		Player->AddControllerYawInput(ToRotate.Yaw);
		Player->AddControllerPitchInput(ToRotate.Pitch);
		CaughtPlayerLocation.Z = Player->GetTargetLocation().Z;
		Player->SetActorLocation(CaughtPlayerLocation);
	}
}

void UGhost::TryCatchingPlayer()
{
	//Ignore height difference
	FVector PlayerLocationWithoutZ = Player->GetActorLocation();
	FVector GhostLocationWithoutZ = GetOwner()->GetActorLocation();
	PlayerLocationWithoutZ.Z = 0;
	GhostLocationWithoutZ.Z = 0;

	//Check if player is within reach and act accordingly
	if (NULLGUARD Player && TargetPlayer && FVector::Distance(GhostLocationWithoutZ, PlayerLocationWithoutZ) <= Reach)
	{
		PlayerCaught = true;
		PlaySound();
		CaughtPlayerLocation = Player->GetTargetLocation();
	}
}

void UGhost::SetTarget(FVector NewTarget)
{
	TargetingActive = true;
	Target = NewTarget;
	Speed = TargetRoomSpeed;
	TargetPlayer = false;
}

//Checks if target has been reached, if true changes target to player
void UGhost::VerifyTarget()
{
	if (FMath::IsNearlyEqual(GetOwner()->GetTargetLocation().X, Target.X, 10.f) && FMath::IsNearlyEqual(GetOwner()->GetTargetLocation().Y, Target.Y, 10.f))
	{
		TargetPlayer = true;
		Speed = InitialSpeed + (SpeedIncrement * SpeedIncreasePerIncrement);
		if (Speed > MaxSpeed) Speed = MaxSpeed;
	}
	if (NULLGUARD Player && TargetPlayer) Target = Player->GetActorLocation();
}