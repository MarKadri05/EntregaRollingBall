#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	ATurret();
	virtual void Tick(float DeltaTime) override;
	
protected:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

private:
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Components")
	UStaticMeshComponent* TurretBase;
	
	UPROPERTY(EditDefaultsOnly, Category= "Components")
	UStaticMeshComponent* TurretTower;
	
	UPROPERTY(EditAnywhere, Category= "Components")
	USceneComponent* ProjectileSpawnPoint;
	
	UPROPERTY(EditDefaultsOnly, Category= "Components")
	class USphereComponent* RangeSphere;
#pragma endregion
	
#pragma region Data
	UPROPERTY()
	AActor* TargetActor = nullptr;
	
	float RotationSpeed = 5.f;
	float FireRate = 2.f;
	
	UPROPERTY(EditDefaultsOnly, Category= "Data")
	TSubclassOf<class AProjectile> ProjectileClass;

	FTimerHandle FireRateTimer;
#pragma endregion
	
	UPROPERTY(EditDefaultsOnly, Category= "Sounds")
	USoundBase* FireSound;
	
	void SpawnProjectile() const;
};