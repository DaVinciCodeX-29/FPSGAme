// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSGameProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AFPSGameProjectile::AFPSGameProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSGameProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AFPSGameProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		// Set random color
		SetRandomColor(OtherComp);

		// Scale the object
		SetScale(OtherActor, OtherComp);

		// Destroy the projectile
		Destroy();
	}
}

void AFPSGameProjectile::SetRandomColor(UPrimitiveComponent* OtherComp)
{
	// Create a dynamic material instance for changing color
	UMaterialInstanceDynamic* MatInst = OtherComp->CreateAndSetMaterialInstanceDynamic(0);
	if (MatInst)
	{
		MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
	}
}

void AFPSGameProjectile::SetScale(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	FVector Scale = OtherComp->GetComponentScale();
	Scale *= 0.8f; // Scale down by 20%
	if (Scale.GetMin() < 0.5f)
	{
		OtherActor->Destroy();
	}
	else
	{
		OtherComp->SetWorldScale3D(Scale);
	}
}