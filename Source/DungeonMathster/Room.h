// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Questions.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBox.h"
#include "Door.h"
#include "Pillar.h"
#include "Room.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API URoom : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoom();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SetupDoor();
	void SetupPlayer();
	void SetupAnswers();
	void VerifyTriggerVolume();
	void SetupPillars();
	bool IsCorrectPillarActivated();
	UPROPERTY(EditAnywhere) TArray<AActor*> PillarActors;
	UPROPERTY(EditAnywhere) ATriggerBox* TriggerVolume = nullptr;
	UPROPERTY(EditAnywhere) AActor* DoorActor = nullptr;
	int32 CorrectAnswer = 0;
	TArray<UPillar*> PillarComponents;
	UDoor* DoorComponent = nullptr;
	AActor* Player = nullptr;
};
