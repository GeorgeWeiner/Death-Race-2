// Fill out your copyright notice in the Description page of Project Settings.


#include "PID_Controller.h"

#include <ThirdParty/Vulkan/Include/vulkan/vulkan_core.h>

PID_Controller::PID_Controller()
{
	
}

PID_Controller::~PID_Controller()
{
	
}

float PID_Controller::Seek(float seekValue, float currentValue, float deltaTime)
{
	auto Proportional = seekValue - currentValue;

	auto Derivative = (Proportional - _lastProportional) / deltaTime;
	_integral += Proportional * deltaTime;
	_lastProportional = Proportional;

	auto value = pCoeff * Proportional + iCoeff * _integral + dCoeff * Derivative;
	value = FMath::Clamp(value, minimum, maximum);
	
	return value;
}
