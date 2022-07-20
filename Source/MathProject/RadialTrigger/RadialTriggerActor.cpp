// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialTrigger/RadialTriggerActor.h"

#include "Librarys/MathProjectLibrary.h"

// Sets default values
ARadialTriggerActor::ARadialTriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(FName("Scene root component"));
	SetRootComponent(SceneRoot);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ball static mesh component"));
	BallMesh->SetupAttachment(SceneRoot);

	UpdateEditorDraw();
}

// Called when the game starts or when spawned
void ARadialTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARadialTriggerActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateEditorDraw();
}

void ARadialTriggerActor::UpdateEditorDraw()
{
	const FVector DirPos = FVector(BallPosition,0.0f,0.0f);
	BallMesh->SetRelativeLocation(DirPos);
	ColorTrigger = ((GetActorLocation() + DirPos) - GetActorLocation()).Length() > RadiusTrigger ? FColor::Red : FColor::Green;
	FlushPersistentDebugLines(UMathProjectLibrary::GetWorldInEditor());
	DrawDebugSphere(UMathProjectLibrary::GetWorldInEditor(), GetActorLocation(), RadiusTrigger, 12, ColorTrigger,true, 0, 0, 2);
}

void ARadialTriggerActor::UpdateGameDraw()
{
	FMath::FInterpTo(BallPosition,)
	DrawDebugSphere(GetWorld(), GetActorLocation(), RadiusTrigger, 12, ColorTrigger,false, 0, 0, 2);
}

// Called every frame
void ARadialTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateGameDraw();
}

