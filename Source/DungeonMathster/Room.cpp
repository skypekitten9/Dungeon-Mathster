#include "Room.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#define OUT
#define NULLGUARD

// Sets default values for this component's properties
URoom::URoom()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void URoom::BeginPlay()
{
	Super::BeginPlay();
	SetupPlayer();
	SetupDoor();
	VerifyTriggerVolume();
	SetupPillars();
	SetupAnswers();
}

#pragma region Setup
void URoom::SetupDoor()
{
	if (NULLGUARD !DoorActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'Door'."), *(GetOwner()->GetName()));
		return;
	}
	DoorComponent = DoorActor->FindComponentByClass<UDoor>();
	if (NULLGUARD !DoorComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'DoorComponent'."), *(GetOwner()->GetName()));
	}
}

void URoom::SetupPlayer()
{
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (NULLGUARD !Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'Player'."), *(GetOwner()->GetName()));
	}
}

void URoom::SetupAnswers()
{
	int32 CorrectPillarIndex = FMath::RandRange(0, PillarComponents.Num()-1);
	PillarComponents[CorrectPillarIndex]->Answer = CorrectAnswer;
	UE_LOG(LogTemp, Warning, TEXT("Pillar has answer %s"), *(FString::FromInt(CorrectPillarIndex)));

	for (int32 i = 0; i < PillarComponents.Num(); i++)
	{
		if (i == CorrectPillarIndex) continue;
		PillarComponents[i]->Answer = CorrectAnswer + 1;
	}
}

void URoom::VerifyTriggerVolume()
{
	if (NULLGUARD !TriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TriggerVolume'."), *(GetOwner()->GetName()));
	}
}

void URoom::SetupPillars()
{
	if (PillarActors.Num() <= 0) UE_LOG(LogTemp, Error, TEXT("Room 's%' needs atleast 1 pillar attatched."), *(GetOwner()->GetName()));
	for (int32 i = 0; i < PillarActors.Num(); i++)
	{
		if (NULLGUARD PillarActors[i] == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Room '%s' has a null pillar."), *(GetOwner()->GetName()));
		}
		else
		{
			PillarComponents.Add(PillarActors[i]->FindComponentByClass<UPillar>());
		}
	}
}

bool URoom::IsCorrectPillarActivated()
{
	for (int32 i = 0; i < PillarComponents.Num(); i++)
	{
		if (PillarComponents[i] && PillarComponents[i]->IsPillarActivated() && PillarComponents[i]->Answer == CorrectAnswer) return true;
	}
	return false;
}

#pragma endregion Setup

// Called every frame
void URoom::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TriggerVolume && TriggerVolume->IsOverlappingActor(Player))
	{
		if (DoorComponent && DoorComponent->IsOpen() == false && IsCorrectPillarActivated())
		{
			DoorComponent->Open();
		}
	}
	else
	{
		if (DoorComponent && DoorComponent->IsOpen() == true)
		{
			DoorComponent->Close();
		}
	}
}

