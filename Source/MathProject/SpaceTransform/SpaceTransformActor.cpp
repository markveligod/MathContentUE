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

void ASpaceTransformActor::UpdateDraw(UWorld* World, bool bPersistentLines)
{
	FVector NewLocalToWorldPos = LocalToWorld();
	DrawDebugSphere(World, NewLocalToWorldPos, 50.0f, 12, FColor::Cyan,bPersistentLines, 0, 0, 2);
	TextRenderLocal->SetWorldLocation(NewLocalToWorldPos + FVector(0.f, 0.f, 1.f) * 60.0f);
	TextRenderLocal->SetText(FText::FromString(FString::Printf(TEXT("Local to World Space: X=%3.1f\n Y=%3.1f\n Z=%3.1f"), NewLocalToWorldPos.X, NewLocalToWorldPos.Y, NewLocalToWorldPos.Z)));
}

FVector ASpaceTransformActor::LocalToWorld()
{
	FVector LocalNewSpace = FVector(LocalTransform.X * GetActorForwardVector() + LocalTransform.Y * GetActorRightVector() + LocalTransform.Z * GetActorUpVector());
	return GetActorLocation() + LocalNewSpace;
}

// Called every frame
void ASpaceTransformActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma endregion


