#include "PlayerManager.h"
#include "Kismet/GameplayStatics.h"
#include "HighscoreSave.h"

UPlayerManager::UPlayerManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerManager::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerManager::IncreaseScore()
{
	Score++;
}

void UPlayerManager::KillPlayer()
{
	int CurrentScore = Score;
	LoadScore();
	if (Score < CurrentScore) SaveScore();
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void UPlayerManager::SaveScore()
{
	UHighscoreSave* SavedGame = Cast<UHighscoreSave>(UGameplayStatics::CreateSaveGameObject(UHighscoreSave::StaticClass()));
	SavedGame->Score = Score;
	UGameplayStatics::SaveGameToSlot(SavedGame, TEXT("MySlot"), 0);
}

void UPlayerManager::LoadScore()
{
	UHighscoreSave* LoadGame = Cast<UHighscoreSave>(UGameplayStatics::CreateSaveGameObject(UHighscoreSave::StaticClass()));
	LoadGame = Cast<UHighscoreSave>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
	Score = LoadGame->Score;
}

int32 UPlayerManager::GetScore()
{
	return Score;
}

