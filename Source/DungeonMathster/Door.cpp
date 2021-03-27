#include "Door.h"
#include "Math/UnrealMathUtility.h"

#define OUT
#define NULLGUARD

UDoor::UDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDoor::BeginPlay()
{
	Super::BeginPlay();
	SetUpRotators();
}

void UDoor::SetUpRotators()
{
	InitialRotation = GetOwner()->GetActorRotation();
	CurrentRotation = InitialRotation;
	TargetRotation = InitialRotation;
	TargetRotation.Yaw = InitialRotation.Yaw + DegreesToOpen;
}

void UDoor::Close()
{
	InProgress = true;
	TargetRotation.Yaw = InitialRotation.Yaw;
}

void UDoor::Open()
{
	InProgress = true;
	TargetRotation.Yaw = InitialRotation.Yaw + DegreesToOpen;
}

void UDoor::Progress(float DeltaTime)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	FRotator ToRotate = InitialRotation;
	ToRotate.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, OpenSpeed);
	GetOwner()->SetActorRotation(ToRotate);
	if (FMath::IsNearlyEqual(ToRotate.Yaw, TargetRotation.Yaw, 1.f))
	{
		InProgress = false;
		UE_LOG(LogTemp, Display, TEXT("Door progression done!"));
	}
}

// Called every frame
void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (InProgress)
	{
		Progress(DeltaTime);
	}
}

