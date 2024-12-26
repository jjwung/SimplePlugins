// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyMovementComponent.h"

UFlyMovementComponent::UFlyMovementComponent()
{
}

void UFlyMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFlyMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType, ThisTickFunction);

	VectorForce = CalForceVector(ForceDirection, ForceLength);
}

FVector UFlyMovementComponent::CalForceVector(FVector _ForceDirection, double _ForceLength)
{
	return _ForceDirection * _ForceLength;
}

void UFlyMovementComponent::AddMovementInput(float)
{
}
