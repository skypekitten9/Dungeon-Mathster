#include "Ghost.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
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
}


void UGhost::SetupPlayer()
{
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (NULLGUARD !Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'Player'."), *(GetOwner()->GetName()));
		return;
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
	FVector ToMove = GetOwner()->GetActorLocation();
	ToMove.X = FMath::FInterpConstantTo(ToMove.X, Target.X, DeltaTime, Speed);
	ToMove.Y = FMath::FInterpConstantTo(ToMove.Y, Target.Y, DeltaTime, Speed);
	GetOwner()->SetActorLocation(ToMove);

}

void UGhost::ProgressEndingGame(float DeltaTime)
{
	EndGameTimer -= DeltaTime;
	if(EndGameTimer <= 0) UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

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

	UE_LOG(LogTemp, Warning, TEXT("Current yaw: %f Target yaw: %f"), ToRotate.Yaw, TargetRotation.Yaw);
	UE_LOG(LogTemp, Warning, TEXT("Current pitch: %f Target pitch: %f"), ToRotate.Pitch, TargetRotation.Pitch);

	//Get difference and apply rotation
	ToRotate.Yaw = FMath::FInterpTo(ToRotate.Yaw, TargetRotation.Yaw, DeltaTime, 2.f) - ToRotate.Yaw;
	ToRotate.Pitch = ToRotate.Pitch - FMath::FInterpTo(ToRotate.Pitch, TargetRotation.Pitch, DeltaTime, 2.f);
	Player->AddControllerYawInput(ToRotate.Yaw);
	Player->AddControllerPitchInput(ToRotate.Pitch);

}

// Called every frame
void UGhost::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (FVector::Distance(GetOwner()->GetActorLocation(), Player->GetActorLocation()) <= Reach)
	{
		PlayerCaught = true;
	}
	if (PlayerCaught) ProgressEndingGame(DeltaTime);
	else
	{
		Target = Player->GetActorLocation();
		GoTowardsTarget(DeltaTime);
	}
	LookTowardsPlayer();

}

