// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeGazePlayerController.h"

#include "EyeGazePlayerCameraManager.h"
#include "IXRTrackingSystem.h"

AEyeGazePlayerController::AEyeGazePlayerController()
{
	PlayerCameraManagerClass = AEyeGazePlayerCameraManager::StaticClass();
}

void AEyeGazePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 禁止玩家控制旋转
	bShowMouseCursor = false;
	SetIgnoreLookInput(true);
	SetIgnoreMoveInput(true);
	
	if (IHeadMountedDisplay* HMD = GEngine->XRSystem->GetHMDDevice())
	{
		// 禁用头部追踪对相机旋转的影响
		//HMD->SetTrackingOrigin(EHMDTrackingOrigin::Floor);
            
		// 或者尝试设置忽略头部旋转
		// 注意：具体方法可能因 HMD 类型而异
	}
	
	// 强制相机不跟随 HMD
	if (PlayerCameraManager)
	{
		//PlayerCameraManager->bFollowHmdOrientation = false; // UE5 新接口类似
	}
}
