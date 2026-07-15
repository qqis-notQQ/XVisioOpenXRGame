// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EyeGazePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class XVISIOOPENXRGAME_API AEyeGazePlayerController : public APlayerController
{
	GENERATED_BODY()

	AEyeGazePlayerController();

protected:
	virtual void BeginPlay() override;
};
