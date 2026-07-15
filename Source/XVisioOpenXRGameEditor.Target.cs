// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class XVisioOpenXRGameEditorTarget : TargetRules
{
	public XVisioOpenXRGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		//BuildEnvironment = TargetBuildEnvironment.Unique;

		//BuildEnvironment = TargetBuildEnvironment.Unique;

        ExtraModuleNames.AddRange( new string[] { "XVisioOpenXRGame","LibSDK" } );
	}
}
