#pragma once
#include "Standalone/FXvisioFunctionPlugin.h"
#include "Standalone/KeyEventRunable.h"
#include "Standalone/XvisioOpenXR.h"
#include "Subsystems/WorldSubsystem.h"
#include "UObject/ObjectMacros.h"
#include "IXRTrackingSystem.h"
#include "UXVisioXRWorldSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyDownBP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyUpBP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyDownTickBP);

UCLASS()
class XVISIOOPENXR_API UXVisioXRWorldSubsystem
	: public UWorldSubsystem
	, public FTickableGameObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable, Category = "Xvisio|Input")
	FOnKeyDownBP OnKeyDown;
	
	UPROPERTY(BlueprintAssignable, Category = "Xvisio|Input")
	FOnKeyDownTickBP OnKeyDownTick;
	
	UPROPERTY(BlueprintAssignable, Category = "Xvisio|Input")
	FOnKeyUpBP OnKeyup;

	unsigned char  TagFamily[6] = {'3','6','h','1','1',0};
	double TagSize = 0.16;

	XrAprilTagDataXV tagsArray[32]; // 最大支持32个tag
	uint32_t TagCout;
	int32 MaxArraySize = 32;
	int32 DetectType = 1;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable, Category = "Xvisio")
	bool StartEvent();
	
	UFUNCTION(BlueprintCallable, Category = "Xvisio")
	bool StopEvent();
	
	UFUNCTION(BlueprintCallable, Category = "Xvisio")
	bool testEvent();

	UFUNCTION(BlueprintCallable, Category = "Xvisio")
	bool GetApriltag(const FString& InTagFamily, float InTagSize, int32 InDetectType, int32& OutTagCount);

	UFUNCTION(BlueprintCallable, Category = "Xvisio")
	bool GetApriltagData(int32 Index, int32& OutTagID, FVector& OutPosition, FRotator& OutOrientation, FQuat& OutQuaternion, float& OutConfidence);

	UFUNCTION(BlueprintCallable, Category = "Xvisio")
	bool StopApriltag(int ret);
	
	uint32_t Brightness = 50;
private:
	bool isTickable = false;
	XvisioOpenXR::FXVisioOpenXRModule* xVisioOpenXRModule = nullptr;
	XvisioOpenXR::FKeyEventPlugin* keyEventPlugin = nullptr;
	XvisioOpenXR::AKeyEventRunable* keyEventRunable = nullptr;

	XvisioOpenXR::FXvisioFunctionPlugin* XvisioFunctionPlugin = nullptr;

	IXRTrackingSystem* XRTrackingSystem = nullptr;
	
	void HandleNativeKeyDown();
	
	void HandleNativeKeyUp();
	
	void HandleNativeOnKeyDownTick();
	
};
