#include "Ghost.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"


#define NULLGUARD
UGhost::UGhost()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGhost::BeginPlay()
{
	Super::BeginPlay();
	SetupPlayer();
	Speed = InitialSpeed;
	SetupSound();
}


void UGhost::SetupSound()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (NULLGUARD !AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing component 'UAudioComponent'."), *(GetOwner()->GetName()));
	}
}

void UGhost::PlaySound()
{
	if (NULLGUARD AudioComponent) AudioComponent->Play();
}

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

void UGhost::LookTowardsPlayer()
{
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Player->GetActorLocation());
	NewRotation.Pitch = 0.f;
	NewRotation.Roll = 0.f;
	NewRotation.Yaw -= 90.f;
	GetOwner()->SetActorRotation(NewRotation);
}

void UGhost::GoTowardsTarget(float DeltaTime)
{
	FVector ToMove = Target - GetOwner()->GetActorLocation();
	ToMove.Z = 0;
	ToMove = ToMove.GetSafeNormal() * Speed * DeltaTime;
	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + ToMove);

}

void UGhost::ProgressEndingGame(float DeltaTime)
{
	EndGameTimer -= DeltaTime;
	if(NULLGUARD PlayerManager && EndGameTimer <= 0) PlayerManager->KillPlayer();

	//Get look towards rotation
	FVector OffsetOwnerLocation = GetOwner()->GetActorLocation();
	OffsetOwnerLocation.Z += 50;
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), OffsetOwnerLocation);
	FRotator ToRotate;
	FVector PlayerViewPos;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPos,
		OUT ToRotate
	);

	//Get difference and apply rotation
	ToRotate.Yaw = FMath::FInterpTo(ToRotate.Yaw, TargetRotation.Yaw, DeltaTime, 2.f) - ToRotate.Yaw;
	ToRotate.Pitch = ToRotate.Pitch - FMath::FInterpTo(ToRotate.Pitch, TargetRotation.Pitch, DeltaTime, 2.f);
	Player->AddControllerYawInput(ToRotate.Yaw);
	Player->AddControllerPitchInput(ToRotate.Pitch);
	CaughtPlayerLocation.Z = Player->GetTargetLocation().Z;
	Player->SetActorLocation(CaughtPlayerLocation);

}

void UGhost::VerifyTarget()
{
	if (FMath::IsNearlyEqual(GetOwner()->GetTargetLocation().X, Target.X, 10.f) && FMath::IsNearlyEqual(GetOwner()->GetTargetLocation().Y, Target.Y, 10.f))
	{
		TargetPlayer = true;
		Speed = InitialSpeed + (SpeedIncrement * SpeedIncreasePerIncrement);
		if (Speed > MaxSpeed) Speed = MaxSpeed;
	}
	if(TargetPlayer) Target = Player->GetActorLocation();
}

// Called every frame
void UGhost::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerLocationWithoutZ = Player->GetActorLocation();
	FVector GhostLocationWithoutZ = GetOwner()->GetActorLocation();
	PlayerLocationWithoutZ.Z = 0;
	GhostLocationWithoutZ.Z = 0;
	if (TargetPlayer && FVector::Distance(GhostLocationWithoutZ, PlayerLocationWithoutZ) <= Reach && PlayerCaught == false)
	{
		PlayerCaught = true;
		PlaySound();
		CaughtPlayerLocation = Player->GetTargetLocation();
	}
	if (PlayerCaught) ProgressEndingGame(DeltaTime);
	else
	{
		VerifyTarget();
		if(TargetingActive) GoTowardsTarget(DeltaTime);
	}
	LookTowardsPlayer();

}

void UGhost::SetTarget(FVector NewTarget)
{
	TargetingActive = true;
	Target = NewTarget;
	Speed = TargetRoomSpeed;
	TargetPlayer = false;
}

void UGhost::IncreaseSpeedIncrement()
{
	SpeedIncrement++;
	if(TargetPlayer) Speed = InitialSpeed + (SpeedIncrement * SpeedIncreasePerIncrement);
}

