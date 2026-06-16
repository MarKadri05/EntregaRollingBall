#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityZone.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API AGravityZone : public AActor
{
	GENERATED_BODY()

public:
	
	AGravityZone();
	
protected:
	
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* OverlapBox;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* Mesh;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};