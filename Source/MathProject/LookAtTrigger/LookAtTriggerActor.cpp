// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtTrigger/LookAtTriggerActor.h"
#include "Librarys/MathProjectLibrary.h"

#pragma region Default

// Sets default values
ALookAtTriggerActor::ALookAtTriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(FName("Scene root component"));
	SetRootComponent(SceneRoot);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ball static mesh component"));
	BallMesh->SetupAttachment(SceneRoot);
	BallMesh->SetRelativeLocation(FVector(200.0f,0.0f,0.0f));
}

// Called every frame
void ALookAtTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveBallTimeline.TickTimeline(DeltaTime);
	RotateBallTimeline.TickTimeline(DeltaTime);
	InflateTimeline.TickTimeline(DeltaTime);

	UpdateDraw(GetWorld(), false);
}

// Called when the game starts or when spawned
void ALookAtTriggerActor::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat MoveBallTimelineCallback;
	MoveBallTimelineCallback.BindDynamic(this, &ALookAtTriggerActor::UpdateMoveBallTimeline);
	MoveBallTimeline.AddInterpFloat(MoveBallCurve, MoveBallTimelineCallback);
	MoveBallTimeline.SetLooping(true);
	MoveBallTimeline.Play();

	FOnTimelineFloat RotateBallTimelineCallback;
	RotateBallTimelineCallback.BindDynamic(this, &ALookAtTriggerActor::UpdateRotateBallTimeline);
	RotateBallTimeline.AddInterpFloat(RotateBallCurve, RotateBallTimelineCallback);
	RotateBallTimeline.SetLooping(true);
	RotateBallTimeline.Play();

	FOnTimelineFloat InflateTimelineCallback;
	InflateTimelineCallback.BindDynamic(this, &ALookAtTriggerActor::UpdateInflateTimeline);
	InflateTimeline.AddInterpFloat(InflateCurve, InflateTimelineCallback);
	InflateTimeline.SetLooping(true);
	InflateTimeline.Play();
}

void ALookAtTriggerActor::OnFlushPersistentDebug_Implementation(UWorld* World)
{
	UpdateDraw(World, true);
}

#pragma endregion

#pragma region EditorData

void ALookAtTriggerActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FlushPersistentDebugLines(UMathProjectLibrary::GetWorldInEditor());
	UMathProjectLibrary::RestartPushDebugLine(UMathProjectLibrary::GetWorldInEditor());
}

#pragma endregion

void ALookAtTriggerActor::UpdateDraw(UWorld* World, bool bPersistentLines)
{
	BallMesh->SetRelativeRotation(FRotator(0.0f, RotZAxis, 0.0f));
	FVector RelPos = BallMesh->GetRelativeLocation();
	RelPos.Y = BallPosition;
	BallMesh->SetRelativeLocation(RelPos);
	
	const FVector ForwardVector = BallMesh->GetComponentLocation() + BallMesh->GetForwardVector() * RadiusTrigger;
	const FVector UpVector = BallMesh->GetComponentLocation() + BallMesh->GetUpVector() * RadiusTrigger;
	const FVector DirToSphere = (GetActorLocation() - BallMesh->GetComponentLocation()).GetSafeNormal();
	const FVector ToSpherePos = BallMesh->GetComponentLocation() + DirToSphere * RadiusTrigger;
	const float Result = FVector::DotProduct(BallMesh->GetForwardVector(), DirToSphere);
	ColorDotResult = (Result >= 1.0f - DeltaSuccessResult) ? FColor::Orange : FColor::Red;
	
	DrawDebugSphere(World, GetActorLocation(), RadiusTrigger, 12, FColor::Cyan,bPersistentLines, 0, 0, 2);
	DrawDebugDirectionalArrow(World, BallMesh->GetComponentLocation(), ForwardVector, 5.0f, ColorDotResult, bPersistentLines, 0, 0, 2);
	DrawDebugDirectionalArrow(World, BallMesh->GetComponentLocation(), UpVector, 5.0f, FColor::Blue, bPersistentLines, 0, 0, 2);
	DrawDebugLine(World, BallMesh->GetComponentLocation(), ToSpherePos, FColor::Orange, bPersistentLines, 0, 0, 2);
}

void ALookAtTriggerActor::UpdateMoveBallTimeline(float NewValue)
{
	BallPosition = NewValue;
}

void ALookAtTriggerActor::UpdateInflateTimeline(float NewValue)
{
	RadiusTrigger = NewValue;
}

void ALookAtTriggerActor::UpdateRotateBallTimeline(float NewValue)
{
	RotZAxis = NewValue;
}
