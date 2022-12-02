// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PIDController.h"
#include "GameFramework/Pawn.h"
#include "HoverShip.generated.h"

UCLASS()
class HOVERRACER_API AHoverShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHoverShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="HoverShipControls")
	void Acceleration(float axisInput);
	UFUNCTION(BlueprintCallable, Category="HoverShipControls")
	void Brake(float axisInput);
	UFUNCTION(BlueprintCallable, Category="HoverShipControls")
	void Steer(float axisInput);
	UFUNCTION(BlueprintCallable, Category="HoverShipControls")
	void AntiGravity(float deltaTime, FRotator targetRotation);
	UFUNCTION(BlueprintCallable, Category="HoverShipControls")
	void SideThrust(float axisInput);
	UFUNCTION(BlueprintCallable, Category="ShipBase")
	void Initialize(UStaticMeshComponent* physicsObject);
	UFUNCTION(BlueprintCallable, Category="ShipBase")
	void AirBrakes(float axisInput);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acceleration and Braking")
	float accelerationConstant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acceleration and Braking")
	float maxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acceleration and Braking")
	float currentSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acceleration and Braking")
	float currentBackwardsSpeed;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acceleration and Braking")
	float brakeForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acceleration and Braking")
	float brakeMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acceleration and Braking")
	float decelerationConstant;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Acceleration and Braking")
	UCurveBase* accelerationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Steering")
	float sideThrustAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Steering")
	float steeringSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Steering")
	float speedDependentAngularDragMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anti Gravity")
	float downForceMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anti Gravity")
	float hoverMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anti Gravity")
	float hoverHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anti Gravity")
    float pidHeightThreshold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anti Gravity")
	float fallGravityMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anti Gravity")
	bool isOnRoadTrack;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Physics")
	float physicsDeltaTime = 0.05f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Track Information")
	//Do the layer mask thing here.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Track Information")
	float maxSphereCastDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Track Information")
	float sphereCastRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Track Information")
	FHitResult fHitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	float animationInterpolationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	float steerAnimationSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	float idleSteeringAnimationMultiplier;
	UPROPERTY(BlueprintReadOnly, Category="Animations")
	float animationTime;

	FVector steeringAngle;
	UPIDController* pidController;
	UPrimitiveComponent* ship;
	
	float _drag;
};
