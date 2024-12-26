// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPoolGameInstanceSubsystem.h"

AActor* UActorPoolGameInstanceSubsystem::SpawnActor(UClass* Class,const FVector Location, const FRotator Rotation)
{
	AActor* Actor = nullptr;
	if (ActorPool.Num() > 0)
	{
		for (AActor* A : ActorPool)
		{
			A->SetActorLocation(Location);
			A->SetActorRotation(Rotation);
			Actor = A;
			Remove(A);
			break;
		}
	}
	else
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			Actor = World->SpawnActor<AActor>(Class, Location, Rotation, SpawnParams);
		}
	}
	if (Actor)
	{
		Actor->SetActorLocation(Location);
		Actor->SetActorRotation(Rotation);
		Actor->SetActorHiddenInGame(false);
	}
	return Actor;
}

void UActorPoolGameInstanceSubsystem::DestroyActor(AActor* InActor)
{
	if (ActorPool.Num() >= MaxPoolSize)
	{
		InActor->Destroy();
	}
	else
	{
		Add(InActor);
	}
}

void UActorPoolGameInstanceSubsystem::Add(AActor* InActor)
{
	//InActor->OnEndPlay;
	ActorPool.Add(InActor);
	InActor->SetActorHiddenInGame(true);
	InActor->SetActorEnableCollision(false);
}

void UActorPoolGameInstanceSubsystem::Remove(AActor* InActor)
{
	ActorPool.Remove(InActor);
	InActor->SetActorEnableCollision(true);
	InActor->DispatchBeginPlay();
}