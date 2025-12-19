#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingPlatform();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:	
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditDefaultsOnly, Category= "Component")
	class USplineComponent* Spline;
#pragma endregion
	
#pragma region Data
	float Speed = 250.f;
	
	bool bIsWaiting = false;
	
	int32 SplinePointsNum, CurrentPointIndex;
	
	FVector CurrentDestination, CurrentDirection;
	
	FTimerHandle MovementTimerHandle;
#pragma endregion
	
	void MovePlatform();
	void CalculateNewDestination();
};