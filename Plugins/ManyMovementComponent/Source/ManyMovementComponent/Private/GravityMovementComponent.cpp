// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityMovementComponent.h"

UGravityMovementComponent::UGravityMovementComponent(){}

void UGravityMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// 初始时设置星球大小
	GetOwner()->SetActorScale3D(FVector(Diameter, Diameter, Diameter));
}

void UGravityMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGravityMovementComponent::ApplyCalculateVectorForceEveryFrame(float DeltaTime)
{
	Super::ApplyCalculateVectorForceEveryFrame(DeltaTime);
	
	// 计算引力
	TSet<AActor*> OverlapActors;
	GetOwner()->GetOverlappingActors(OverlapActors, AActor::StaticClass());
	if (!OverlapActors.IsEmpty())
	{
		VectorForce = CalculateAllVectorForce(OverlapActors);
	}
}

FVector UGravityMovementComponent::CalculateVectorForce(AActor* OtherActor)
{
	FVector OwnerLocation = GetOwner()->GetActorLocation() * 0.01;
	FVector OtherLocation = OtherActor->GetActorLocation() * 0.01;
	UGravityMovementComponent* GravityMovementComponent = OtherActor->GetComponentByClass<UGravityMovementComponent>();
	if (GravityMovementComponent)
	{
		FVector TwoBodyVector = OtherLocation - OwnerLocation;
		double TwoBodyLength = TwoBodyVector.Length();
		if (TwoBodyLength < Diameter * 0.5 + GravityMovementComponent->Diameter * 0.5)
		{
			// 融合湮灭
			MergeDestroy(OtherActor);
		}
		double Force = CalculateForce(Mass, GravityMovementComponent->Mass, TwoBodyLength);
		return TwoBodyVector.GetSafeNormal() * Force;
	}
	return FVector(0, 0, 0);
}

double UGravityMovementComponent::CalculateForce(double MassA, double MassB, double Distance)
{
	return GravitationalConstant * MassA * MassB / (Distance * Distance);
}

FVector UGravityMovementComponent::CalculateAllVectorForce(TSet<AActor*> OtherActors)
{
	FVector AllVectorForce = FVector(0, 0, 0);
	for (AActor* OtherActor : OtherActors)
	{
		AllVectorForce += CalculateVectorForce(OtherActor);
	}
	return AllVectorForce;
}

void UGravityMovementComponent::MergeDestroy(AActor* OtherActor)
{
	UGravityMovementComponent* OtherGravityMovementComp = OtherActor->GetComponentByClass<UGravityMovementComponent>();

	AActor* OwnerActor = GetOwner();
	// 计算合体后的新质心位置
	OwnerActor->SetActorLocation((OwnerActor->GetActorLocation() * Mass + OtherActor->GetActorLocation() * OtherGravityMovementComp->Mass) / (Mass + OtherGravityMovementComp->Mass));
	// 计算新的体积
	Diameter = FMath::Pow(FMath::Pow(Diameter, 3) + FMath::Pow(OtherGravityMovementComp->Diameter, 3), 1.0 / 3.0);
	OwnerActor->SetActorScale3D(FVector(Diameter, Diameter, Diameter));
	// 计算合体后的速度 目前因帧问题，有一些微小误差
	Velocity = (Mass * Velocity + OtherGravityMovementComp->Mass * OtherGravityMovementComp->Velocity) / (Mass + OtherGravityMovementComp->Mass);
	// 计算合体后的加速度
	Acceleration += OtherGravityMovementComp->Acceleration;
	// 计算合体后的质量
	Mass += OtherGravityMovementComp->Mass;
	// 计算合体后的综合受力
	VectorForce += OtherGravityMovementComp->VectorForce;
	MergeExtraEvent(OtherActor);
	OtherActor->Destroy();
}
