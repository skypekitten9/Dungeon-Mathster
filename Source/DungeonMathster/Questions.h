// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Array.h"
#include "Math/UnrealMathUtility.h"
#include "CoreMinimal.h"

class Question
{
public:
	FString Query;
	int32 CorrectAnswer, WrongAnswerA, WrongAnswerB;
	Question(FString Query, int32 CorrectAnswer, int32 WrongAnswerA, int32 WrongAnswerB) :
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



