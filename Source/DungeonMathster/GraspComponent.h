#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GraspComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API UGraspComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGraspComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	void SetupPhysicsHandle();
	void SetupInputComponent();
	void Grasp();
	void Release();
	void Throw();
	FRotator GetPhysicsRotatorOffset();
	FHitResult GetActorWithinReach();
	FVector GetTargetPositionOnGrabbedActor();
	FVector GetTargetPosition();
	FVector GetPlayerViewPos();

	FRotator ActorRotationAtPickUp;
	UInputComponent* InputComponent = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	float DistanceToGrabbedActor = 0.f;
	UPROPERTY(EditAnywhere) float Reach = 200.f;
	UPROPERTY(EditAnywhere) float ArmLength = 75.f;
	UPROPERTY(EditAnywhere) float ThrowForce = 100000.f;
};
