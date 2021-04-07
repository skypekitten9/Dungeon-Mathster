#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
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
	void SetupCameraManager();
	void Grasp();
	void Release();
	void Throw();
	FRotator GetPhysicsRotatorOffset();
	FHitResult GetActorWithinReach();
	FVector GetTargetPosition();
	FVector GetPlayerViewPos();

	FRotator ActorRotationAtPickUp;
	UInputComponent* InputComponent = nullptr;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	APlayerCameraManager* CameraManager = nullptr;
	UPROPERTY(EditAnywhere) float Reach = 100.f;
	UPROPERTY(EditAnywhere) float ThrowForce = 100000.f;
};
