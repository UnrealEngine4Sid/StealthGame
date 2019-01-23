// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	SphereCenterComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCenterComp"));
	SphereCenterComponent->SetSphereRadius(100);
	SphereCenterComponent->SetupAttachment(MeshComponent);

	SphereOuterComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOuterComp"));
	SphereOuterComponent->SetSphereRadius(3000);
	SphereOuterComponent->SetupAttachment(MeshComponent);
}

void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	SphereCenterComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapSphereCenter);
}

void AFPSBlackHole::OverlapSphereCenter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();	
	}
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps; // Output variable
	SphereOuterComponent->GetOverlappingComponents(OverlappingComps);

	// Loop through all overlapping actors
	for (UPrimitiveComponent* PrimComp : OverlappingComps)
	{
		//UPrimitiveComponent* PrimComp = OverlappingComps[i];

		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = SphereOuterComponent->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}

}

