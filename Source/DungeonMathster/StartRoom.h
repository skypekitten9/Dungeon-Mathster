// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBox.h"
#include "Components/TextRenderComponent.h"
#include "Door.h"
#include "StartRoom.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API UStartRoom : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStartRoom();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	void SetupDoor();
	void SetupPlayer();
	void VerifyTriggerVolume();
	void SetupTextComponent();
	UPROPERTY(EditAnywhere) AActor* DoorActor = nullptr;
	UPROPERTY(EditAnywhere) ATriggerBox* TriggerVolume = nullptr;
	UDoor* DoorComponent = nullptr;
	UTextRenderComponent* TextComponent = nullptr;
	AActor* Player = nullptr;
};
