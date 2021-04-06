// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Array.h"
#include "Math/UnrealMathUtility.h"
#include "CoreMinimal.h"

class Question
{
public:
	FString Query, CorrectAnswer, WrongAnswerA, WrongAnswerB;
	Question(FString Query, FString CorrectAnswer, FString WrongAnswerA, FString WrongAnswerB) :
		Query(Query),
		CorrectAnswer(CorrectAnswer),
		WrongAnswerA(WrongAnswerA),
		WrongAnswerB(WrongAnswerB) {};
};

class DUNGEONMATHSTER_API Questions
{
public:
	static const TArray<Question> QuestionArray;
	static const Question GetRandomQuestion();

private:
	Questions();
};



