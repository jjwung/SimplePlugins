// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ActorPoolGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ACTORPOOL_API UActorPoolGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	TSet<AActor*> ActorPool;

protected:
	UPROPERTY(BlueprintReadOnly)
	int MaxPoolSize = 20;

public:
	UFUNCTION(BlueprintCallable)
	AActor* SpawnActor(UClass* Class, const FVector Location, const FRotator Rotation);

	UFUNCTION(BlueprintCallable)
	void DestroyActor(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void SetMaxPoolSize(int Size)
	{
		if (Size > 0)
		{
			MaxPoolSize = Size;
		}
		else
		{
			MaxPoolSize = 20;
		}
	};

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetPoolSize()
	{
		return ActorPool.Num();
	};

	UFUNCTION(Blueprintable)
	void Add(AActor* InActor);

	UFUNCTION(Blueprintable)
	void Remove(AActor* InActor);
};
