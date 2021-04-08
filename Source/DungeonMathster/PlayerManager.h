#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API UPlayerManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerManager();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void IncreaseScore();
	void SaveScore();
	int32 GetScore();

protected:
	virtual void BeginPlay() override;

private:
	int32 Score = 0;
};
