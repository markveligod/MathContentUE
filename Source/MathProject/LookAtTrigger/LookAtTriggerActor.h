// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Librarys/FlushDebugInterface.h"
#include "LookAtTriggerActor.generated.h"

UCLASS()
class MATHPROJECT_API ALookAtTriggerActor : public AActor, public IFlushDebugInterface
{
	GENERATED_BODY()

#pragma region Default
	
public:
	// Sets default values for this actor's properties
	ALookAtTriggerActor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnFlushPersistentDebug_Implementation(UWorld* World) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion

#pragma region EditorData

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

#pragma endregion

#pragma region Components
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BallMesh;

#pragma endregion

#pragma region Data
	
private:

	void UpdateDraw(UWorld* World, bool bPersistentLines);
	
	UPROPERTY(EditInstanceOnly, Category = "Setting Radius Trigger", meta = (ClampMin = "30.0", ClampMax = "150.0", ForceUnits = "cm"))
	float RadiusTrigger{100.0f};

	UPROPERTY(EditInstanceOnly, Category = "Setting Radius Trigger", meta = (ClampMin = "-200.0", ClampMax = "200.0", ForceUnits = "cm"))
	float BallPosition{0.0f};

	UPROPERTY(EditInstanceOnly, Category = "Setting Radius Trigger", meta = (ClampMin = "-360.0", ClampMax = "360.0", ForceUnits = "deg"))
	float RotZAxis{0.0f};

	UPROPERTY(EditInstanceOnly, Category = "Setting Radius Trigger", meta = (ClampMin = "0.01", ClampMax = "0.5", ForceUnits = "x"))
	float DeltaSuccessResult = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Setting Radius Trigger")
	UCurveFloat* MoveBallCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setting Radius Trigger")
	UCurveFloat* RotateBallCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setting Radius Trigger")
	UCurveFloat* InflateCurve = nullptr;
	
	FColor ColorDotResult = FColor::Red;

	FTimeline MoveBallTimeline;
	FTimeline RotateBallTimeline;
	FTimeline InflateTimeline;

#pragma endregion

private:
	
	UFUNCTION()
	void UpdateMoveBallTimeline(float NewValue);
	UFUNCTION()
	void UpdateInflateTimeline(float NewValue);
	UFUNCTION()
	void UpdateRotateBallTimeline(float NewValue);
	
};
