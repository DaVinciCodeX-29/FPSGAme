// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBombActor.generated.h"

UCLASS()
class FPSGAME_API AFPSBombActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBombActor();

protected:

    // Components
    UPROPERTY(VisibleAnywhere, Category = "BombActor")
    class UStaticMeshComponent* BombMesh;

    UPROPERTY(VisibleAnywhere, Category = "BombActor")
    class UBoxComponent* BombBox;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class URadialForceComponent* RadialForceComp;

    // Variables
    UPROPERTY(EditDefaultsOnly, Category = "BombActor")
    float ExplodeDelay;

    UPROPERTY(EditDefaultsOnly, Category = "BombActor")
    UParticleSystem* ExplosionTemplate;

    UFUNCTION()
    void Explode();
    UFUNCTION()
    void DestroyBomb();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    virtual void Tick(float DeltaTime) override;

#pragma region ICA1
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit);

    void Hold(USkeletalMeshComponent* HoldingComponent);

    void Throw(FVector direction);
#pragma endregion
};
