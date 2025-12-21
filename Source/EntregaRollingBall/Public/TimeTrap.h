#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeTrap.generated.h"


UCLASS()
class ENTREGAROLLINGBALL_API ATimeTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	ATimeTrap();

protected:
	virtual void BeginPlay() override;
	
private:
#pragma region Components
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshOff;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshOn;
	
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* TriggerBox;
#pragma endregion
	
#pragma region Data
	// Tiempo de retraso antes de activar la trampa
	float ActivationDelay = 0.9f;
	
	// Booleanos de comprobación para ver si ha detectado al player o está activa la trampa
	bool bTrapActive = false;
	bool bPlayerDetected = false;
	
	FTimerHandle TrapTimerHandle;
	FTimerHandle DamageTimerHandle;
#pragma endregion
	
	UPROPERTY(EditDefaultsOnly, Category= "Sounds")
	USoundBase* ActivateTrapSound;
	
	UPROPERTY(EditDefaultsOnly, Category= "Sounds")
	USoundBase* DeactivateTrapSound;
	
	// Detección del player
	UPROPERTY()
	AActor* OverlappingPlayer = nullptr;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void ToggleTrap();
	void ApplyDamage();
};