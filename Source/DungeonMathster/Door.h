// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
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

protected:
	virtual void BeginPlay() override;

private:
	void SetUpRotators();
	void Progress(float DeltaTime);
	UPROPERTY(EditAnywhere) UAudioComponent* AudioComponent = nullptr;
	UPROPERTY(EditAnywhere) float DegreesToOpen = 90.f;
	UPROPERTY(EditAnywhere) float OpenSpeed = 2;
	UPROPERTY(EditAnywhere) float CloseSpeed = 2;
	FRotator InitialRotation, TargetRotation, CurrentRotation;
	bool InProgress = false;
};
