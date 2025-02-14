// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameGameMode.generated.h"

UCLASS(minimalapi)
class AFPSGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameGameMode();

protected:
	virtual void BeginPlay() override;
};



