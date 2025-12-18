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
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

private:	
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditDefaultsOnly, Category= "Component")
	class USplineComponent* Spline;
#pragma endregion
	
	float Speed = 250.f;
	
	int32 SplinePointsNum, CurrentPointIndex;
	
	FVector CurrentDestination, CurrentDirection;
	
	void CalculateNewDestination();
};