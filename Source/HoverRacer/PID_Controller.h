// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class HOVERRACER_API PID_Controller 
{
public:
	PID_Controller();
	~PID_Controller();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pCoeff = 0.08f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float iCoeff = 0.0002f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dCoeff = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minimum = -1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maximum = 1.0f;

private:
	float _integral;
	float _lastProportional;

public:
	float Seek(float seekValue, float currentValue, float deltaTime);
};
