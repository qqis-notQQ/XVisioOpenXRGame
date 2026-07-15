// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeGazePlayerCameraManager.h"

#include "EyeGazePawn.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"

//
// void AEyeGazePlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
// {
// 	// if (bFollowHmdOrientation)
// 	// {
// 	// 	//如果有VR设备就直接运行引擎原始函数
// 	// 	Super::UpdateViewTarget(OutVT, DeltaTime);
// 	// 	return;
// 	// }
// 	// Don't update outgoing viewtarget during an interpolation 
// 	if ((PendingViewTarget.Target != NULL) && BlendParams.bLockOutgoing && OutVT.Equal(ViewTarget))
// 	{
// 		return;
// 	}
// 	bool bDoNotApplyModifiers = false;
//
// 	if (ACameraActor* CamActor = Cast<ACameraActor>(OutVT.Target))
// 	{
// 		// Viewing through a camera actor.
// 		CamActor->GetCameraComponent()->GetCameraView(DeltaTime, OutVT.POV);
// 	}
// 	else
// 	{
// 		if (CameraStyle == FName("SceneFixed"))
// 		{
// 			//这里我感觉可能用PendingViewTarget来传递摄像机坐标和旋转比较好，但是还没测试过
// 			//自己定义一个场景固定视角
// 			AEyeGazePawn *Character = Cast<AEyeGazePawn>(GetOwningPlayerController()->GetPawn());
// 			OutVT.POV.Location = Character->GetPawnViewLocation();
// 			OutVT.POV.Rotation = Character->GetViewRotation();
// 			//DesiredView.FOV = FieldOfView;
// 			//DesiredView.AspectRatio = AspectRatio;
// 			// don't apply modifiers when using this debug camera mode
// 			bDoNotApplyModifiers = true;
// 		}else if (CameraStyle==FName("Default"))
// 		{
// 			//默认方式是直接取得摄像机的参数来设置FTViewTarget.pov,而摄像机被控制类、SpringArm控制。
// 			UpdateViewTargetInternal(OutVT, DeltaTime);
// 		}
// 		else
// 		{
// 			Super::UpdateViewTarget(OutVT, DeltaTime);
// 		}
// 	}
// 	if (!bDoNotApplyModifiers || bAlwaysApplyModifiers)
// 	{
// 		// Apply camera modifiers at the end (view shakes for example)
// 		ApplyCameraModifiers(DeltaTime, OutVT.POV);
// 	}
// 	// Synchronize the actor with the view target results
// 	SetActorLocationAndRotation(OutVT.POV.Location, OutVT.POV.Rotation, false);
// 	UpdateCameraLensEffects(OutVT);
// }
