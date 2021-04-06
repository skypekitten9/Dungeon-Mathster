// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBox.h"
#include "Components/TextRenderComponent.h"
#include "Pillar.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API UPillar : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPillar();
	void SetAnswer(FString AnswerToSet, bool IsCorrect);
	FString GetAnswer();
	bool IsPillarActivated();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FString Answer = "";
	bool CallGhostOnActivation = true;
	bool Activated = false;
	bool InProgress = false;
	UPROPERTY(EditAnywhere) AActor* ActorOnPillar = nullptr;
	UPROPERTY(EditAnywhere) ATriggerBox* TriggerVolume = nullptr;
	UPROPERTY(EditAnywhere) float UnitsToLower = 30.f;
	UPROPERTY(EditAnywhere) float Speed = 2.f;
	UTextRenderComponent* TextComponent = nullptr;
	FVector InitialPos, CurrentPos, TargetPos, InitialPosActorOnPillar;
	FRotator InitialRotActorOnPillar;
	void SetupPositions();
	void VerifyPointers();
	void ActivatePillar();
	void Progress(float DeltaTime);
	void Reset();
		
};
