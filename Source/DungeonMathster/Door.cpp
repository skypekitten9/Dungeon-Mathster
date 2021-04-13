#include "Door.h"
#include "GameFramework/Actor.h"
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
	SetupRotators();
	SetupSound();
}

void UDoor::SetupRotators()
{
	InitialRotation = GetOwner()->GetActorRotation();
	CurrentRotation = InitialRotation;
	TargetRotation = InitialRotation;
	TargetRotation.Yaw = InitialRotation.Yaw + DegreesToOpen;
}

void UDoor::SetupSound()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (NULLGUARD !AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing component 'UAudioComponent'."), *(GetOwner()->GetName()));
	}
	if (OpenSounds.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing sounds."), *(GetOwner()->GetName()));
	}
	for (USoundBase* s : OpenSounds)
	{
		if (NULLGUARD !s)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a sound."), *(GetOwner()->GetName()));
		}
	}
	for (USoundBase* s : CloseSounds)
	{
		if (NULLGUARD !s)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a sound."), *(GetOwner()->GetName()));
		}
	}
}

void UDoor::Close()
{
	InProgress = true;
	Opened = false;
	TargetRotation.Yaw = InitialRotation.Yaw;
	PlaySound(false);
}

void UDoor::Open()
{
	InProgress = true;
	Opened = true;
	TargetRotation.Yaw = InitialRotation.Yaw + DegreesToOpen;
	PlaySound(true);
}

bool UDoor::IsOpen()
{
	return Opened;
}

void UDoor::Progress(float DeltaTime)
{
	CurrentRotation = GetOwner()->GetActorRotation();
	FRotator ToRotate = InitialRotation;
	float Speed = 0.f;
	if (IsOpen())
	{
		Speed = OpenSpeed;
	}
	else
	{
		Speed = CloseSpeed;
	}
	ToRotate.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, Speed);
	GetOwner()->SetActorRotation(ToRotate);
	if (FMath::IsNearlyEqual(ToRotate.Yaw, TargetRotation.Yaw, 1.f))
	{
		InProgress = false;
		UE_LOG(LogTemp, Display, TEXT("Door progression done!"));
	}
}

void UDoor::PlaySound(bool Open)
{
	if (NULLGUARD !AudioComponent || OpenSounds.Num() <= 0 || CloseSounds.Num() <= 0) return;
	if (Open)
	{
		int32 RandomNum = FMath::RandRange(0, OpenSounds.Num() - 1);
		if (NULLGUARD !OpenSounds[RandomNum]) return;
		AudioComponent->SetSound(OpenSounds[RandomNum]);
	}
	else
	{
		int32 RandomNum = FMath::RandRange(0, CloseSounds.Num() - 1);
		if (NULLGUARD !CloseSounds[RandomNum]) return;
		AudioComponent->SetSound(CloseSounds[RandomNum]);
	}
	AudioComponent->Play();
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

