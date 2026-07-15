// Fill out your copyright notice in the Description page of Project Settings.


#include "EyePlayActor.h"
#include "IOpenXREyeTrackerModule.h"
#include "EyeTrackerFunctionLibrary.h"
#include "EyeTrackerTypes.h"
#include "IXRTrackingSystem.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEyePlayActor::AEyePlayActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建静态网格体组件
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    
	// 将静态网格体组件设为根组件（Actor的Transform会同步到该组件）
	RootComponent = StaticMeshComponent;

	// （可选）为静态网格体指定默认模型（例如引擎自带的立方体）
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (DefaultMesh.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(DefaultMesh.Object);
	}

	// （可选）设置材质（例如引擎自带的默认材质）
	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (DefaultMaterial.Succeeded())
	{
		StaticMeshComponent->SetMaterial(0, DefaultMaterial.Object);
	}
}

// Called when the game starts or when spawned
void AEyePlayActor::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine->XRSystem.IsValid())
	{
		//获取头显的位置信息
		GEngine->XRSystem->GetCurrentPose(IXRTrackingSystem::HMDDeviceId,Orientation,Position);
		UE_LOG(LogTemp,Log,TEXT("HMD Position: X=%.4f,Y=%.4f,Z=%.4f”"),Position.X,Position.Y,Position.Z);
	}
}

// Called every frame
void AEyePlayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (true)
	if (IOpenXREyeTrackerModule::IsAvailable())
	{
		FEyeTrackerGazeData OutGazeData = FEyeTrackerGazeData();
		//设置初始值()
		OutGazeData.GazeOrigin = FVector(-100,0,150);//当前pwan的初始位置
		OutGazeData.GazeDirection = FVector(1,0,0);//默认向前
		//获取眼控数据
		if (UEyeTrackerFunctionLibrary::GetGazeData(OutGazeData))
		{
			const FVector StartLocation  = OutGazeData.GazeOrigin;
			const FVector Location  = OutGazeData.GazeOrigin  + (OutGazeData.GazeDirection * 300);

			//绘制当前的视线
			FHitResult OutHit;
			bool bTraceSuccess = UKismetSystemLibrary::LineTraceSingle(
				this, 
				StartLocation,
				Location, 
				ETraceTypeQuery::TraceTypeQuery1, 
				false, 
				TArray<AActor*>(), 
				EDrawDebugTrace::ForDuration,
				OutHit, 
				true,
				FLinearColor::Red, 
				FLinearColor::Green, 
				0.1f 
			);

			//设置球的位置
			this->SetActorLocation(Location);
		}
	}
}

