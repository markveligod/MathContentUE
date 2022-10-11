// Fill out your copyright notice in the Description page of Project Settings.

#include "RadialTrigger/RadialTriggerActor.h"
#include "Librarys/MathProjectLibrary.h"

#pragma region Default

// Sets default values
ARadialTriggerActor::ARadialTriggerActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(FName("Scene root component"));
    SetRootComponent(SceneRoot);

    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ball static mesh component"));
    BallMesh->SetupAttachment(SceneRoot);

    const FVector DirPos = FVector(BallPosition, 0.0f, 0.0f);
    BallMesh->SetRelativeLocation(DirPos);
    ColorTrigger = ((GetActorLocation() + DirPos) - GetActorLocation()).Length() > RadiusTrigger ? FColor::Red : FColor::Green;
}

void ARadialTriggerActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    MoveTimeline.TickTimeline(DeltaSeconds);
    InflateTimeline.TickTimeline(DeltaSeconds);

    DrawDebugSphere(GetWorld(), GetActorLocation(), RadiusTrigger, 12, ColorTrigger, false, 0, 0, 2);
}

// Called when the game starts or when spawned
void ARadialTriggerActor::BeginPlay()
{
    Super::BeginPlay();

    FOnTimelineFloat MoveTimelineCallback;
    MoveTimelineCallback.BindDynamic(this, &ARadialTriggerActor::UpdateMoveTimeline);
    MoveTimeline.AddInterpFloat(MoveCurve, MoveTimelineCallback);
    MoveTimeline.SetLooping(true);
    MoveTimeline.Play();

    FOnTimelineFloat InflateTimelineCallback;
    InflateTimelineCallback.BindDynamic(this, &ARadialTriggerActor::UpdateInflateTimeline);
    InflateTimeline.AddInterpFloat(InflateCurve, InflateTimelineCallback);
    InflateTimeline.SetLooping(true);
    InflateTimeline.Play();
}

void ARadialTriggerActor::OnFlushPersistentDebug_Implementation(UWorld* World)
{
    UpdateEditorDraw();
}

#pragma endregion

#pragma region EditorData

void ARadialTriggerActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    FlushPersistentDebugLines(UMathProjectLibrary::GetWorldInEditor());
    UMathProjectLibrary::RestartPushDebugLine(UMathProjectLibrary::GetWorldInEditor());
}

void ARadialTriggerActor::UpdateEditorDraw()
{
    const FVector DirPos = FVector(BallPosition, 0.0f, 0.0f);
    BallMesh->SetRelativeLocation(DirPos);
    ColorTrigger = ((GetActorLocation() + DirPos) - GetActorLocation()).Length() > RadiusTrigger ? FColor::Red : FColor::Green;
    DrawDebugSphere(UMathProjectLibrary::GetWorldInEditor(), GetActorLocation(), RadiusTrigger, 12, ColorTrigger, true, 0, 0, 2);
}

#pragma endregion

void ARadialTriggerActor::UpdateMoveTimeline(float NewValue)
{
    const FVector DirPos = FVector(NewValue, 0.0f, 0.0f);
    BallMesh->SetRelativeLocation(DirPos);
    ColorTrigger = ((GetActorLocation() + DirPos) - GetActorLocation()).Length() > RadiusTrigger ? FColor::Red : FColor::Green;
}

void ARadialTriggerActor::UpdateInflateTimeline(float NewValue)
{
    RadiusTrigger = NewValue;
}
