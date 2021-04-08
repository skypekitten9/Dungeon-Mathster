#include "PlayerManager.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Player recieved a point! Score: %s"), *FString::FromInt(Score));
	Score++;
}

void UPlayerManager::SaveScore()
{

}

int32 UPlayerManager::GetScore()
{
	return Score;
}

