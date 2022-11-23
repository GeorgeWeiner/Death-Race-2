// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PIDController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOVERRACER_API UPIDController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPIDController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pCoeff = 0.0016f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float iCoeff = 0.000004f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dCoeff = 0.004f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minimum = -3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maximum = 3.0f;

	UFUNCTION(BlueprintCallable)
	float Seek(float seekValue, float currentValue, float deltaTime);

private:
	float _integral;
	float _lastProportional;
};
