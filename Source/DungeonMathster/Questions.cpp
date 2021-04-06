// Fill out your copyright notice in the Description page of Project Settings.
#include "Questions.h"

const TArray<Question> Questions::QuestionArray = {
		Question("How many legs does a spider have?", "10","8", "12"),
		Question("How many legs does a dog have?", "4","5", "7"),
		Question("How many legs does an ant have?", "6","8", "12")
};

const Question Questions::GetRandomQuestion()
{
	return QuestionArray[FMath::RandRange(0, QuestionArray.Num() - 1)];
}
