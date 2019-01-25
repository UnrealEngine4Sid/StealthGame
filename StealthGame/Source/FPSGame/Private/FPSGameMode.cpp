// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn)
{
	if(InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		if (SpectatingViewpointClass == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatomgViewTarget is nullptr. Please update gamemode class with valid subclass."))
			return;
		}

		TArray<AActor*> OutReturnActors;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, OutReturnActors);

		// Change view target if any valid actor found
		if (OutReturnActors.Num() > 0)
		{
			AActor* NewViewTarget = OutReturnActors[0];

			APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());

			if (PC)
			{
				PC->SetViewTargetWithBlend(NewViewTarget, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}		
	}

	OnMissionCompleted(InstigatorPawn);	
}
