// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoostTrap.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API ABoostTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoostTrap();

private:	
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	UMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	class UBoxComponent* TriggerBox;
#pragma endregion

	float BoostForce = 5000.f;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);
	
	void ApplyForce(UPrimitiveComponent* PlayerComponent);
};