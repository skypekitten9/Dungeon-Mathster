// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighscoreSave.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONMATHSTER_API UHighscoreSave : public USaveGame
{
	GENERATED_BODY()
public:
	UHighscoreSave();

	UPROPERTY(EditAnywhere)
	int32 Score;
};
