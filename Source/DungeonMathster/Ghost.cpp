#include "Ghost.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
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

// Called every frame
void UGhost::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	LookTowardsPlayer();
	Target = Player->GetActorLocation();
	GoTowardsTarget(DeltaTime);
}

