// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "EyeGazePawn.generated.h"

UCLASS()
class XVISIOOPENXRGAME_API AEyeGazePawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEyeGazePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void LockedPawnView(FVector location,FRotator rotation);
	
	void startCalibration();
	
private:
	// VR摄像机
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* VRCamera;
	
	/** Enhanced Input */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* GKeyAction;
	

	// 是否锁定视角
	bool bLockView = true;

	// 锁定时摄像机固定位置和旋转
	FVector LockedLocation;
	FRotator LockedRotation;

	// 锁定视角函数
	void LockCameraView();

};
