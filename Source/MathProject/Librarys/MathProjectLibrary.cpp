// Fill out your copyright notice in the Description page of Project Settings.


#include "Librarys/MathProjectLibrary.h"
#include "FlushDebugInterface.h"

void UMathProjectLibrary::Print_Log(ELogVerb LogVerb, FString Text, int Line, const char* Function)
{
	switch (LogVerb)
	{
		case ELogVerb::Display:
			UE_LOG(LogMath, Display, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
			break;

		case ELogVerb::Warning:
			UE_LOG(LogMath, Warning, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
			break;

		case ELogVerb::Error:
			UE_LOG(LogMath, Error, TEXT("[%s][%d] TEXT:[%s]"), ANSI_TO_TCHAR(Function), Line, *Text);
			break;
	}
}

UWorld* UMathProjectLibrary::GetWorldInEditor()
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

void UMathProjectLibrary::RestartPushDebugLine(UWorld* World)
{
	TArray<AActor*> ArrayActors;
	UMathProjectLibrary::FindAllActors(World, ArrayActors);
	for (auto* Actor : ArrayActors)
	{
		IFlushDebugInterface* Interface = Cast<IFlushDebugInterface>(Actor);
		if (Interface)
		{
			Interface->OnFlushPersistentDebug_Implementation(World);
		}
	}
}
