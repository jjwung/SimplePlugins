// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManyMovementComponentBase.h"
#include "FlyMovementComponent.generated.h"

/**
 * �����������ƶ����
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class MANYMOVEMENTCOMPONENT_API UFlyMovementComponent : public UManyMovementComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFlyMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** �˶����ٷ��� */
	UPROPERTY(BlueprintReadWrite)
	FVector ForceDirection;

	/** ������С */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	double ForceLength;

public:
	/** �˶����ٷ��� */
	FVector CalForceVector(FVector _ForceDirection, double _ForceLength);

	/** �������� */
	void AddMovementInput(float);
};
