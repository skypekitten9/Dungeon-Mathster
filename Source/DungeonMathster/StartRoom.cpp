#include "StartRoom.h"

#define NULLGUARD

UStartRoom::UStartRoom()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UStartRoom::BeginPlay()
{
	Super::BeginPlay();
	SetupDoor();
	SetupPlayer();
	VerifyTriggerVolume();
	SetupTextComponent();

	LoadHighscore();
}


void UStartRoom::SetupDoor()
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

void UStartRoom::SetupPlayer()
{
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (NULLGUARD !Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'Player'."), *(GetOwner()->GetName()));
		return;
	}
}

void UStartRoom::VerifyTriggerVolume()
{
	if (NULLGUARD !TriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TriggerVolume'."), *(GetOwner()->GetName()));
	}
}

void UStartRoom::SetupTextComponent()
{
	TextComponent = GetOwner()->FindComponentByClass<UTextRenderComponent>();
	if (NULLGUARD !TextComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TextComponent'"), *(GetOwner()->GetName()));
	}
}

void UStartRoom::LoadHighscore()
{
	UHighscoreSave* LoadGame = Cast<UHighscoreSave>(UGameplayStatics::CreateSaveGameObject(UHighscoreSave::StaticClass()));
	LoadGame = Cast<UHighscoreSave>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
	int32 Highscore = LoadGame->Score;
	TextComponent->SetText(FText::AsNumber(Highscore));
}

void UStartRoom::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TriggerVolume && TriggerVolume->IsOverlappingActor(Player))
	{
		if (DoorComponent && DoorComponent->IsOpen() == false)
		{
			DoorComponent->Open();
		}
	}
	else
	{
		if (DoorComponent && DoorComponent->IsOpen())
		{
			DoorComponent->Close();
		}
	}
}

