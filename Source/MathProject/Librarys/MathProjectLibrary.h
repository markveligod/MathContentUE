// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathProjectLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MATHPROJECT_API UMathProjectLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MathProjectLibrary")
	static UWorld* GetWorldInEditor()
	{
		TIndirectArray<FWorldContext> LWorlds = GEngine->GetWorldContexts();

		for (FWorldContext LWorld : LWorlds)
		{
			if (LWorld.WorldType == EWorldType::Editor)
			{
				return LWorld.World();
			}
		}

		return nullptr;
	}
};
