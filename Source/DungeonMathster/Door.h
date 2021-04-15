#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Sound/SoundBase.h"
#include "Door.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API UDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDoor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Close();
	void Open();
	bool IsOpen();

protected:
	virtual void BeginPlay() override;

private:
	void PlaySound(bool open);
	void Progress(float DeltaTime);
	void SetupRotators();
	void SetupSound();

	UPROPERTY(EditAnywhere) float CloseSpeed = 2;
	UPROPERTY(EditAnywhere) float DegreesToOpen = 90.f;
	UPROPERTY(EditAnywhere) float OpenSpeed = 2;
	UPROPERTY(EditAnywhere) TArray<USoundBase*> CloseSounds;
	UPROPERTY(EditAnywhere) TArray<USoundBase*> OpenSounds;

	bool InProgress = false;
	bool Opened = false;
	FRotator CurrentRotation;
	FRotator InitialRotation;
	FRotator TargetRotation;
	UAudioComponent* AudioComponent = nullptr;
};
