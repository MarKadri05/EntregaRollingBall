#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	
protected:
	virtual void BeginPlay() override;

private:
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, Category= "Components")
	class UProjectileMovementComponent* ProjectileMovement;
#pragma endregion
	
#pragma region Particles
	UPROPERTY(EditDefaultsOnly, Category= "Particle")
	UParticleSystem* HitParticles;

	UPROPERTY(EditDefaultsOnly, Category= "Particle")
	UParticleSystem* TrailParticles;
#pragma endregion
	
#pragma region Sounds
	UPROPERTY(EditDefaultsOnly, Category= "Sounds")
	USoundBase* HitSound;
#pragma endregion
	
	float ProjectileSpeed = 2250.f;
	
	UFUNCTION()
	void ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};