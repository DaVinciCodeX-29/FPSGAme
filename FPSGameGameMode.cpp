// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSGameGameMode.h"
#include "FPSHUD.h"
#include "FPSGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"

AFPSGameGameMode::AFPSGameGameMode()
	
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	//TODO NETWORK: Uncomment This, We need to set all the StaticMesh Actors in the level to replicate
	for (TActorIterator<AStaticMeshActor> StaticmeshItr(GetWorld()); StaticmeshItr; ++StaticmeshItr)
	{
		StaticmeshItr->SetReplicates(true);
		FRepMovement mv;
		mv.bRepPhysics = true;

		StaticmeshItr->SetReplicatedMovement(mv);
		StaticmeshItr->GetRootComponent()->SetIsReplicated(true);


	}
}
