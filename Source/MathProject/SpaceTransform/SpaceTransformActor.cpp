// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceTransform/SpaceTransformActor.h"
#include "Components/TextRenderComponent.h"
#include "Librarys/MathProjectLibrary.h"

#pragma region Default

// Sets default values
ASpaceTransformActor::ASpaceTransformActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(FName("Root scene component"));
    SetRootComponent(SceneRoot);

    MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Main mesh"));
    MainMesh->SetupAttachment(SceneRoot);

    TextRenderWorld = CreateDefaultSubobject<UTextRenderComponent>(FName("Text render world"));
    TextRenderWorld->SetupAttachment(SceneRoot);

    TextRenderLocal = CreateDefaultSubobject<UTextRenderComponent>(FName("Text render local"));
    TextRenderLocal->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ASpaceTransformActor::BeginPlay()
{
    Super::BeginPlay();

    FOnTimelineFloat MoveLocalTransformTimelineCallback;
    MoveLocalTransformTimelineCallback.BindDynamic(this, &ASpaceTransformActor::UpdateLocalTransformTimeline);
    MoveLocalTransformTimeline.AddInterpFloat(MoveLocalTransformCurve, MoveLocalTransformTimelineCallback);
    MoveLocalTransformTimeline.SetLooping(true);
    MoveLocalTransformTimeline.Play();

    FOnTimelineFloat MoveWorldTransformTimelineCallback;
    MoveWorldTransformTimelineCallback.BindDynamic(this, &ASpaceTransformActor::UpdateWorldTransformTimeline);
    MoveWorldTransformTimeline.AddInterpFloat(MoveWorldTransformCurve, MoveWorldTransformTimelineCallback);
    MoveWorldTransformTimeline.SetLooping(true);
    MoveWorldTransformTimeline.Play();
}

void ASpaceTransformActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    FlushPersistentDebugLines(UMathProjectLibrary::GetWorldInEditor());
    UMathProjectLibrary::RestartPushDebugLine(UMathProjectLibrary::GetWorldInEditor());
}

void ASpaceTransformActor::OnFlushPersistentDebug_Implementation(UWorld* World)
{
    UpdateDraw(World, true);
}

void ASpaceTransformActor::UpdateDraw(const UWorld* World, const bool bPersistentLines) const
{
    const FVector NewLocalToWorldPos = LocalToWorld();
    const FVector NewWorldToLocalPos = WorldToLocal();

    DrawDebugSphere(World, NewLocalToWorldPos, 10.0f, 12, FColor::Green, bPersistentLines, 0, 0, 2);
    DrawDebugSphere(World, MainMesh->GetComponentLocation() + NewWorldToLocalPos, 10.0f, 12, FColor::Red, bPersistentLines, 0, 0, 2);

    TextRenderLocal->SetWorldLocation(GetActorLocation() + FVector(1.f, 0.f, 1.f) * DistanceRenderText);
    TextRenderLocal->SetText(FText::FromString(FString::Printf(TEXT("Local to World Space: X=%3.1f\n Y=%3.1f\n Z=%3.1f"), NewLocalToWorldPos.X, NewLocalToWorldPos.Y, NewLocalToWorldPos.Z)));
    TextRenderLocal->SetTextRenderColor(FColor::Green);
    TextRenderWorld->SetWorldLocation(GetActorLocation() + FVector(-1.f, 0.f, 1.f) * DistanceRenderText);
    TextRenderWorld->SetText(FText::FromString(FString::Printf(TEXT("World Space to Local: X=%3.1f\n Y=%3.1f\n Z=%3.1f"), NewWorldToLocalPos.X, NewWorldToLocalPos.Y, NewWorldToLocalPos.Z)));
    TextRenderWorld->SetTextRenderColor(FColor::Red);
}

FVector ASpaceTransformActor::LocalToWorld() const
{
    const FVector LocalNewSpace = FVector(LocalTransform.X * GetActorForwardVector() + LocalTransform.Y * GetActorRightVector() + LocalTransform.Z * GetActorUpVector());
    return MainMesh->GetComponentLocation() + LocalNewSpace + FVector(0.0f, 0.0f, DelayLocalTransform);
}

FVector ASpaceTransformActor::WorldToLocal() const
{
    const FVector RelPoint = WorldTransform - MainMesh->GetComponentLocation();
    const float X = FVector::DotProduct(RelPoint, GetActorForwardVector());
    const float Y = FVector::DotProduct(RelPoint, GetActorRightVector());
    const float Z = FVector::DotProduct(RelPoint, GetActorUpVector());

    return FVector(X, Y, Z) + FVector(0.0f, 0.0f, DelayWorldTransform);
}

void ASpaceTransformActor::UpdateLocalTransformTimeline(float NewValue)
{
    DelayLocalTransform = NewValue;
}

void ASpaceTransformActor::UpdateWorldTransformTimeline(float NewValue)
{
    DelayWorldTransform = NewValue;
}

// Called every frame
void ASpaceTransformActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MoveLocalTransformTimeline.TickTimeline(DeltaTime);
    MoveWorldTransformTimeline.TickTimeline(DeltaTime);
    UpdateDraw(GetWorld(), false);
}

#pragma endregion
