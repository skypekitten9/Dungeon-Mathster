#pragma once
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "PlayerManager.h"
#include "Sound/SoundBase.h"
#include "Ghost.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API UGhost : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGhost();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void IncreaseSpeed();
	void SetTarget(FVector NewTarget);

protected:
	virtual void BeginPlay() override;

private:
	void GoTowardsTarget(float DeltaTime);
	void LookTowardsPlayer();
	void PlaySound();
	void ProgressEndingGame(float DeltaTime);
	void SetupPlayer();
	void SetupSound();
	void TryCatchingPlayer();
	void VerifyTarget();

	UPROPERTY(EditAnywhere) float EndGameTimer = 1.5f;
	UPROPERTY(EditAnywhere) float InitialSpeed = 50.f;
	UPROPERTY(EditAnywhere) float MaxSpeed = 300.f;
	UPROPERTY(EditAnywhere) float ModelYawOffset = -90.f;
	UPROPERTY(EditAnywhere) float ModelZOffset = 50.f;
	UPROPERTY(EditAnywhere) float Reach = 250.f;
	UPROPERTY(EditAnywhere) float SpeedIncreasePerIncrement = 10.f;
	UPROPERTY(EditAnywhere) float TargetRoomSpeed = 400.f;
	UPROPERTY(EditAnywhere) TArray<USoundBase*> Sounds;

	APawn* Player = nullptr;
	bool PlayerCaught = false;
	bool TargetingActive = false;
	bool TargetPlayer = false;
	float Speed;
	FVector CaughtPlayerLocation;
	FVector Target;
	int SpeedIncrement = 0;
	UAudioComponent* AudioComponent = nullptr;
	UPlayerManager* PlayerManager = nullptr;
};
