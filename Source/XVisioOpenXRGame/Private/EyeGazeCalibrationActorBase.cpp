// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeGazeCalibrationActorBase.h"

#include "EyeGazeCalibrationGameMode.h"
#include "Misc/FileHelper.h"
#include "Standalone/XvisioOpenXR.h"

// Sets default values
AEyeGazeCalibrationActorBase::AEyeGazeCalibrationActorBase() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建静态网格组件并将其设置为根组件
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent; // 将网格组件设置为根组件
	
	pointIndex = -1;
	
}

void AEyeGazeCalibrationActorBase::BroadcastOnPointCollected()
{
	if (OnPointCollected.IsBound())
	{
		UE_LOG(LogTemp, Log, TEXT("OnPointCollected is  IsBound  OK !!!!!!"));
		//uint32_t ret;
		//Gaze->GazeCalibrationPubSetConfGaze(0,1,0.1,0.2,&ret);
		
		// 每 0.1 秒执行一次
		GetWorld()->GetTimerManager().SetTimer(
			RepeatTimerHandle,
			this,
			&AEyeGazeCalibrationActorBase::Calibration,
			1.0f,
			true
		);
		
		// 5 秒后停止
		GetWorld()->GetTimerManager().SetTimer(
			StopTimerHandle,
			this,
			&AEyeGazeCalibrationActorBase::StopCalibration,
			5.0f,
			false
		);
		
		//BroadcastAfterDelay();
		// //模拟处理 等待10秒钟返回
		// // 设置定时器模拟 10 秒后广播
		// GetWorldTimerManager().ClearTimer(TimerHandle);
		// GetWorld()->GetTimerManager().SetTimer(
		// 	TimerHandle,
		// 	this,
		// 	&AEyeGazeCalibrationActorBase::BroadcastAfterDelay,
		// 	10.0f,
		// 	false
		// );
		
		//OnPointCollected.Broadcast(pointIndex,1);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("!!!!!!   OnPointCollected is  IsBound  OK "));
	}
	
}

void AEyeGazeCalibrationActorBase::Calibration()
{
	FVector Vector = this->GetActorLocation();

	switch (pointIndex)
	{
	case 1:
		Gaze->GazeCalibrationPubSetConfGaze(0,0,Vector.Y,Vector.Z,&ret);
		break;
	case 2:
		Gaze->GazeCalibrationPubSetConfGaze(1,1,Vector.Y,Vector.Z,&ret);
		break;
	case 3:
		Gaze->GazeCalibrationPubSetConfGaze(0,2,Vector.Y,Vector.Z,&ret);
		break;
	case 4:
		Gaze->GazeCalibrationPubSetConfGaze(1,2,Vector.Y,Vector.Z,&ret);
		break;
	case 5:
		Gaze->GazeCalibrationPubSetConfGaze(0,1,Vector.Y,Vector.Z,&ret);
		Gaze->GazeCalibrationPubSetConfGaze(1,0,Vector.Y,Vector.Z,&ret);
		break;
	}
}

void AEyeGazeCalibrationActorBase::StopCalibration()
{
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
	BroadcastAfterDelay();
}


void AEyeGazeCalibrationActorBase::BroadcastAfterDelay()
{
	UE_LOG(LogTemp, Log, TEXT("Broadcasting OnPointCollected"));
	OnPointCollected.Broadcast(pointIndex, ret); //
}

void AEyeGazeCalibrationActorBase::OnCalibrationStateChanged_Implementation(EEyeGazeCalibrationState NewState,int id)
{
	switch (NewState)
	{
	case EEyeGazeCalibrationState::Preparing:
		break;

	case EEyeGazeCalibrationState::Running:
		UE_LOG(LogTemp, Log, TEXT("Actor get State: Running"));
		break;

	case EEyeGazeCalibrationState::Collecting:
		UE_LOG(LogTemp, Log, TEXT("Actor get State: Collecting data"));
		break;

	case EEyeGazeCalibrationState::Finished:
		UE_LOG(LogTemp, Log, TEXT("Actor get State: Finished"));
		break;

	case EEyeGazeCalibrationState::Failed:
		UE_LOG(LogTemp, Error, TEXT("EActor get State: Failed"));
		break;

	default:
		break;
	}
}

// Called when the game starts or when spawned
void AEyeGazeCalibrationActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	Module = XvisioOpenXR::FXVisioOpenXRModule::Get();
	Gaze = &Module->GetGazeCalibrationPlugin();
	
	if (Gaze == nullptr && Module == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR EyeGazeCalibrationActor"));
	}
	
	if (AEyeGazeCalibrationGameMode* GM = Cast<AEyeGazeCalibrationGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnCalibrationStateChanged.AddDynamic(this,&AEyeGazeCalibrationActorBase::OnCalibrationStateChanged);
	}

	if (!this->Tags.IsEmpty())
	{
		isCenter = this->Tags.Contains(TEXT("Sphere6"));
	}
	
}

// Called every frame
void AEyeGazeCalibrationActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


