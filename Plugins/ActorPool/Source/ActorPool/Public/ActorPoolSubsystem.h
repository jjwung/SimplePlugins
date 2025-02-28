// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ActorPoolSubsystem.generated.h"

// 对象池接口（可选）
UINTERFACE()
class UPoolableInterface : public UInterface
{
	GENERATED_BODY()
};

class IPoolableInterface
{
	GENERATED_BODY()

public:
	virtual void OnPoolableActivate() = 0;
	virtual void OnPoolableDeactivate() = 0;
};

// 对象池配置
USTRUCT(BlueprintType)
struct FActorPoolConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 InitialPoolSize = 10;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 MaxPoolSize = 50;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bAutoExpand = true;
};

UCLASS()
class ACTORPOOL_API UActorPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 初始化对象池
	UFUNCTION(BlueprintCallable)
	void InitializePool(TSubclassOf<AActor> ActorClass, const FActorPoolConfig& PoolConfig);

	// 从池中获取Actor
	UFUNCTION(BlueprintCallable)
	AActor* AcquireActor(
		TSubclassOf<AActor> ActorClass,
		const FTransform& SpawnTransform,
		ESpawnActorCollisionHandlingMethod CollisionHandling = ESpawnActorCollisionHandlingMethod::Undefined);

	// 回收Actor到池中
	UFUNCTION(BlueprintCallable)
	void ReleaseActor(AActor* ActorToRelease);

	// 清空指定类型的对象池
	UFUNCTION(BlueprintCallable)
	void ClearPool(TSubclassOf<AActor> ActorClass);

protected:
	virtual void Deinitialize() override;

private:
	// 对象池数据结构
	TMap<TSubclassOf<AActor>, TArray<AActor*>> InactiveActors;
	TMap<TSubclassOf<AActor>, FActorPoolConfig> PoolConfigs;

	AActor* CreateNewActor(TSubclassOf<AActor> ActorClass);
	void SetupActor(AActor* Actor, bool bActivate);
	void ResetActorState(AActor* Actor);
};