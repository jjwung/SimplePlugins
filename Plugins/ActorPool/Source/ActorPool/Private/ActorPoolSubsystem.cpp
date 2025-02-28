// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPoolSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UActorPoolSubsystem::InitializePool(TSubclassOf<AActor> ActorClass, const FActorPoolConfig& PoolConfig)
{
    if (!ActorClass) return;

    PoolConfigs.Add(ActorClass, PoolConfig);
    TArray<AActor*>& Pool = InactiveActors.FindOrAdd(ActorClass);

    for (int32 i = 0; i < PoolConfig.InitialPoolSize; ++i)
    {
        if (AActor* NewActor = CreateNewActor(ActorClass))
        {
            Pool.Add(NewActor);
        }
    }
}

AActor* UActorPoolSubsystem::AcquireActor(
    TSubclassOf<AActor> ActorClass,
    const FTransform& SpawnTransform,
    ESpawnActorCollisionHandlingMethod CollisionHandling)
{
    if (!ActorClass) return nullptr;

    TArray<AActor*>* Pool = InactiveActors.Find(ActorClass);
    if (!Pool) return nullptr;

    AActor* SpawnedActor = nullptr;

    // 从池中获取可用Actor
    if (Pool->Num() > 0)
    {
        SpawnedActor = Pool->Pop();
    }
    else if (PoolConfigs[ActorClass].bAutoExpand && 
            InactiveActors[ActorClass].Num() < PoolConfigs[ActorClass].MaxPoolSize)
    {
        SpawnedActor = CreateNewActor(ActorClass);
    }

    if (SpawnedActor)
    {
        SetupActor(SpawnedActor, true);
        UGameplayStatics::FinishSpawningActor(SpawnedActor, SpawnTransform);
        SpawnedActor->SetActorTransform(SpawnTransform, false, nullptr, 
                                      CollisionHandling != ESpawnActorCollisionHandlingMethod::Undefined ? 
                                      ETeleportType::ResetPhysics : ETeleportType::None);
    }

    return SpawnedActor;
}

void UActorPoolSubsystem::ReleaseActor(AActor* ActorToRelease)
{
    if (!ActorToRelease) return;

    TSubclassOf<AActor> ActorClass = ActorToRelease->GetClass();
    if (TArray<AActor*>* Pool = InactiveActors.Find(ActorClass))
    {
        if (Pool->Num() < PoolConfigs[ActorClass].MaxPoolSize)
        {
            SetupActor(ActorToRelease, false);
            Pool->Add(ActorToRelease);
        }
        else
        {
            ActorToRelease->SetLifeSpan(0.1f);
        }
    }
}

void UActorPoolSubsystem::ClearPool(TSubclassOf<AActor> ActorClass)
{
    // 步骤1：安全验证
    if (!ActorClass) return;

    // 步骤2：查找对应的对象池
    if (TArray<AActor*>* PoolPtr = InactiveActors.Find(ActorClass))
    {
        // 步骤3：遍历销毁所有Actor
        for (AActor* Actor : *PoolPtr)
        {
            if (IsValid(Actor))
            {
                // 安全销毁方式
                Actor->ConditionalBeginDestroy();
            }
        }
        
        // 步骤4：清空数组
        PoolPtr->Empty();
    }

    // 步骤5：移除配置数据
    PoolConfigs.Remove(ActorClass);

    // 步骤6：强制内存回收（可选）
    if (GEngine)
    {
        GEngine->ForceGarbageCollection(true);
    }
}

AActor* UActorPoolSubsystem::CreateNewActor(TSubclassOf<AActor> ActorClass)
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.ObjectFlags |= RF_Transient;

    AActor* NewActor = World->SpawnActor<AActor>(ActorClass, FTransform::Identity, SpawnParams);
    if (NewActor)
    {
        SetupActor(NewActor, false);
        NewActor->SetActorEnableCollision(false);
        NewActor->SetActorHiddenInGame(true);
    }
    return NewActor;
}

void UActorPoolSubsystem::SetupActor(AActor* Actor, bool bActivate)
{
    if (!Actor) return;

    if (bActivate)
    {
        Actor->SetActorHiddenInGame(false);
        Actor->SetActorEnableCollision(true);
        Actor->SetActorTickEnabled(true);
        
        if (IPoolableInterface* Poolable = Cast<IPoolableInterface>(Actor))
        {
            Poolable->OnPoolableActivate();
        }
    }
    else
    {
        Actor->SetActorHiddenInGame(true);
        Actor->SetActorEnableCollision(false);
        Actor->SetActorTickEnabled(false);
        Actor->SetActorTransform(FTransform::Identity);
        
        if (IPoolableInterface* Poolable = Cast<IPoolableInterface>(Actor))
        {
            Poolable->OnPoolableDeactivate();
        }
    }
}

void UActorPoolSubsystem::Deinitialize()
{
    for (auto& PoolEntry : InactiveActors)
    {
        for (AActor* Actor : PoolEntry.Value)
        {
            if (Actor)
            {
                Actor->Destroy();
            }
        }
    }
    InactiveActors.Empty();
    Super::Deinitialize();
}
