// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Librarys/FlushDebugInterface.h"
#include "SpaceTransformActor.generated.h"

class UTextRenderComponent;
UCLASS()
class MATHPROJECT_API ASpaceTransformActor : public AActor, public IFlushDebugInterface
{
    GENERATED_BODY()

#pragma region Default

public:
    // Sets default values for this actor's properties
    ASpaceTransformActor();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void OnFlushPersistentDebug_Implementation(UWorld* World) override;

private:
    void UpdateDraw(const UWorld* World, bool bPersistentLines) const;

#pragma endregion

#pragma region Components

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USceneComponent* SceneRoot;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MainMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UTextRenderComponent* TextRenderWorld;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UTextRenderComponent* TextRenderLocal;

#pragma endregion

#pragma region Data

private:
    UPROPERTY(EditInstanceOnly, Category = "Setting Space Transform")
    FVector LocalTransform{FVector::ZeroVector};
    UPROPERTY(EditInstanceOnly, Category = "Setting Space Transform")
    FVector WorldTransform{FVector::ZeroVector};
    UPROPERTY(EditInstanceOnly, Category = "Setting Space Transform", meta = (ClampMin = "150.0", ClampMax = "200.0", ForceUnits = "cm"))
    float DistanceRenderText{150.0f};
    UPROPERTY(EditAnywhere, Category = "Setting Space Transform")
    UCurveFloat* MoveLocalTransformCurve{nullptr};
    UPROPERTY(EditAnywhere, Category = "Setting Space Transform")
    UCurveFloat* MoveWorldTransformCurve{nullptr};

    FTimeline MoveLocalTransformTimeline;
    FTimeline MoveWorldTransformTimeline;

    float DelayLocalTransform{0.0f};
    float DelayWorldTransform{0.0f};

    FVector LocalToWorld() const;
    FVector WorldToLocal() const;

    UFUNCTION()
    void UpdateLocalTransformTimeline(float NewValue);
    UFUNCTION()
    void UpdateWorldTransformTimeline(float NewValue);

#pragma endregion
};
