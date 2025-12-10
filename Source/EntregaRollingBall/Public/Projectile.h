#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category= "Components")
	class UProjectileMovementComponent* ProjectileMovement;
#pragma endregion
	
#pragma region Particles
	UPROPERTY(EditAnywhere, Category= "Particle")
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, Category= "Particle")
	UParticleSystem* TrailParticles;
#pragma endregion
	
	UFUNCTION()
	void ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};