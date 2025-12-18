#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoin();

    virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

private:
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	USceneComponent* SceneComponent;
    	
	UPROPERTY(EditAnywhere, Category= "Component")
	UStaticMeshComponent* MyMesh;

	// La esfera se encarga de la colision
	UPROPERTY(EditAnywhere, Category="Component")
	class USphereComponent* CollisionSphere;
	
	UPROPERTY(EditAnywhere, Category= "Component")
	FRotator MyRotator;
#pragma endregion
};
