// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EngineUtils.h"
#include "MathProjectLibrary.generated.h"

UENUM(BlueprintType)
enum class ELogVerb : uint8
{
	Display,
	Warning,
	Error
};

DEFINE_LOG_CATEGORY_STATIC(LogMath, All, All);
#define LOG_MATH(LogVerb, Text) UMathProjectLibrary::Print_Log(LogVerb, Text, __LINE__, __FUNCTION__)


/**
 * @class Generalized Math project Functional Library
 */
UCLASS()
class MATHPROJECT_API UMathProjectLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * @public Write a log
	 **/
	static void Print_Log(ELogVerb LogVerb, FString Text, int Line, const char* Function);
	
	UFUNCTION(BlueprintCallable, Category = "MathProjectLibrary")
	static UWorld* GetWorldInEditor();

	UFUNCTION(BlueprintCallable, Category = "MathProjectLibrary")
	static void RestartPushDebugLine(UWorld* World);

	template<typename T>
	static void FindAllActors(UWorld* InWorld, TArray<T*>& OutActors)
	{
		if (!InWorld) return;

		for (TActorIterator<T> Iterator(InWorld); Iterator; ++Iterator)
		{
			OutActors.Add(*Iterator);
		}
	}
};
