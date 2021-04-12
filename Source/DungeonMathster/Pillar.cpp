#include "Pillar.h"
#include "Math/UnrealMathUtility.h"

#define OUT
#define NULLGUARD

// Sets default values for this component's properties
UPillar::UPillar()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPillar::SetAnswer(FString AnswerToSet, bool IsCorrect)
{
	Answer = AnswerToSet;
	CallGhostOnActivation = !IsCorrect;
	if (NULLGUARD TextComponent) TextComponent->SetText(FText::FromString(Answer));
}

FString UPillar::GetAnswer()
{
	return Answer;
}

bool UPillar::IsPillarActivated()
{
	return Activated;
}

// Called when the game starts
void UPillar::BeginPlay()
{
	Super::BeginPlay();
	TextComponent = GetOwner()->FindComponentByClass<UTextRenderComponent>();
	PrimitiveComponent = ActorOnPillar->FindComponentByClass<UPrimitiveComponent>();
	VerifyPointers();
	SetupPositions();
	SetupGhost();
	if (NULLGUARD TextComponent) TextComponent->SetText(FText::FromString(Answer));
}


void UPillar::SetupPositions()
{
	InitialPos = GetOwner()->GetActorLocation();
	CurrentPos = InitialPos;
	TargetPos = InitialPos;
	TargetPos.Z = InitialPos.Z - UnitsToLower;
	InitialPosActorOnPillar = ActorOnPillar->GetActorLocation();
	InitialRotActorOnPillar = ActorOnPillar->GetActorRotation();
}

void UPillar::SetupGhost()
{
	TArray<AActor*> TempArrayOfGhost;
	FName Tag = TEXT("Ghost");
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, TempArrayOfGhost);
	if (TempArrayOfGhost.Num() > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s detected more then 1 'Ghost' in the map."), *(GetOwner()->GetName()));
		return;
	}
	if (TempArrayOfGhost.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s detected no 'Ghost' in map."), *(GetOwner()->GetName()));
		return;
	}
	Ghost = TempArrayOfGhost[0]->FindComponentByClass<UGhost>();
	if (NULLGUARD !Ghost)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a refferense to 'Ghost'."), *(GetOwner()->GetName()));
		return;
	}
}

void UPillar::VerifyPointers()
{
	if (NULLGUARD !ActorOnPillar)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'ActorOnPillar'"), *(GetOwner()->GetName()));
	}
	if (NULLGUARD !TriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TriggerVolume'"), *(GetOwner()->GetName()));
	}
	if (NULLGUARD !TextComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TextComponent'"), *(GetOwner()->GetName()));
	}
	if (NULLGUARD !PrimitiveComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'PrimitiveComponent'"), *(GetOwner()->GetName()));
	}
}

void UPillar::ActivatePillar()
{
	if (NULLGUARD Ghost && CallGhostOnActivation) Ghost->IncreaseSpeedIncrement();
	Activated = true;
	InProgress = true;
}

void UPillar::Progress(float DeltaTime)
{
	FVector ToMove = CurrentPos;
	CurrentPos = GetOwner()->GetActorLocation();
	ToMove.Z = FMath::FInterpTo(CurrentPos.Z, TargetPos.Z, DeltaTime, Speed);
	GetOwner()->SetActorLocation(ToMove);
	if (FMath::IsNearlyEqual(ToMove.Z, TargetPos.Z, 1.f))
	{
		InProgress = false;
		UE_LOG(LogTemp, Display, TEXT("Pillar done!"));
	}
}

void UPillar::Reset()
{
	//Resets
	GetOwner()->SetActorLocation(InitialPos);
	Activated = false;
	if (NULLGUARD !ActorOnPillar) return;
	ActorOnPillar->SetActorLocation(InitialPosActorOnPillar);
	ActorOnPillar->SetActorRotation(InitialRotActorOnPillar);
	if (NULLGUARD PrimitiveComponent) PrimitiveComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
}

// Called every frame
void UPillar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TriggerVolume->IsOverlappingActor(ActorOnPillar) == false && Activated == false) ActivatePillar();
	if (Activated && InProgress) Progress(DeltaTime);
}

