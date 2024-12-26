// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManyMovementComponentBase.generated.h"


UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MANYMOVEMENTCOMPONENT_API UManyMovementComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UManyMovementComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** 物体的速度向量，单位m/s */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	/** 物体的加速度向量，单位m/s² */
	UPROPERTY(BlueprintReadOnly)
	FVector Acceleration;

	/** 物体的质量，单位kg */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Mass;

	/** 物体当前的综合受力，单位N */
	UPROPERTY(BlueprintReadOnly)
	FVector VectorForce;

public:
	/** 物体的速度大小 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	double GetVelocityLength();

	/** 计算受力向量 */
	FVector CalculateVectorForce();

	/** 计算加速度 */
	FVector CalculateAcceleration(double _Mass, FVector _ForceVector);

	/** 应用计算每帧的受力向量 */
	virtual void ApplyCalculateVectorForceEveryFrame(float DeltaTime);
	
	/** 应用每帧的移动位置 */
	void ApplyMovingEveryFrame(float DeltaTime);

	/** 通过每帧的加速度计算并应用到速度 */
	void ApplyVelocityByAccelerationEveryFrame(float DeltaTime);

	/** 通过每帧的综合受力和物体质量计算并应用到速度 */
	void ApplyAccelerationEveryFrame(float DeltaTime);

};
