#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighscoreSave.h"
#include "PlayerManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API UPlayerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerManager();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void IncreaseScore();
	int32 GetScore();

protected:
	virtual void BeginPlay() override;

private:
	void SaveScore();
	void LoadScore();
	int32 Score = 0;
};
