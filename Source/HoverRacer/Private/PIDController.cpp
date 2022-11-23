// Fill out your copyright notice in the Description page of Project Settings.


#include "PIDController.h"

#include <string>

// Sets default values for this component's properties
UPIDController::UPIDController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPIDController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPIDController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UPIDController::Seek(float seekValue, float currentValue, float deltaTime)
{
	const float Proportional = seekValue - currentValue;

	const float Derivative = (Proportional - _lastProportional) / deltaTime;
	_integral += Proportional * deltaTime;
	_lastProportional = Proportional;

	float value = pCoeff * Proportional + iCoeff * _integral + dCoeff * Derivative;
	value = FMath::Clamp(value, minimum, maximum);
	
	return value;
}

