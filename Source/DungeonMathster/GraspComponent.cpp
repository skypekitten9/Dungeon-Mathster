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
	SetupCameraManager();
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

void UGraspComponent::SetupCameraManager()
{
	CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (NULLGUARD !CameraManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor '%s' is missing a 'CameraManager'."));
		return;
	}
}

void UGraspComponent::Grasp()
{
	FHitResult HitResult = GetActorWithinReach();
	if (NULLGUARD HitResult.GetActor())
	{
		if(NULLGUARD PhysicsHandle) PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, HitResult.GetActor()->GetActorLocation(), FRotator::ZeroRotator);
		ActorRotationAtPickUp = GetOwner()->GetActorRotation();
	}
}

void UGraspComponent::Release()
{
	if (NULLGUARD PhysicsHandle) PhysicsHandle->ReleaseComponent();
}

void UGraspComponent::Throw()
{
	UPrimitiveComponent* PrimitiveToThrow = PhysicsHandle->GetGrabbedComponent();
	if (NULLGUARD PhysicsHandle) PhysicsHandle->ReleaseComponent();
	if (NULLGUARD PrimitiveToThrow && CameraManager) PrimitiveToThrow->AddForce(CameraManager->GetCameraRotation().Vector() * ThrowForce);
}

FRotator UGraspComponent::GetPhysicsRotatorOffset()
{
	FRotator Result = FRotator::ZeroRotator;
	Result = GetOwner()->GetActorRotation() - ActorRotationAtPickUp;
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

FVector UGraspComponent::GetTargetPositionOnGrabbedActor()
{
	FVector PlayerViewPos;
	FRotator PlayerViewRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPos,
		OUT PlayerViewRotator
	);
	return PlayerViewPos + PlayerViewRotator.Vector() * ArmLength;
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
	if (NULLGUARD PhysicsHandle) PhysicsHandle->SetTargetLocationAndRotation(GetTargetPositionOnGrabbedActor(), GetPhysicsRotatorOffset());
}

