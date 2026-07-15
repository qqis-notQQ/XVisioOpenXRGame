// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Modules/ModuleManager.h"

class FLibSDK : public IModuleInterface
{
public:
	virtual void StartupModule();
	virtual void ShutdownModule();
};

