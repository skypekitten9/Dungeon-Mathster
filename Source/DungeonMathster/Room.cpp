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


void URoom::Reset()
{
	HasReset = true;
	SetupAnswers();
	for (UPillar* p : PillarComponents)
	{
		if(NULLGUARD p) p->Reset();
	}
}

// Called when the game starts
void URoom::BeginPlay()
{
	Super::BeginPlay();
	SetupPlayer();
	SetupDoor();
	VerifyTriggerVolume();
	SetupPillars();
	SetupTextComponent();

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
	Question Quary = Questions::GetRandomQuestion();
	CorrectAnswer = Quary.CorrectAnswer;
	if (NULLGUARD TextComponent) TextComponent->SetText(FText::FromString(Quary.Query));
	TArray<FString> IncorrectAnswers = { Quary.WrongAnswerA, Quary.WrongAnswerB };

	int32 CorrectPillarIndex = FMath::RandRange(0, PillarComponents.Num()-1);
	PillarComponents[CorrectPillarIndex]->SetAnswer(Quary.CorrectAnswer, true);

	if (PillarActors.Num() > IncorrectAnswers.Num() + 1)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has too many pillars attached."), *(GetOwner()->GetName()));
		return;
	}
	int32 j = 0;
	for (int32 i = 0; i < PillarComponents.Num(); i++)
	{
		if (i == CorrectPillarIndex) continue;
		PillarComponents[i]->SetAnswer(IncorrectAnswers[j], false);
		j++;
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
		if (NULLGUARD !PillarActors[i])
		{
			UE_LOG(LogTemp, Error, TEXT("Room '%s' has a null pillar."), *(GetOwner()->GetName()));
		}
		else
		{
			PillarComponents.Add(PillarActors[i]->FindComponentByClass<UPillar>());
		}
	}
}

void URoom::SetupTextComponent()
{
	TextComponent = GetOwner()->FindComponentByClass<UTextRenderComponent>();
	if (NULLGUARD !TextComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TextComponent'"), *(GetOwner()->GetName()));
	}
}

bool URoom::IsCorrectPillarActivated()
{
	for (int32 i = 0; i < PillarComponents.Num(); i++)
	{
		if (PillarComponents[i] && PillarComponents[i]->IsPillarActivated() && CorrectAnswer.Equals(PillarComponents[i]->GetAnswer())) return true;
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
		ResetTimer = TimeBeforeReset;
		HasReset = false;
		if (DoorComponent && DoorComponent->IsOpen() == false && IsCorrectPillarActivated())
		{
			DoorComponent->Open();
		}
	}
	else
	{
		ResetTimer -= DeltaTime;
		if (ResetTimer <= 0.f && HasReset == false) Reset();
		if (DoorComponent && DoorComponent->IsOpen() == true)
		{
			DoorComponent->Close();
		}
	}
}

