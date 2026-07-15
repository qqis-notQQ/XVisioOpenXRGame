#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEventListenerInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(Blueprintable,MinimalAPI)
class UEventListenerInterface : public UInterface
{
	GENERATED_BODY()
};


class XVISIOOPENXRGAME_API IEventListenerInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnJumpPressed();
	
	// 如果希望提供默认实现
	virtual void OnJumpPressed_Implementation();
};

