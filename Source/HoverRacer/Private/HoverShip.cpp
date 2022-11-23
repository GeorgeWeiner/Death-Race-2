// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverShip.h"

#include <string>

#include "Kismet/KismetMathLibrary.h"

FRotator MyLookRotation(FVector lookAt, FVector upDirection);

// Sets default values
AHoverShip::AHoverShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHoverShip::Initialize(UStaticMeshComponent* physicsObject)
{
	ship = physicsObject;
}

// Called when the game starts or when spawned
void AHoverShip::BeginPlay()
{
	Super::BeginPlay();
	pidController = FindComponentByClass<UPIDController>();
}

void AHoverShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AHoverShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/// <summary>
/// Checks if the current acceleration axis is above threshold, if true, increments interpolator of acceleration curve.
/// It also functions as an idle-slow-down, because if false, the interpolator decrements, resulting in a loss of movement speed.
/// </summary>
void AHoverShip::Acceleration(float axisInput)
{
	currentSpeed = axisInput  >= 0.01f && isOnRoadTrack ? FMath::Clamp(currentSpeed += 0.01f * accelerationConstant * physicsDeltaTime * 100, 0,1)
				: FMath::Clamp(currentSpeed -= 0.01f * decelerationConstant * physicsDeltaTime * 100,0,1);
	ship->AddForce(GetActorForwardVector() * (maxSpeed * currentSpeed) - _drag);
}

/// <summary>
/// Adds a force, towards the negative of the Z-Axis. This functions as a brake and a reverse gear at the same time.
/// </summary>
void AHoverShip::Brake(float axisInput)
{
	if (axisInput > 0.0f) return;

	currentBackwardsSpeed = axisInput <= -0.01f && isOnRoadTrack && currentSpeed <= 0.01f ?
		FMath::Clamp(currentBackwardsSpeed += 0.01f * accelerationConstant * physicsDeltaTime * 200, 0, 1)
		: FMath::Clamp(currentBackwardsSpeed -= 0.01f * decelerationConstant * physicsDeltaTime * 500, 0, 1);

	ship->AddForce(GetActorForwardVector() * (brakeForce * axisInput * physicsDeltaTime * 10));
}

/// <summary>
/// Applies the rotation of the local Y-Axis, by adding a torque towards an angle, resulting in somewhat realistic flying turn behavior.
/// The amount of torque is dependent on the current speed variable, as higher speed equals higher angular drag (not of the rigidbody but added locally here).
/// It then calculates the friction of the turn, by taking the dot-product of the current velocity * transform.right to get the magnitude of sidewards-movement.
/// Finally the calculated force gets applied via Acceleration.
/// </summary>
void AHoverShip::Steer(float axisInput)
{
	steeringAngle = GetActorUpVector() * ((steeringSpeed - speedDependentAngularDragMagnitude) * axisInput * physicsDeltaTime);
	ship->AddTorqueInDegrees(steeringAngle, NAME_None, true);

	FVector right = GetActorRightVector();
	float sidewaysSpeed = ship->GetComponentVelocity().Dot(right);

	FVector sideFriction = -right * (sidewaysSpeed / physicsDeltaTime / 2.0f);
	ship->AddForce(sideFriction);
}

/// <summary>
/// Uses PID Controller to create a constant balance between gravity (down on local y-axis) and force (up on local y-axis).
/// This creates a smooth flying experience. If it detects no ground underneath, it automatically uses the world y-axis as the gravitational direction.
/// The ship itself gets rotated along a Vector that gets projected on a plane. (With interpolation)
/// </summary>
void AHoverShip::AntiGravity(float deltaTime, FRotator targetRotation)
{
	if (ship == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Ship is null. returning.");
		return;
	}
	if (pidController == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "PID is null. returning.");
		return;
	}

	FVector groundNormal;
	if (isOnRoadTrack)
	{
		const float height = fHitResult.Distance;
		groundNormal = fHitResult.ImpactNormal;

		//...use the PID controller to determine the amount of hover force needed...
		const float forcePercent = pidController->Seek(hoverHeight, height, physicsDeltaTime);
		//...calculate the total amount of hover force based on normal (or "up") of the ground...
		const FVector force = groundNormal * hoverMultiplier * forcePercent;
		//...calculate the force and direction of gravity to adhere the ship to the track.
		const FVector gravity = -groundNormal * downForceMultiplier * (height / 100);

		//...and finally apply the hover and gravity forces
		ship->AddForce(force);
		ship->AddForce(gravity);
	}
	else
	{
		//...use Up to represent the ground normal.
		groundNormal = FVector::UpVector;

		//Calculate and apply the stronger falling gravity straight down on the ship
		const FVector gravity = -groundNormal * downForceMultiplier;
		ship->AddForce(gravity);
	}
}

/// <summary>
/// Adds force towards the steering direction, to make steering feel more responsive.
/// </summary>
void AHoverShip::SideThrust(float axisInput)
{
	ship->AddForce(GetActorRightVector() * (sideThrustAmount * axisInput * physicsDeltaTime * 100));
}

