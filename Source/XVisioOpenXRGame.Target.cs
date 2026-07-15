// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class XVisioOpenXRGameTarget : TargetRules
{
	public XVisioOpenXRGameTarget(TargetInfo Target) : base(Target)
	{
        
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

        //BuildEnvironment = TargetBuildEnvironment.Unique;

        ExtraModuleNames.AddRange( new string[] { "XVisioOpenXRGame","LibSDK" } );
	}
}
