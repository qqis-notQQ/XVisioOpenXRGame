// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeGazeCalibrationGameMode.h"

#include "EyeGazeCalibrationActorBase.h"
#include "EyeGazePawn.h"
#include "EyeGazePlayerController.h"
#include "IXRTrackingSystem.h"

#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"


AEyeGazeCalibrationGameMode::AEyeGazeCalibrationGameMode()
{
	DefaultPawnClass = AEyeGazePawn::StaticClass();
	PlayerControllerClass = AEyeGazePlayerController::StaticClass();
	
	//PrimaryActorTick.bCanEverTick = true;    // 开启 Tick
	//PrimaryActorTick.bStartWithTickEnabled = true; // 启动时启用
}

void AEyeGazeCalibrationGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AEyeGazeCalibrationGameMode::StartPlay()
{
	Super::StartPlay();
	
	//设置路径
	jsonPath = FPaths::Combine(FPaths::ProjectConfigDir(),TEXT("pcvr.json"));
	
	UE_LOG(LogTemp, Log, TEXT("AEyeGazeCalibrationGameMode is BeginPlay"));
	if (GEngine && GEngine->XRSystem.IsValid())
	{
		// 获取HMD设备
		IHeadMountedDisplay* HMDDevice = (GEngine->XRSystem == nullptr) ? nullptr : GEngine->XRSystem->GetHMDDevice();
		if (HMDDevice != nullptr)
		{
			HMDDevice->GetFieldOfView(HorizontalFOV, VerticalFOV);
		}
		UE_LOG(LogTemp, Log, TEXT("HorizontalFOV: %f, VerticalFOV %f"), HorizontalFOV, VerticalFOV);
		
		Module = XvisioOpenXR::FXVisioOpenXRModule::Get();
		Gaze = &Module->GetGazeCalibrationPlugin();
	}
	
	SetDefaultConfig();
	
	ActorMonitor = NewObject<UEyeGazeActorSpawnMonitor>(this);
	ActorMonitor->InitActorWithTagDelegate.BindUObject(this,&AEyeGazeCalibrationGameMode::WaitForActorsWithTagInitialized);
	ActorMonitor->InitPawnLocationDelegate.BindUObject(this,&AEyeGazeCalibrationGameMode::WaitForPawnAndFloorInitialized);
	ActorMonitor->InitAllActorDelegate.BindUObject(this,&AEyeGazeCalibrationGameMode::WaitForAllActorsInitialized);
	ActorMonitor->StartMonitoring(GetWorld());
	
}

bool AEyeGazeCalibrationGameMode::InitCalibrationConfig()
{
	bool b_setConfigPath = Gaze->GazeSetConfigPath(jsonPath);
	UE_LOG(LogTemp, Log, TEXT("rGazeSetConfigPath: %d"),b_setConfigPath)
	
	//Gaze->GetGazeCallback(&etEyeData);
	
	int ret_setGazeConfig = -1;
	bool b_setGazeConfig = Gaze->SetGazeConfigs(&configs, &ret_setGazeConfig);
	UE_LOG(LogTemp, Log, TEXT("SetGazeConfigs: %d ,ret: %d "),ret_setGazeConfig,b_setGazeConfig);
	
	int ret_startGaze = -1;
	bool b_startGaze = Gaze->StartGaze(&ret_startGaze);
	UE_LOG(LogTemp, Log, TEXT("StartGaze: %d ,ret: %d "),ret_startGaze,b_startGaze);
	
	if (b_startGaze)
	{
		//Gaze->GetGazeCallback(&EtEyeDataEx);
		
		GetWorld()->GetTimerManager().SetTimer(
			GazeCallbackTimerHandle,
			[this]()
			{
				if (Gaze != nullptr && Module != nullptr)
				{
					Gaze->GetGazeCallback(&etEyeData);
					UE_LOG(LogTemp, Warning, TEXT("GetGazeCallback timestamp: %d , ipd: %f , recommend : %d"),etEyeData.timestamp,etEyeData.ipd,etEyeData.recommend);
				}
			},
			0.2f,
			true
			);
		
	
		
		int ret_setExposure = -1;
		bool b_setExposure = Gaze->SetExposure(10, 5, 10, 5, &ret_setExposure);
		UE_LOG(LogTemp, Log, TEXT("SetExposure: %d ,ret: %d"), b_setExposure, ret_setExposure);
	
		int ret_setBright = -1;
		bool b_setBright = Gaze->SetBright(2, 8, 1, &ret_setBright);
		UE_LOG(LogTemp, Log, TEXT("SetBright: %d ,ret: %d"), b_setBright, ret_setBright);
		
		isGettingGazeCallback = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" StartGaze filled!!!"));
		return false;
	}
	isInit = true;
	//将ipd调节至65mm位置
	int ret_changeIpd = -1;
	Gaze->ChangeIpd(65,&ret_changeIpd);
	return true;
}

void AEyeGazeCalibrationGameMode::InitPawnLocation(APawn* Pawn)
{
	//从openxr中获取的视场角度感觉略大
	HorizontalFOV = HorizontalFOV - 35;
	VerticalFOV = VerticalFOV - 35;
	int width = 192000.0;
	int height = 108000.0;
	
	// 1. FOV（角度 → 弧度）
	float HFOVRad = FMath::DegreesToRadians(HorizontalFOV);
	float VFOVRad = FMath::DegreesToRadians(VerticalFOV);
	
	// 3. 计算距离
	float DistByWidth  = (width  * 0.5f) / FMath::Tan(HFOVRad * 0.5f);
	float DistByHeight = (height * 0.5f) / FMath::Tan(VFOVRad * 0.5f);
	float Distance = FMath::Max(DistByWidth, DistByHeight);

	
	// 2. 设置位置和旋转
	//FVector NewLocation(-95340.0f, 96000.0f, 54000.0f);
	FVector NewLocation(-Distance, 96000.0f, 54000.0f);
	FRotator NewRotation(0.0f, 0.0f, 0.0f); // 根据需要设置具体角度
	
	UE_LOG(LogTemp, Error, TEXT("PawnLocation Distance: %f"),Distance);
	
	if (AEyeGazePawn* eyeGazePawn = Cast<AEyeGazePawn>(Pawn))
	{
		eyeGazePawn->LockedPawnView(NewLocation,NewRotation);
	}
	
	// Pawn->SetActorLocation(NewLocation, false, nullptr);
	// Pawn->SetActorRotation(NewRotation);
	
}

void AEyeGazeCalibrationGameMode::InitCalibration()
{
	if (InitCalibrationConfig())
	{
		EnterState(EEyeGazeCalibrationState::Preparing,-1);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[this]()
			{
				EnterState(EEyeGazeCalibrationState::WaitStart,-1);
			},
			3.0f,
			false
			);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Calibration Config Init filled"));
		EnterState(EEyeGazeCalibrationState::Failed,-1);
	}
}

void AEyeGazeCalibrationGameMode::StartCalibration()
{
	if (isStartCalibration)
	{
		return;
	}
	
	if (CalibrationState != EEyeGazeCalibrationState::WaitStart && CalibrationState != EEyeGazeCalibrationState::Preparing)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calibration not ready"));
		return;
	}

	EnterState(EEyeGazeCalibrationState::Running,-1);
	isStartCalibration = true;
	
	// 下一帧进入采集阶段
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		EnterState(EEyeGazeCalibrationState::Collecting,-1);
		CollectingCalibration();
		//
	});
}

void AEyeGazeCalibrationGameMode::CollectingCalibration()
{
	CurrentPointIndex = 1;
	
	int ret_setUsrEyeready = -1;
	bool b_setUsrEyeready = Gaze->SetUsrEyeReady(&ret_setUsrEyeready);
	UE_LOG(LogTemp, Log, TEXT("SetUsrEyeReady: %d ,ret: %d "),b_setUsrEyeready,ret_setUsrEyeready);
		
	//
		
	int ret_pubBegin0 = -1;
	bool b_pubBegin0 = Gaze->GazeCalibrationPubBegin(0,&ret_pubBegin0);
	UE_LOG(LogTemp, Log, TEXT("GazeCalibrationPubBegin: %d ,ret: %d "),b_pubBegin0,ret_pubBegin0);
		
	int ret_pubBegin1 = -1;
	bool b_pubBegin1 = Gaze->GazeCalibrationPubBegin(1,&ret_pubBegin1);
	UE_LOG(LogTemp, Log, TEXT("GazeCalibrationPubBegin: %d ,ret: %d "),b_pubBegin1,ret_pubBegin1);

	
	CollectNextPoint();
	// GetWorldTimerManager().SetTimer(
	// 	CollectTimer,
	// 	this,
	// 	&AEyeGazeCalibrationGameMode::CollectNextPoint,
	// 	0.1f,   
	// 	true
	// );
}

void AEyeGazeCalibrationGameMode::CollectNextPoint()
{
	if (CurrentPointIndex > 5)
	{
		int ret_pubEnd0 = -1;
		bool b_pubEnd0 = Gaze->GazeCalibrationPubEnd(0,&ret_pubEnd0);
		UE_LOG(LogTemp, Log, TEXT("GazeCalibrationPubBegin: %d ,ret: %d "),b_pubEnd0,ret_pubEnd0);
		
		int ret_pubEnd1 = -1;
		bool b_pubEnd1 = Gaze->GazeCalibrationPubEnd(0,&ret_pubEnd1);
		UE_LOG(LogTemp, Log, TEXT("GazeCalibrationPubBegin: %d ,ret: %d "),b_pubEnd1,ret_pubEnd1);

		GetWorldTimerManager().ClearTimer(CollectTimer);
		FinishCalibration(true);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Calibration on Collecting , Num is: %d "),CurrentPointIndex);
	EnterState(EEyeGazeCalibrationState::Collecting, CurrentPointIndex);
}

void AEyeGazeCalibrationGameMode::OnPointCollected(int PointIndex,int ret)
{
	CurrentPointIndex++;
	CollectNextPoint();
}

void AEyeGazeCalibrationGameMode::FinishCalibration(bool bSuccess)
{
	float ipd = 0;
	bool b_getGazeIpd = Gaze->GetGazeIpd(&ipd);
	UE_LOG(LogTemp, Log, TEXT("ipd: %f ,ret: %d "),ipd,b_getGazeIpd);
	
	if (ipd > 56 && ipd < 72)
	{
		//resultText.text = $"校准成功\n瞳距: {ipd}mm";
		//RefreshIpd(ipd);
		int ret = -1;
		Gaze->ChangeIpd(ipd,&ret);
	}
	else
	{
		float ipd_random = FMath::RoundToFloat(FMath::RandRange(64.0f, 68.0f) * 100.0f) / 100.0f;
		//esultText.text = $"校准成功\n瞳距: {ipd_random}mm";
		int ret = -1;
		Gaze->ChangeIpd(ipd_random,&ret);
	}
	
	GetWorld()->GetTimerManager().ClearTimer(GazeCallbackTimerHandle);
	
	SaveCalibrationData();
	EnterState(bSuccess? EEyeGazeCalibrationState::WriteFile: EEyeGazeCalibrationState::Failed,-1);
	
	int ret_stopGaze = -1;
	bool b_stopGaze = Gaze->StopGaze(&ret_stopGaze);
	UE_LOG(LogTemp, Log, TEXT("StartGaze: %d ,ret: %d "),ret_stopGaze,b_stopGaze);
	
	EnterState(bSuccess? EEyeGazeCalibrationState::Finished: EEyeGazeCalibrationState::Failed,-1);
	
}

void AEyeGazeCalibrationGameMode::WaitForPawnAndFloorInitialized()
{
	UE_LOG(LogTemp, Warning, TEXT("-------------- WaitForPawnAndFloorInitialized is ok!"));
	
	// 1. 安全获取Controller和Pawn
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found!"));
		return;
	}
	
	APawn* Pawn = PlayerController->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Pawn not found!"));
		return;
	}
	
	InitPawnLocation(Pawn);
	
	
}

void AEyeGazeCalibrationGameMode::WaitForActorsWithTagInitialized()
{
	UE_LOG(LogTemp, Warning, TEXT("*********************** WaitForActorsWithTagInitialized is ok!!"));
	InitCalibration();
	
}

void AEyeGazeCalibrationGameMode::WaitForAllActorsInitialized()
{
	UE_LOG(LogTemp, Warning, TEXT("*********************** WaitForAllActorsInitialized is ok!!"));
	isRead = true;
}

void AEyeGazeCalibrationGameMode::SetDefaultConfig()
{
	//配置使用默认值
	configs.width = 3840;
	configs.height = 3552;
	configs.ipdDist = -1;
	configs.srValue = 0;
	configs.etWidth = 400;
	configs.etHeight = 400;
	configs.loplength = 8;
	configs.etFoclen = 276.0f;
	configs.etOccupy = 0.3f;
	configs.ftFoclen = 2399.0f;
}

void AEyeGazeCalibrationGameMode::EnterState(EEyeGazeCalibrationState NewState, int id)
{
	
	if (NewState != EEyeGazeCalibrationState::Collecting &&
	CalibrationState == NewState)
	{
		return;
	}

	CalibrationState = NewState;

	// ⭐ 广播（蓝图 & C++ 都能收到）
	OnCalibrationStateChanged.Broadcast(NewState,id);
	
	UE_LOG(LogTemp, Warning, TEXT("Broadcast,NewState is: %d ,id is: %d "),NewState,id);
	switch (CalibrationState)
	{
	case EEyeGazeCalibrationState::Preparing:
		
		UE_LOG(LogTemp, Log, TEXT("EyeGaze Calibration: Preparing"));
		break;

	case EEyeGazeCalibrationState::Running:
		UE_LOG(LogTemp, Log, TEXT("EyeGaze Calibration: Running"));
		break;

	case EEyeGazeCalibrationState::Collecting:
		UE_LOG(LogTemp, Log, TEXT("EyeGaze Calibration: Collecting data"));
		break;

	case EEyeGazeCalibrationState::Finished:
		UE_LOG(LogTemp, Log, TEXT("EyeGaze Calibration: Finished"));
		break;

	case EEyeGazeCalibrationState::Failed:
		UE_LOG(LogTemp, Error, TEXT("EyeGaze Calibration: Failed"));
		break;

	default:
		break;
	}
}

void AEyeGazeCalibrationGameMode::SaveCalibrationData() const
{
	int Size = 0;
	TArray<uint8> CalData;
	CalData.Init(0, 4096); // 初始化数组大小为 4096

	int RetGetPref = -1;
	
	bool bGetPref = Gaze->GetPref(-1, CalData.GetData(), &Size, &RetGetPref);

	UE_LOG(LogTemp, Log, TEXT("XrFeatureGetPref: %s, ret: %d"), bGetPref ? TEXT("True") : TEXT("False"), RetGetPref);

	// 校准数据文件路径
	FString HomeDir = FPlatformProcess::UserHomeDir();
	// 结果: /home/ydp/
	FString CalibrationDir = FPaths::Combine(HomeDir,TEXT(".config/gaze"));
	
	FString CalibrationFilePath =FPaths::Combine(CalibrationDir, TEXT("CaliData.dat"));

	IPlatformFile& PlatformFile =FPlatformFileManager::Get().GetPlatformFile();

	// ✅ 关键：创建目录（递归）
	if (!PlatformFile.DirectoryExists(*CalibrationDir))
	{
		if (!PlatformFile.CreateDirectoryTree(*CalibrationDir))
		{
			UE_LOG(LogTemp, Error, TEXT("创建校准目录失败: %s"), *CalibrationDir);
			return;
		}
	}

	// 可选：删旧文件
	if (PlatformFile.FileExists(*CalibrationFilePath))
	{
		PlatformFile.DeleteFile(*CalibrationFilePath);
	}

	// 写入数据到文件
	if (FFileHelper::SaveArrayToFile(CalData, *CalibrationFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("校准数据已成功保存到文件: %s"), *CalibrationFilePath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("保存校准数据时失败: %s"), *CalibrationFilePath);
	}
}

// 写入数据到文件
void AEyeGazeCalibrationGameMode::WriteDataToFile(const FString& Path, const TArray<uint8>& Data) const
{
	// 使用 FFileHelper 写入文件
	if (FFileHelper::SaveArrayToFile(Data, *Path))
	{
		UE_LOG(LogTemp, Log, TEXT("数据已成功写入文件: %s"), *Path);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("写入文件时失败: %s"), *Path);
	}
}

void AEyeGazeCalibrationGameMode::ReadDataFromFile(const FString& Path, TArray<uint8>& OutData) const
{
	// 使用 FFileHelper 读取文件
	if (FFileHelper::LoadFileToArray(OutData, *Path))
	{
		UE_LOG(LogTemp, Log, TEXT("数据已成功从文件读取, 数据大小: %d"), OutData.Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("读取文件时失败: %s"), *Path);
	}
}


