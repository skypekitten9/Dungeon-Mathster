// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Ghost.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API UGhost : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGhost();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetTarget(FVector NewTarget);

protected:
	virtual void BeginPlay() override;

private:
	void SetupPlayer();
	void LookTowardsPlayer();
	void GoTowardsTarget(float DeltaTime);
	void ProgressEndingGame(float DeltaTime);
	UPROPERTY(EditAnywhere) float InitialSpeed = 1.f;
	UPROPERTY(EditAnywhere) float Reach = 250.f;
	UPROPERTY(EditAnywhere) float EndGameTimer = 1.5f;
	APawn* Player = nullptr;
	bool PlayerCaught = false;
	FVector Target;
	float Speed;
};
