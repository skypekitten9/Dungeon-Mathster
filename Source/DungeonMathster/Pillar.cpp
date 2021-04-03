#include "Pillar.h"
#include "Math/UnrealMathUtility.h"

#define OUT
#define NULLGUARD

// Sets default values for this component's properties
UPillar::UPillar()
{
	PrimaryComponentTick.bCanEverTick = true;
}


bool UPillar::IsPillarActivated()
{
	return Activated;
}

// Called when the game starts
void UPillar::BeginPlay()
{
	Super::BeginPlay();
	VerifyPointers();
	SetupPositions();
}


void UPillar::SetupPositions()
{
	InitialPos = GetOwner()->GetActorLocation();
	CurrentPos = InitialPos;
	TargetPos = InitialPos;
	TargetPos.Z = InitialPos.Z - UnitsToLower;
	InitialPosActorOnPillar = ActorOnPillar->GetActorLocation();
	InitialRotActorOnPillar = ActorOnPillar->GetActorRotation();

	UE_LOG(LogTemp, Warning, TEXT("Current: %s"), *CurrentPos.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Target: %s"), *TargetPos.ToString());
}

void UPillar::VerifyPointers()
{
	if (NULLGUARD !ActorOnPillar)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'ActorOnPillar'"), *(GetOwner()->GetName()));
	}
	if (NULLGUARD !TriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TriggerVolume'"), *(GetOwner()->GetName()));
	}
}

void UPillar::ActivatePillar()
{
	Activated = true;
	InProgress = true;
}

void UPillar::Progress(float DeltaTime)
{
	FVector ToMove = CurrentPos;
	CurrentPos = GetOwner()->GetActorLocation();
	ToMove.Z = FMath::FInterpTo(CurrentPos.Z, TargetPos.Z, DeltaTime, Speed);
	GetOwner()->SetActorLocation(ToMove);
	if (FMath::IsNearlyEqual(ToMove.Z, TargetPos.Z, 1.f))
	{
		InProgress = false;
		UE_LOG(LogTemp, Display, TEXT("Pillar done!"));
	}
}

void UPillar::Reset()
{
	//Resets
	GetOwner()->SetActorLocation(InitialPos);
	Activated = false;
	if (NULLGUARD !ActorOnPillar) return;
	ActorOnPillar->SetActorLocation(InitialPosActorOnPillar);
	ActorOnPillar->SetActorRotation(InitialRotActorOnPillar);
}

// Called every frame
void UPillar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TriggerVolume->IsOverlappingActor(ActorOnPillar) == false && Activated == false) ActivatePillar();
	if (Activated && InProgress) Progress(DeltaTime);
}

