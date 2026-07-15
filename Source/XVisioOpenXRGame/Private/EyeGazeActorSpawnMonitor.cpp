// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeGazeActorSpawnMonitor.h"

#include "EyeGazeCalibrationActorBase.h"
#include "Kismet/GameplayStatics.h"

UEyeGazeActorSpawnMonitor::UEyeGazeActorSpawnMonitor()
{
    PawnTagValues.Add(TEXT("Floor"));
    
    ActorTagValues.Add(TEXT("Sphere1"));
    ActorTagValues.Add(TEXT("Sphere2"));
    ActorTagValues.Add(TEXT("Sphere3"));
    ActorTagValues.Add(TEXT("Sphere4"));
    ActorTagValues.Add(TEXT("Sphere5"));
    
}

UEyeGazeActorSpawnMonitor::~UEyeGazeActorSpawnMonitor()
{
}

// EyeGazeActorSpawnMonitor.cpp
void UEyeGazeActorSpawnMonitor::StartMonitoring(UWorld* World)
{
    if (!World) return;
    
    TargetWorld = World;
    
    // 绑定Actor生成委托
    ActorSpawnedHandle = World->AddOnActorSpawnedHandler(
        FOnActorSpawned::FDelegate::CreateUObject(this, &UEyeGazeActorSpawnMonitor::OnActorSpawned)
    );

    // 也可以绑定Actor销毁委托（可选）
    // ActorDestroyedHandle = World->AddOnActorDestroyedHandler(...);
    
    // 立即检查已存在的Actor
    TArray<AActor*> ExistingActors;
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), ExistingActors);
    
    for (AActor* Actor : ExistingActors)
    {
        OnActorSpawned(Actor);
    }
}

void UEyeGazeActorSpawnMonitor::OnActorSpawned(AActor* Actor)
{
    if (!Actor) return;
    ActorNum ++;
    UE_LOG(LogTemp, Log, TEXT("Actor生成: %s (类型: %s, Tag: %s ， Actor 数量： %d"),
        *Actor->GetName(),
        *Actor->GetClass()->GetName(),
        Actor->Tags.Num() > 0 ? *Actor->Tags[0].ToString() : TEXT("NoTag"),
        ActorNum);
    
    FString tagValue = Actor->Tags.Num() > 0 ? *Actor->Tags[0].ToString() : TEXT("NoTag");
    
    //等待包Pawn和floor加载完成
    if (Actor->GetClass()->GetName() == "EyeGazePawn")
    {
        //UE_LOG(LogTemp, Warning, TEXT("********* EyeGazePawn is read !!!"));
        PawnIsRead = true;
    }
    
    //加载AEyeGazeCalibrationActorBase的时候，把gamemode和actor绑定委托
    if (Actor->IsA(AEyeGazeCalibrationActorBase::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("********* EyeGazeCalibrationActorBase  !!!"));
        if (AEyeGazeCalibrationGameMode* GM = Cast<AEyeGazeCalibrationGameMode>(GetWorld()->GetAuthGameMode()))
        {
            UE_LOG(LogTemp, Warning, TEXT("********* EyeGazeCalibrationActorBase get gameMode !!!"));
            Cast<AEyeGazeCalibrationActorBase>(Actor)->OnPointCollected.AddDynamic(GM,&AEyeGazeCalibrationGameMode::OnPointCollected);
            //GM->OnCalibrationStateChanged.AddDynamic(this,&AEyeGazeCalibrationActorBase::OnCalibrationStateChanged);
        }
        
    }
    
    //UE_LOG(LogTemp, Warning, TEXT("********* PawnTagValues Num: %d  ,   PawnIsRead : %d  !!!"),PawnTagValues.Num(),PawnIsRead);
    if (PawnTagValues.Contains(tagValue))
    {
        PawnTagValues.Remove(tagValue);
    }
    if (PawnTagValues.Num() == 0 && PawnIsRead)
    {
        //UE_LOG(LogTemp, Warning, TEXT("********* EyeGazePawn 0 !!!"));
        if (InitPawnLocationDelegate.IsBound())
        {
            //UE_LOG(LogTemp, Warning, TEXT("********* EyeGazePawn 1 !!!"));
            InitPawnLocationDelegate.Execute();
            PawnIsRead = false;
        }
        else
        {
            //UE_LOG(LogTemp, Warning, TEXT("********* EyeGazePawn 2 !!!"));
        }
    }
    
    //等待包含指定tag的actor加载完成
    if (ActorTagValues.Contains(tagValue))
    {
        ActorTagValues.Remove(tagValue);
    }
    if (ActorTagValues.Num() == 0 && !PActorTagIsRead)
    {
        if (InitActorWithTagDelegate.IsBound())
        {
            InitActorWithTagDelegate.Execute();
            PActorTagIsRead = true;
        }
    }
    
    //等待所有的actor都加载完成
    if (ActorNum >= 31 && !AllActorIsRead)
    {
        if (InitAllActorDelegate.IsBound())
        {
            InitAllActorDelegate.Execute();
            AllActorIsRead = true;
        }
    }
}
