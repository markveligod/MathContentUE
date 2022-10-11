// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Librarys/FlushDebugInterface.h"
#include "LaserReflectionActor.generated.h"

UCLASS()
class MATHPROJECT_API ALaserReflectionActor : public AActor, public IFlushDebugInterface
{
    GENERATED_BODY()

#pragma region Default

public:
    // Sets default values for this actor's properties
    ALaserReflectionActor();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void OnFlushPersistentDebug_Implementation(UWorld* World) override;

#if WITH_EDITOR

    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

private:
    void UpdateDraw(const UWorld* World, bool bPersistentLines);

#pragma endregion

#pragma region Components

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USceneComponent* SceneRoot{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* RayMesh{nullptr};

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* ReflectionMesh{nullptr};

#pragma endregion

#pragma region Data

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laser Reflection Data")
    FVector DirectionRay{-350.0f, 0.0f, 0.0f};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Laser Reflection Data", meta = (ClampMin = "-150.0", ClampMax = "150.0"))
    float RelPosReflectionMesh{0.0f};

    UPROPERTY(EditAnywhere, Category = "Laser Reflection Data")
    UCurveFloat* MoveRayMeshCurve{nullptr};
    UPROPERTY(EditAnywhere, Category = "Laser Reflection Data")
    UCurveFloat* MoveReflectionMeshCurve{nullptr};

    FTimeline MoveRayMesh;
    FTimeline MoveReflectionMesh;

    float DelayPosRayMesh{0.0f};
    float DelayPosReflectionMesh{0.0f};

    UFUNCTION()
    void UpdateMoveRayMeshTimeline(float NewValue);
    UFUNCTION()
    void UpdateMoveReflectionMeshTimeline(float NewValue);

#pragma endregion
};
