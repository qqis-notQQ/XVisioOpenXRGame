// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Standalone/XvisioOpenXR.h"
#include "EyeGazeActorSpawnMonitor.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/GameModeBase.h"
#include "EyeGazeCalibrationGameMode.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEyeGazeCalibrationState : uint8
{
	None        UMETA(DisplayName="None"),
	Preparing  UMETA(DisplayName="Preparing"),
	WaitStart  UMETA(DisplayName="WaitStart"),
	Running    UMETA(DisplayName="Running"),
	Collecting UMETA(DisplayName="Collecting"),
	Finished   UMETA(DisplayName="Finished"),
	WriteFile   UMETA(DisplayName="WriteFile"),
	Failed     UMETA(DisplayName="Failed")
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEyeGazeCalibrationStateChanged,EEyeGazeCalibrationState, NewState, int ,StateID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPointCollected,int,PointIndex,int,ret);


UCLASS()
class XVISIOOPENXRGAME_API AEyeGazeCalibrationGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AEyeGazeCalibrationGameMode();

protected:
	virtual void BeginPlay() override;

public:
	virtual void StartPlay() override;
	
	XrEtEyeDataEx etEyeData{};
	FTimerHandle GazeCallbackTimerHandle;
	
	//关卡加载过程控制
	void WaitForPawnAndFloorInitialized();
	void WaitForActorsWithTagInitialized();
	void WaitForAllActorsInitialized();
	
	//校准流程控制
	
	/** 当前校准状态 */
	UPROPERTY(BlueprintReadOnly, Category="EyeGaze|Calibration")
	EEyeGazeCalibrationState CalibrationState = EEyeGazeCalibrationState::None;
	
	UPROPERTY(BlueprintAssignable, Category = "EyeGaze Calibration")
	FEyeGazeCalibrationStateChanged OnCalibrationStateChanged;
	
	
	/** 启动初始化
	 * 加载配置文件
	 * 画布、pawm初始化完成设置pawn位置
	 * 
	 * actor 全部加载完成 输出倒计时 启动校准
	 * 
	 */
	
	/** 加载配置文件
	 */
	bool InitCalibrationConfig();
	
	/** 初始化Pawn位置和旋转
	 * 获取当前头显的FOV，根据fov计算当前的位置点位
	 */
	void InitPawnLocation(APawn* Pawn);
	
	/** 启动初始化 */
	//UFUNCTION(BlueprintCallable, Category="EyeGaze|Calibration")
	void InitCalibration();
	
	/** 启动校准 */
	//UFUNCTION(BlueprintCallable, Category="EyeGaze|Calibration")
	void StartCalibration();
	//启动眼动校准流程
	void CollectingCalibration();
	//下一个点的校准
	void CollectNextPoint();
	//校准完成的回调
	UFUNCTION()
	void OnPointCollected(int PointIndex,int ret);

	/** 结束校准 */
	//UFUNCTION(BlueprintCallable, Category="EyeGaze|Calibration")
	void FinishCalibration(bool bSuccess);

	
	//设置config的初始值
	void SetDefaultConfig();
	
	
	//保存校准文件
	void SaveCalibrationData() const;
	void WriteDataToFile(const FString& Path, const TArray<uint8>& Data) const;
	void ReadDataFromFile(const FString& Path, TArray<uint8>& OutData) const;


private:
	
	//当前校准点位的序号
	int32 CurrentPointIndex = 1;
	FTimerHandle CollectTimer;
	
	xrGazeConfigsData configs = xrGazeConfigsData();
	
	bool isRead = false;
	bool isStartCalibration = false;
	
	//当前设备的视场角
	float HorizontalFOV;
	float VerticalFOV;
	
	XvisioOpenXR::FXVisioOpenXRModule *Module;
	XvisioOpenXR::FGazeCalibrationPlugin *Gaze;
	
	bool isInit = false;
	FString calibrationFilePath;
	FString config_path;
	bool isGettingGazeCallback = false;
	FString jsonPath;
	
	void EnterState(EEyeGazeCalibrationState NewState,int id);
	
	UEyeGazeActorSpawnMonitor *ActorMonitor;
};
