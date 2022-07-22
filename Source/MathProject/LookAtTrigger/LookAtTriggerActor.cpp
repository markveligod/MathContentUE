// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtTrigger/LookAtTriggerActor.h"

// Sets default values
ALookAtTriggerActor::ALookAtTriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(FName("Scene root component"));
	SetRootComponent(SceneRoot);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ball static mesh component"));
	BallMesh->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void ALookAtTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALookAtTriggerActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateEditorDraw();
}

void ALookAtTriggerActor::UpdateEditorDraw()
{
	const FVector DirPos = FVector(BallPosition,0.0f,0.0f);
	BallMesh->SetRelativeLocation(DirPos);

	FlushPersistentDebugLines(GetWorld());
	DrawDebugSphere(GetWorld(), GetActorLocation(), RadiusTrigger, 12, FColor::Cyan,true, 0, 0, 2);
}

// Called every frame
void ALookAtTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

