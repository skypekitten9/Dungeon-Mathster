#include "GraspComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

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
	FHitResult HitResult = GetActorWithinReach();
	if (NULLGUARD HitResult.GetActor())
	{
		if(NULLGUARD PhysicsHandle) PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, GetTargetPosition(), FRotator::ZeroRotator);
		ActorRotationAtPickUp = GetOwner()->GetActorRotation();
	}
}

void UGraspComponent::Release()
{
	if (NULLGUARD PhysicsHandle) PhysicsHandle->ReleaseComponent();
}

void UGraspComponent::Throw()
{
	UE_LOG(LogTemp, Warning, TEXT("Threw!"));
}

FRotator UGraspComponent::GetPhysicsRotatorOffset()
{
	FRotator Result = FRotator::ZeroRotator;
	Result.Yaw = GetOwner()->GetActorRotation().Yaw - ActorRotationAtPickUp.Yaw;
	return Result;
}

FHitResult UGraspComponent::GetActorWithinReach()
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetPlayerViewPos(),
		GetTargetPosition(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(TEXT(""), false, GetOwner())
	);
	return HitResult;
}

FVector UGraspComponent::GetTargetPosition()
{
	FVector PlayerViewPos;
	FRotator PlayerViewRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPos,
		OUT PlayerViewRotator
	);
	return PlayerViewPos + PlayerViewRotator.Vector() * Reach;
}

FVector UGraspComponent::GetPlayerViewPos()
{
	FVector result;
	FRotator temp;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT result,
		OUT temp
	);
	return result;
}

void UGraspComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (NULLGUARD PhysicsHandle) PhysicsHandle->SetTargetLocationAndRotation(GetTargetPosition(), GetPhysicsRotatorOffset());
}

