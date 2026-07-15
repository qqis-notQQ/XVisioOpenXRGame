// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "EyeGazeActorSpawnMonitor.generated.h"

DECLARE_DELEGATE(FInitPawnLocationDelegate);
DECLARE_DELEGATE(FInitActorWithTagDelegate);
DECLARE_DELEGATE(FInitAllActorDelegate);

/**
 * 
 */
UCLASS()
class XVISIOOPENXRGAME_API UEyeGazeActorSpawnMonitor : public UObject
{
	GENERATED_BODY()
	
public:
	UEyeGazeActorSpawnMonitor() ;

	~UEyeGazeActorSpawnMonitor();

	TArray<FString> PawnTagValues;
	TArray<FString> ActorTagValues;
	
	FInitPawnLocationDelegate InitPawnLocationDelegate;
	FInitActorWithTagDelegate InitActorWithTagDelegate;
	FInitAllActorDelegate InitAllActorDelegate;
	
	void StartMonitoring(UWorld* World);
	//void StopMonitoring();

private:
	UPROPERTY()
	UWorld* TargetWorld;
	
	bool PawnIsRead = false;
	bool PActorTagIsRead = false;
	bool AllActorIsRead = false;
	
	int8 ActorNum = 0;
	
	TMap<TSubclassOf<AActor>, TArray<TWeakObjectPtr<AActor>>> MonitoredActors;
	//TMap<TSubclassOf<AActor>, int32> ExpectedCounts;
	
	FDelegateHandle ActorSpawnedHandle;
	FDelegateHandle ActorDestroyedHandle;
	
	
    
	void OnActorSpawned(AActor* Actor);
	//void OnActorDestroyed(AActor* Actor);
	//void CheckIfAllActorsReady();
};