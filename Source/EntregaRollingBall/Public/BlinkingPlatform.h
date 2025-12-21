#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlinkingPlatform.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API ABlinkingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlinkingPlatform();
	
protected:
	void Blink();
	virtual void BeginPlay() override;

private:
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category= "Component")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, Category= "Component")
	UStaticMeshComponent* PlatformMesh;
#pragma endregion
	
#pragma region Data
	FTimerHandle BlinkTimerHandle;

	UPROPERTY(EditInstanceOnly, Category= "Data")
	bool bStartsVisible = true;
	
	float BlinkInterval = 2.2f;
#pragma endregion
};