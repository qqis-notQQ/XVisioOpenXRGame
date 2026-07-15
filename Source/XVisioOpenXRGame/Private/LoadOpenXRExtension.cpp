// Fill out your copyright notice in the Description page of Project Settings.

#include "LoadOpenXRExtension.h"

#include "Standalone/XvisioOpenXR.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALoadOpenXRExtension::ALoadOpenXRExtension()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void ALoadOpenXRExtension::BeginPlay()
{
	Super::BeginPlay();

}

void ALoadOpenXRExtension::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALoadOpenXRExtension::Tick(float DeltaTime)
{
	
	//EnableCustomOpenXRExtension();
	Super::Tick(DeltaTime);
}

void ALoadOpenXRExtension::OnJumpPressed_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnJumpPressed_Implementation"));
	//EnableCustomOpenXRExtension();
	sssss();
}

void ALoadOpenXRExtension::EnableCustomOpenXRExtension()
{
	//
	if (!GEngine || !GEngine->XRSystem.IsValid())
		return;

	if (!XvisioOpenXR::FXVisioOpenXRModule::IsAvailable())
	{
		UE_LOG(LogTemp, Warning, TEXT("XvisioOpenXR module not available"));
		return;
	}

	auto Module = XvisioOpenXR::FXVisioOpenXRModule::Get();
	auto Gaze = Module->GetGazeCalibrationPlugin();

	uint32 Type = 0, Width = 0, Height = 0;
	bool bOK = Gaze.GetDeviceCameraImageSize(Type, &Width, &Height);
	
	
	
	//Gaze.GazeSetConfigPath()
	
	UE_LOG(LogTemp, Log, TEXT("GazeCamera: %d %u x %u"), bOK, Width, Height);
}

void ALoadOpenXRExtension::sssss()
{
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		[this]()
		{
			UGameplayStatics::OpenLevel(this, TEXT("EyeGazeCalibration"));
		},
		0.1f,
		false
	);
	//UGameplayStatics::OpenLevel(GetWorld(), "EyeGazeCalibration");
}

void ALoadOpenXRExtension::ETinit(XvisioOpenXR::FGazeCalibrationPlugin Gaze)
{
	// bool b_setConfigPath = Gaze.GazeSetConfigPath(jsonPath);
	// UE_LOG(LogTemp, Log, TEXT("rGazeSetConfigPath: %d"),b_setConfigPath);
	//
	// int ret_setGazeConfig = -1;
	// bool b_setGazeConfig = Gaze.SetGazeConfigs(configs, &ret_setGazeConfig);
	// UE_LOG(LogTemp, Log, TEXT("SetGazeConfigs: %d ,ret: %d "),ret_setGazeConfig,b_setGazeConfig);
	//
	// int ret_startGaze = -1;
	// bool b_startGaze = Gaze.StartGaze(&ret_startGaze);
	// UE_LOG(LogTemp, Log, TEXT("StartGaze: %d ,ret: %d "),ret_startGaze,b_startGaze);
	//
	//
	// //设置曝光增益前等待1秒,确保设置成功
	//
	// //Debug.Log($"before pub_set_usr_eye_ready");
	// //pub_set_usr_eye_ready();
	// //Debug.Log($"pub_set_usr_eye_ready");
	//
	// if (b_startGaze)
	// {
	// 	int ret_setExposure = -1;
	// 	bool b_setExposure = Gaze.SetExposure(10, 5, 10, 5, &ret_setExposure);
	// 	UE_LOG(LogTemp, Log, TEXT("SetExposure: %d ,ret: %d"), b_setExposure, ret_setExposure);
	//
	// 	int ret_setBright = -1;
	// 	bool b_setBright = Gaze.SetBright(2, 8, 1, &ret_setBright);
	// 	UE_LOG(LogTemp, Log, TEXT("SetBright: %d ,ret: %d"), b_setBright, ret_setBright);
	// 	
	//
	// 	isGettingGazeCallback = true;
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT(" StartGaze filled!!!"));
	// }
	//
	//
	// isInit = true;
	//
	// //将ipd调节至65mm位置
	// int ret_changeIpd = -1;
	// Gaze.ChangeIpd(65,&ret_changeIpd);
}

void ALoadOpenXRExtension::Calibrate(XvisioOpenXR::FGazeCalibrationPlugin Gaze)
{
	       //
        // //校准前ready
        // UE_LOG(LogTemp, Log, TEXT("before XrFeatureSetUsrEyeReady"));
        // int ret_setUsrEyeready = -1;
        // bool b_setUsrEyeready = Gaze.SetUsrEyeReady(&ret_setUsrEyeready);
        // UE_LOG(LogTemp, Log, TEXT("XrFeatureSetUsrEyeReady:{b_setUsrEyeready},ret:{ret_setUsrEyeready}"));
        //
        // yield return new WaitForSeconds(0.2f);
        //
        // int ret_pubBegin0 = -1;
        // bool b_pubBegin0 = Gaze.GazeCalibrationPubBegin(0, &ret_pubBegin0);
        // UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubBegin 0:{b_pubBegin0},ret:{ret_pubBegin0}"));
        //
        // int ret_pubBegin1 = -1;
        // bool b_pubBegin1 = Gaze.GazeCalibrationPubBegin(1,  &ret_pubBegin1);
        // UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubBegin 1:{b_pubBegin1},ret:{ret_pubBegin1}"));
        //
        //
        //
        // yield return new WaitForSeconds(1);
        //
        // //隐藏所有校准点
        // for (int i = 0; i < calipoint.transform.childCount; i++)
        // {
        //     calipoint.transform.GetChild(i).gameObject.SetActive(false);
        // }
        //
        // while (currentCalipoint<4)
        // {
        //     
        //     //显示当前校准点
        //     for (int i = 0; i < calipoint.transform.childCount; i++)
        //     {
        //         if (i == currentCalipoint)
        //         {
        //             calipoint.transform.GetChild(i).gameObject.SetActive(true);
        //         }
        //         else
        //         {
        //             calipoint.transform.GetChild(i).gameObject.SetActive(false);
        //         }
        //     }
        //
        //     
        //
        //     //眼睛保持注视状态下才开始校准，即fix和ready for cali都为1时才进行校准
        //     //if (eyeData.leftPupil.pupilBitMask == 1 && eyeData.rightPupil.pupilBitMask == 1 && eyeData.leftGaze.gazeBitMask == 1 && eyeData.rightGaze.gazeBitMask == 1)
        //     {
        //         do
        //         {
        //             yield return new WaitForSeconds(0.5f);
        //             conf_gaze_o[0] = calipoint.GetChild(currentCalipoint).GetComponent<RectTransform>().anchoredPosition.x;
        //             conf_gaze_o[1] = -calipoint.GetChild(currentCalipoint).GetComponent<RectTransform>().anchoredPosition.y;
        //
        //             #region 超时判定
        //             //超时判定
        //             cali_timeout += Time.deltaTime * 90;
        //             switch (currentCalipoint)
        //             {
        //                 case 0:
        //                     if (cali_timeout > 5)
        //                     {
        //                         leftCalipointOK = true;
        //                         cali_timeout = 0;
        //                     }
        //                     break;
        //                 case 1:
        //                     if (cali_timeout > 5)
        //                     {
        //                         rightCalipointOK = true;
        //                         cali_timeout = 0;
        //                     }
        //                     break;
        //                 case 2:
        //                     if (cali_timeout > 5)
        //                     {
        //                         rightCalipointOK = true;
        //                         cali_timeout = 0;
        //                     }
        //                     break;
        //                 case 3:
        //                     if (cali_timeout > 5)
        //                     {
        //                         leftCalipointOK = true;
        //                         cali_timeout = 0;
        //                     }
        //                     break;
        //
        //             }
        //             //Debug.LogError($"cali_timeout:{cali_timeout}");
        //
        //             #endregion
        //
        //             if (!leftCalipointOK)
        //             {
        //                 if (currentCalipoint==0)
        //                 {
        //                     int ret_setConfGaze = -1;
        //                     bool b = Gaze.GazeCalibrationPubSetConfGaze(0, 0, conf_gaze_o[0], conf_gaze_o[1],ref ret_setConfGaze);
        //                     UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubSetConfGaze 0,currentCalipoint:{currentCalipoint},conf_gaze_o[0]:{conf_gaze_o[0]},conf_gaze_o[1]:{conf_gaze_o[1]},b:{b},ret:{ret_setConfGaze}"));
        //
        //                 }
        //                 if (currentCalipoint == 3)
        //                 {
        //                     int ret_setConfGaze = -1;
        //                     bool b = Gaze.GazeCalibrationPubSetConfGaze(0, 2, conf_gaze_o[0], conf_gaze_o[1], ref ret_setConfGaze);
        //                     UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubSetConfGaze 0,currentCalipoint:{currentCalipoint},conf_gaze_o[0]:{conf_gaze_o[0]},conf_gaze_o[1]:{conf_gaze_o[1]},b:{b},ret:{ret_setConfGaze}"));
        //
        //                 }
        //
        //
        //             }
        //
        //             if (!rightCalipointOK)
        //             {
        //                 if (currentCalipoint == 1)
        //                 {
        //                     int ret_setConfGaze = -1;
        //                     bool b = Gaze.GazeCalibrationPubSetConfGaze(1, 1, conf_gaze_o[0], conf_gaze_o[1], ref ret_setConfGaze);
        //                     UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubSetConfGaze 1,currentCalipoint:{currentCalipoint},conf_gaze_o[0]:{conf_gaze_o[0]},conf_gaze_o[1]:{conf_gaze_o[1]},b:{b},ret:{ret_setConfGaze}"));
        //
        //                 }
        //                 if (currentCalipoint == 2)
        //                 {
        //                     int ret_setConfGaze = -1;
        //                     bool b = Gaze.GazeCalibrationPubSetConfGaze(1, 2, conf_gaze_o[0], conf_gaze_o[1], ref ret_setConfGaze);
        //                     UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubSetConfGaze 1,currentCalipoint:{currentCalipoint},conf_gaze_o[0]:{conf_gaze_o[0]},conf_gaze_o[1]:{conf_gaze_o[1]},b:{b},ret:{ret_setConfGaze}"));
        //
        //                 }
        //
        //             }
        //
        //
        //         }
        //         //exDataBitMask底层数据是int类型，所以使用时需要强制转换
        //         //while ((int)eyeData.leftGaze.exDataBitMask != currentCalipoint && (int)eyeData.rightGaze.exDataBitMask != currentCalipoint);
        //         while (!leftCalipointOK && !rightCalipointOK);
        //
        //         UE_LOG(LogTemp, Log, TEXT("leftCalipointOK:{leftCalipointOK},rightCalipointOK:{rightCalipointOK}"));
        //         //当前校准点校准成功后校准下一个点
        //         currentCalipoint++;
        //
        //         leftCalipointOK = false;
        //         rightCalipointOK = false;
        //
        //         //播放音效
        //         GetComponent<AudioSource>().Play();
        //     }
        //     yield return new WaitForSeconds(1.5f);
        // }
        //
        //
        // while (currentCalipoint == 4)
        // {
        //
        //     //显示当前校准点
        //     for (int i = 0; i < calipoint.transform.childCount; i++)
        //     {
        //         if (i == currentCalipoint)
        //         {
        //             calipoint.transform.GetChild(i).gameObject.SetActive(true);
        //         }
        //         else
        //         {
        //             calipoint.transform.GetChild(i).gameObject.SetActive(false);
        //         }
        //     }
        //
        //     
        //     //眼睛保持注视状态下才开始校准
        //     //if (eyeData.leftPupil.pupilBitMask == 1 && eyeData.rightPupil.pupilBitMask == 1 && eyeData.leftGaze.gazeBitMask == 1 && eyeData.rightGaze.gazeBitMask == 1)
        //     {
        //         do
        //         {
        //             yield return new WaitForSeconds(0.5f);
        //             conf_gaze_o[0] = calipoint.GetChild(currentCalipoint).GetComponent<RectTransform>().anchoredPosition.x;
        //             conf_gaze_o[1] = -calipoint.GetChild(currentCalipoint).GetComponent<RectTransform>().anchoredPosition.y;
        //
        //
        //             #region 超时判定
        //             //超时判定
        //             cali_timeout += Time.deltaTime * 90;
        //             if (cali_timeout > 5)
        //             {
        //                 leftCalipointOK = true;
        //                 rightCalipointOK = true;
        //                 cali_timeout = 0;
        //             }
        //             //Debug.LogError($"cali_timeout:{cali_timeout}");
        //             #endregion
        //
        //
        //             if (!leftCalipointOK)
        //             {
        //                 
        //                 if (currentCalipoint == 4)
        //                 {
        //                     int ret_setConfGaze = -1;
        //                     bool b = Gaze.GazeCalibrationPubSetConfGaze(0, 1, conf_gaze_o[0], conf_gaze_o[1], ref ret_setConfGaze);
        //                     UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubSetConfGaze 0,currentCalipoint:{currentCalipoint},conf_gaze_o[0]:{conf_gaze_o[0]},conf_gaze_o[1]:{conf_gaze_o[1]},b:{b},ret:{ret_setConfGaze}"));
        //
        //                 }
        //
        //             }
        //
        //             if (!rightCalipointOK)
        //             {
        //                 
        //                 if (currentCalipoint == 4)
        //                 {
        //                     int ret_setConfGaze = -1;
        //                     bool b = Gaze.GazeCalibrationPubSetConfGaze(1, 0, conf_gaze_o[0], conf_gaze_o[1], ref ret_setConfGaze);
        //                     UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubSetConfGaze 1,currentCalipoint:{currentCalipoint},conf_gaze_o[0]:{conf_gaze_o[0]},conf_gaze_o[1]:{conf_gaze_o[1]},b:{b},ret:{ret_setConfGaze}"));
        //
        //                 }
        //             }
        //
        //
        //         }
        //         //exDataBitMask底层数据是int类型，所以使用时需要强制转换
        //         //while ((int)eyeData.leftGaze.exDataBitMask != currentCalipoint && (int)eyeData.rightGaze.exDataBitMask != currentCalipoint);
        //         while (!leftCalipointOK || !rightCalipointOK);
        //
        //         UE_LOG(LogTemp, Log, TEXT("leftCalipointOK:{leftCalipointOK},rightCalipointOK:{rightCalipointOK}");
        //         //当前校准点校准成功后校准下一个点
        //         currentCalipoint++;
        //
        //         leftCalipointOK = false;
        //         rightCalipointOK = false;
        //
        //         //播放音效
        //         GetComponent<AudioSource>().Play();
        //
        //     }
        //     yield return new WaitForSeconds(1.5f);
        // }
        //
        // //yield return new WaitForSeconds(1);
        //
        //
        // int ret_pubEnd0 = -1;
        // bool b_pubEnd0 = Gaze.GazeCalibrationPubEnd(0,  &ret_pubEnd0);
        // UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubEnd 0:{b_pubEnd0},ret:{ret_pubEnd0}"));
        //
        // int ret_pubEnd1 = -1;
        // bool b_pubEnd1 = Gaze.GazeCalibrationPubEnd(1, ref ret_pubEnd1);
        // UE_LOG(LogTemp, Log, TEXT("XrFeatureGazeCalibrationPubEnd 1:{b_pubEnd1},ret:{ret_pubEnd1}"));
        //
        // resultText.gameObject.SetActive(true);
        // //if (ret_pubEnd0 == 1 && ret_pubEnd1 == 1)
        // //{
        // //    //resultText.text = $"校准成功\nend0：{end0}，end1：{end1}";
        // //    resultText.text = $"校准成功";
        //
        // //    //保存校准文件
        // //    SaveCalibrationData();
        // //}
        // //else
        // //{
        // //    //resultText.text = $"校准失败\nend0：{end0}，end1：{end1}";
        // //    resultText.text = $"校准失败";
        // //}
        //
        // //获取校准后得到的ipd数据
        // float ipd = 0;
        // bool b_getIpd = Gaze.GetGazeIpd(ref ipd);
        // UE_LOG(LogTemp, Log, TEXT("XrFeatureGetGazeIpd:{b_getIpd},ipd:{ipd}");
        // ipdText.text = $"ipd:{ipd}";
        // //取小数点后两位
        // ipd = (float)Math.Round(ipd, 2);
        // //判断ipd数据是否在56~72范围内，若不在则采取随机数
        // if (ipd > 56 && ipd < 72)
        // {
        //     resultText.text = $"校准成功\n瞳距: {ipd}mm";
        //     RefreshIpd(ipd);
        // }
        // else
        // {
        //     float ipd_random = (float)Math.Round(UnityEngine.Random.Range(64.0f, 68.0f), 2);
        //     resultText.text = $"校准成功\n瞳距: {ipd_random}mm";
        //     RefreshIpd(ipd_random);
        // }
        //
        //
        // for (int i = 0; i < calipoint.transform.childCount; i++)
        // {
        //     calipoint.transform.GetChild(i).gameObject.SetActive(false);
        // }
        //
        //
        // isCalibrating = false;
        //
        //
        // yield return new WaitForSeconds(4f);
        //
        // if (MonitorManager.isEyeAutoCaliOpen)
        // {
        //     //通知服务切换到其他应用
        //     bool b_cameraDistance = Gaze.CameraDistance(1);
        //     UE_LOG(LogTemp, Log, TEXT("XrFeatureCameraDistance(1):{b_cameraDistance}");
        // }
        //
        // SceneManager.LoadScene("LauncherPCVR");
        //
        //


}
