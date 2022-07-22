// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LookAtTriggerActor.generated.h"

UCLASS()
class MATHPROJECT_API ALookAtTriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALookAtTriggerActor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BallMesh;

private:
	UPROPERTY(EditInstanceOnly, Category = "Setting Radius Trigger", meta = (ClampMin = "30.0", ClampMax = "150.0"))
	float RadiusTrigger{100.0f};

	UPROPERTY(EditInstanceOnly, Category = "Setting Radius Trigger", meta = (ClampMin = "-200.0", ClampMax = "200.0"))
	float BallPosition{200.0f};

	void UpdateEditorDraw();
};