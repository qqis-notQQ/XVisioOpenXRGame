// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeGazePawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EyeGazeCalibrationGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
AEyeGazePawn::AEyeGazePawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// 创建摄像机组件
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->bLockToHmd = true; // 默认跟随HMD
	
	
	// IMC 路径： /Script/EnhancedInput.InputMappingContext'/Game/input/IMC_Test.IMC_Test'
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("InputMappingContext'/Game/input/IMC_Test.IMC_Test'"));

	// IA_Move /Script/EnhancedInput.InputAction'/Game/input/Actions/IA_G_Key.IA_G_Key'
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_GKey(TEXT("InputAction'/Game/input/Actions/IA_G_Key.IA_G_Key'"));

	 check(IMC.Succeeded());
	 check(IA_GKey.Succeeded());
	
	DefaultMappingContext = IMC.Object;
	GKeyAction = IA_GKey.Object;
	
}

// Called when the game starts or when spawned
void AEyeGazePawn::BeginPlay()
{
	Super::BeginPlay();
	
	// 延迟一帧，避免打包 / VR 下 LocalPlayer 还没好
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (ULocalPlayer* LP = PC->GetLocalPlayer())
			{
				if (auto* Subsystem =
					LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	});
	
	// 禁止Character移动
	GetCharacterMovement()->DisableMovement();

	// 记录锁定位置
	LockedLocation = VRCamera->GetComponentLocation();
	LockedRotation = VRCamera->GetComponentRotation();

	// 如果需要，可以直接锁定摄像机视角
	if (bLockView)
	{
		LockCameraView();
	}
	
}

// Called every frame
void AEyeGazePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bLockView && VRCamera)
	{
		// 每帧重置摄像机位置和旋转，防止HMD影响
		VRCamera->SetWorldLocationAndRotation(LockedLocation, LockedRotation);
	}

}

// Called to bind functionality to input
void AEyeGazePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput =Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(
			GKeyAction,
			ETriggerEvent::Triggered,
			this,
			&AEyeGazePawn::startCalibration
		);
	}
	

}

void AEyeGazePawn::LockedPawnView(FVector location, FRotator rotation)
{
	bLockView = true;
	LockedLocation = location;
	LockedRotation = rotation;
}

void AEyeGazePawn::startCalibration()
{
	if (AEyeGazeCalibrationGameMode* GM = Cast<AEyeGazeCalibrationGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->StartCalibration();
	}
}

void AEyeGazePawn::LockCameraView()
{
	if (!VRCamera) return;

	// 禁止HMD控制摄像机
	VRCamera->bLockToHmd = false;

	// 重置位置和旋转
	VRCamera->SetWorldLocationAndRotation(LockedLocation, LockedRotation);

	// 禁止Pawn移动
	GetCharacterMovement()->DisableMovement();
}

