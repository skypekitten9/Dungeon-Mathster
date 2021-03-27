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

void URoom::VerifyTriggerVolume()
{
	if (NULLGUARD !TriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TriggerVolume'."), *(GetOwner()->GetName()));
	}
}
#pragma endregion Setup

// Called every frame
void URoom::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (DoorComponent && DoorComponent->IsOpen() == false)
	{
		DoorComponent->Open();
	}
}

