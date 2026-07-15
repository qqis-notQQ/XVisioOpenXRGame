// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Standalone/GazeCalibrationPlugin.h"
#include "IEventListenerInterface.h"
#include "GameFramework/Actor.h"
#include "LoadOpenXRExtension.generated.h"



UCLASS()
class XVISIOOPENXRGAME_API ALoadOpenXRExtension : public AActor, public IEventListenerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoadOpenXRExtension();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnJumpPressed_Implementation() override;
	
	void EnableCustomOpenXRExtension();
	
	//切换关卡
	void sssss();
	
private:

	void ETinit(XvisioOpenXR::FGazeCalibrationPlugin Gaze);
	
	void Calibrate(XvisioOpenXR::FGazeCalibrationPlugin Gaze);
	
    

};
