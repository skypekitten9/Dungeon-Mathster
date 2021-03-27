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


void UDoor::ToggleDoor(bool ToOpen)
{

}

void UDoor::Close(float DeltaTime)
{

}

void UDoor::Open(float DeltaTime)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	FRotator ToRotate = InitialRotation;
	ToRotate.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, OpenSpeed);
	GetOwner()->SetActorRotation(ToRotate);
	UE_LOG(LogTemp, Warning, TEXT("Rotation set to %s"), *ToRotate.ToString());
}

// Called every frame
void UDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Open(DeltaTime);
}

