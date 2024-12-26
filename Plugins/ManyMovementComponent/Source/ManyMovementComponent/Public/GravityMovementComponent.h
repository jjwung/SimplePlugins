// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManyMovementComponentBase.h"
#include "GravityMovementComponent.generated.h"

/** 重力常数G 单位：m³/kg·s² */
const double GravitationalConstant = 0.0000000000667259;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class MANYMOVEMENTCOMPONENT_API UGravityMovementComponent : public UManyMovementComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGravityMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** 物体的直径，单位m */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Diameter = 10;

public:
	/** 计算星球的每帧受力 */
	virtual void ApplyCalculateVectorForceEveryFrame(float DeltaTime) override;

	/** 计算某个天体和自身互相受力向量 */
	FVector CalculateVectorForce(AActor* OtherActor);

	/** 计算某个天体和自身互相受力大小 */
	double CalculateForce(double MassA, double MassB, double Distance);

	/** 计算自身对所有天体的综合受力 */
	FVector CalculateAllVectorForce(TSet<AActor*> OtherActors);

	/** 融合湮灭 */
	void MergeDestroy(AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
	void MergeExtraEvent(AActor* OtherActor);

};
