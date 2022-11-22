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

// Called when the game starts or when spawned
void AHoverShip::BeginPlay()
{
	Super::BeginPlay();
	pidController = FindComponentByClass<UPIDController>();
	//ship = FindComponentByClass<UStaticMeshComponent>();
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

//Call these functions in the blueprint

void AHoverShip::Acceleration(float axisInput)
{
	currentSpeed = axisInput  >= 0.01f && isOnRoadTrack ? FMath::Clamp(currentSpeed += 0.01f * accelerationConstant * physicsDeltaTime * 100, 0,1)
				: FMath::Clamp(currentSpeed -= 0.01f * decelerationConstant * physicsDeltaTime * 100,0,1);
	ship->AddForce(GetActorForwardVector() * (maxSpeed * currentSpeed) - _drag);
}

void AHoverShip::Brake(float axisInput)
{
	
}

void AHoverShip::Steer(float axisInput)
{
	
}

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

		const float forcePercent = pidController->Seek(hoverHeight, height, deltaTime);
		const FVector force = groundNormal * hoverMultiplier * forcePercent;
		const FVector gravity = -groundNormal * downForceMultiplier * (height / 100);

		ship->AddForce(force);
		ship->AddForce(gravity);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, "Is on Road-track");
	}
	else
	{
		groundNormal = FVector::UpVector;
		const FVector gravity = -groundNormal * downForceMultiplier;
		ship->AddForce(gravity);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, "Is not on Road-track");
	}
}

void AHoverShip::SideThrust()
{
}

void AHoverShip::Initialize(UStaticMeshComponent* physicsObject)
{
	ship = physicsObject;
}

