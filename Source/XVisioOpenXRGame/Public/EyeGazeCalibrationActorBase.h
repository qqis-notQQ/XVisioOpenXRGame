// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EyeGazeCalibrationGameMode.h"
#include "Standalone/XvisioOpenXR.h"
#include "ExtensionsHead/XvisioOpenXRExtensions.h"
#include "GameFramework/Actor.h"
#include "EyeGazeCalibrationActorBase.generated.h"

UCLASS()
class XVISIOOPENXRGAME_API AEyeGazeCalibrationActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEyeGazeCalibrationActorBase();
	UFUNCTION(BlueprintCallable, Category = "EyeGaze Calibration")
	void BroadcastOnPointCollected();
	
	void Calibration();
	
	void StopCalibration();
	
	void BroadcastAfterDelay();

	UFUNCTION(BlueprintNativeEvent, Category="EyeGaze Calibration")
	void OnCalibrationStateChanged(EEyeGazeCalibrationState NewState,int id);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(BlueprintAssignable, Category = "EyeGaze Calibration")
	FOnPointCollected  OnPointCollected;
	
	UPROPERTY(BlueprintReadWrite, Category = "EyeGaze Calibration")
	int pointIndex; 
	
	FTimerHandle TimerHandle ;
private:
	
	XvisioOpenXR::FXVisioOpenXRModule *Module;
	XvisioOpenXR::FGazeCalibrationPlugin *Gaze;
	
	FTimerHandle RepeatTimerHandle;
	FTimerHandle StopTimerHandle;
	
	uint32_t ret;
	bool isCenter = false;
	
	bool GetGazeCallbackRet = false;

};
