// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();
	// Called every frame
    virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	USceneComponent* SceneComponent;
    	
	UPROPERTY(EditAnywhere, Category= "Component")
	UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere, Category= "Component")
	FRotator MyRotator;
#pragma endregion
};
