// Fill out your copyright notice in the Description page of Project Settings.

#include "LaserReflection/LaserReflectionActor.h"

#include "Librarys/MathProjectLibrary.h"

#pragma region Default

// Sets default values
ALaserReflectionActor::ALaserReflectionActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(FName("Root scene component"));
    SetRootComponent(SceneRoot);

    RayMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ray mesh"));
    RayMesh->SetupAttachment(SceneRoot);

    ReflectionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Reflection mesh"));
    ReflectionMesh->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ALaserReflectionActor::BeginPlay()
{
    Super::BeginPlay();

    FOnTimelineFloat MoveRayMeshTimelineCallback;
    MoveRayMeshTimelineCallback.BindDynamic(this, &ALaserReflectionActor::UpdateMoveRayMeshTimeline);
    MoveRayMesh.AddInterpFloat(MoveRayMeshCurve, MoveRayMeshTimelineCallback);
    MoveRayMesh.SetLooping(true);
    MoveRayMesh.Play();

    FOnTimelineFloat MoveReflectionMeshTimelineCallback;
    MoveReflectionMeshTimelineCallback.BindDynamic(this, &ALaserReflectionActor::UpdateMoveReflectionMeshTimeline);
    MoveReflectionMesh.AddInterpFloat(MoveReflectionMeshCurve, MoveReflectionMeshTimelineCallback);
    MoveReflectionMesh.SetLooping(true);
    MoveReflectionMesh.Play();
}

void ALaserReflectionActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    FlushPersistentDebugLines(UMathProjectLibrary::GetWorldInEditor());
    UMathProjectLibrary::RestartPushDebugLine(UMathProjectLibrary::GetWorldInEditor());
}

void ALaserReflectionActor::OnFlushPersistentDebug_Implementation(UWorld* World)
{
    UpdateDraw(World, true);
}

#if WITH_EDITOR

void ALaserReflectionActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (!PropertyChangedEvent.Property) return;

    FlushPersistentDebugLines(UMathProjectLibrary::GetWorldInEditor());
    UMathProjectLibrary::RestartPushDebugLine(UMathProjectLibrary::GetWorldInEditor());
}

#endif

// Called every frame
void ALaserReflectionActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MoveRayMesh.TickTimeline(DeltaTime);
    MoveReflectionMesh.TickTimeline(DeltaTime);
    UpdateDraw(GetWorld(), false);
}

void ALaserReflectionActor::UpdateDraw(const UWorld* World, bool bPersistentLines)
{
    FVector L_RelPosRefMesh = ReflectionMesh->GetRelativeLocation();
    L_RelPosRefMesh.Y = RelPosReflectionMesh + DelayPosReflectionMesh;
    ReflectionMesh->SetRelativeLocation(L_RelPosRefMesh);

    FHitResult HitResult;
    const FVector& RayStart = RayMesh->GetComponentLocation();
    DirectionRay.Y = DelayPosRayMesh;
    FVector RayEnd = RayStart + DirectionRay;

    World->LineTraceSingleByChannel(HitResult, RayStart, RayEnd, ECollisionChannel::ECC_Visibility);
    if (HitResult.bBlockingHit)
    {
        RayEnd = HitResult.Location;
        DrawDebugDirectionalArrow(World, RayEnd, RayEnd + HitResult.Normal * 60.0f, 3.0f, FColor::Blue, bPersistentLines, 0.0f, 0, 2.0f);
        FVector L_DirRay = (RayEnd - RayStart).GetSafeNormal();
        FVector DirectionReflection = L_DirRay - 2 * FVector::DotProduct(L_DirRay, HitResult.Normal) * HitResult.Normal;
        DrawDebugLine(World, RayEnd, RayEnd + DirectionReflection * 60.0f, FColor::Red, bPersistentLines, 0.0f, 0, 2.0f);
    }
    DrawDebugLine(World, RayStart, RayEnd, FColor::Red, bPersistentLines, 0.0f, 0, 2.0f);
}

void ALaserReflectionActor::UpdateMoveRayMeshTimeline(float NewValue)
{
    DelayPosRayMesh = NewValue;
}

void ALaserReflectionActor::UpdateMoveReflectionMeshTimeline(float NewValue)
{
    DelayPosReflectionMesh = NewValue;
}

#pragma endregion
