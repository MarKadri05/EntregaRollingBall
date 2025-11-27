#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MyPlayerController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API ABall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABall();
	void SetBallController(AMyPlayerController* MyPlayerController);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void NotifyActorBeginOverlap(AActor* OtherActor);
	void Move(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	
private:	
#pragma region Components
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MyMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* Camera;
#pragma endregion

#pragma region Data
	UPROPERTY(EditDefaultsOnly, Category = "My Data")
	float Force = 100000.f;

	UPROPERTY(EditDefaultsOnly, Category = "My Data")
	float JumpForce = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category = "My Data")
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "My Data")
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "My Data")
	int DamageTaken;
#pragma endregion
	
	UPROPERTY()
	USoundBase* JumpSound;
	
	UPROPERTY()
	AMyPlayerController* BallController;
	
	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};