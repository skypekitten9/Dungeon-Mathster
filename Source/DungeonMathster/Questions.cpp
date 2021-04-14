// Fill out your copyright notice in the Description page of Project Settings.
#include "Questions.h"

const TArray<Question> Questions::QuestionArray = {
		Question("How many legs does a spider have?", "10", "8", "12"),
		Question("How many legs does a dog have?", "4", "5", "7"),
		Question("How many arms does a dog have?", "0", "4", "7"),
		Question("How many legs does an ant have?", "6", "8", "12"),
		Question("What letter is not included in 1 to 999?", "A", "E", "F"),
		Question("What is 2 + 5?", "7", "9", "6"),
		Question("What's 9 + 10?", "19", "21", "18"),
		Question("What's 9 * 10?", "90", "82", "100"),
		Question("What day in May is world bee day?", "20", "14", "6"),
		Question("What day in April is international jazz day?", "30", "4", "12"),
		Question("What day in June is world oceans day?", "8", "2", "16"),
		Question("What day in July is world chess day?", "20", "14", "6"),
		Question("What day in October is world habitat day?", "4", "15", "9"),
		Question("What day in July is world chess day?", "20", "14", "6"),
		Question("Do dogs have arms or legs?", "Legs", "Arms", "Both"),
		Question("Do cats have arms or legs?", "Legs", "Arms", "Both"),
		Question("Do fish have arms or legs?", "None", "Arms", "Both"),
		Question("Do humans have arms or legs?", "Both", "Arms", "Legs"),
		Question("How many legs does a cat have?", "4", "6", "12")
};

const Question Questions::GetRandomQuestion()
{
	return QuestionArray[FMath::RandRange(0, QuestionArray.Num() - 1)];
}
