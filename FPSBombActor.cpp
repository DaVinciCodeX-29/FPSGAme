// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBombActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "BombDamageType.h"
#include "TimerManager.h"
#include "Particles/ParticleSystem.h"
// Sets default values
AFPSBombActor::AFPSBombActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BombBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BombBox"));
    BombBox->SetCollisionProfileName("BlockAllDynamic");
    BombBox->OnComponentHit.AddDynamic(this, &AFPSBombActor::OnHit);
    RootComponent = BombBox;

    BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
    BombMesh->SetupAttachment(RootComponent);
    BombMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    RadialForceComp->SetupAttachment(RootComponent);
    RadialForceComp->Radius = 250.0f;
    RadialForceComp->bImpulseVelChange = true;
    RadialForceComp->bAutoActivate = false;
    RadialForceComp->bIgnoreOwningActor = true;

    BombBox->SetSimulatePhysics(true);
    ExplodeDelay = 2.0f;
}

void AFPSBombActor::Explode()
{
    if (ExplosionTemplate)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionTemplate, GetActorLocation());
    }

    RadialForceComp->FireImpulse();

    UGameplayStatics::ApplyRadialDamage(this, 100.0f, GetActorLocation(), RadialForceComp->Radius, UBombDamageType::StaticClass(), {}, this);

    // Schedule the bomb to be destroyed after a delay
    GetWorldTimerManager().SetTimerForNextTick(this, &AFPSBombActor::DestroyBomb);
}

void AFPSBombActor::DestroyBomb()
{
    Destroy();
}

// Called when the game starts or when spawned
void AFPSBombActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSBombActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSBombActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    GetWorldTimerManager().SetTimerForNextTick(this, &AFPSBombActor::Explode);
}

void AFPSBombActor::Hold(USkeletalMeshComponent* HoldingComponent)
{
    if (HoldingComponent)
    {
        BombBox->SetSimulatePhysics(false);
        AttachToComponent(HoldingComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Muzzle");
    }
}

void AFPSBombActor::Throw(FVector direction)
{
    BombBox->SetSimulatePhysics(true);
    BombBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BombBox->SetNotifyRigidBodyCollision(true);
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    BombBox->AddImpulse(direction);
}
