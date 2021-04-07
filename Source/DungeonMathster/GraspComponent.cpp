#include "GraspComponent.h"

#define OUT
#define NULLGUARD

UGraspComponent::UGraspComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGraspComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupPhysicsHandle();
	SetupInputComponent();

}

void UGraspComponent::SetupPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (NULLGUARD !PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor '%s' is missing a 'PhysicsHandle'."));
	}
}

void UGraspComponent::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (NULLGUARD !InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor '%s' is missing a 'InputComponent'."));
		return;
	}
	InputComponent->BindAction("Grab", IE_Pressed, this, &UGraspComponent::Grasp);
	InputComponent->BindAction("Grab", IE_Released, this, &UGraspComponent::Release);
	InputComponent->BindAction("Throw", IE_Pressed, this, &UGraspComponent::Throw);
}

void UGraspComponent::Grasp()
{
	UE_LOG(LogTemp, Warning, TEXT("Grasped something!"));
}

void UGraspComponent::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
}

void UGraspComponent::Throw()
{
	UE_LOG(LogTemp, Warning, TEXT("Threw!"));
}

void UGraspComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

