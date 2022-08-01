// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void UpdateDraw(UWorld* World, bool bPersistentLines);

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
	FVector LocalTransform = FVector::ZeroVector;


	FVector LocalToWorld();

#pragma endregion
	
};
