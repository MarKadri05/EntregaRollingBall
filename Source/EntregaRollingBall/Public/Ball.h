#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API ABall : public APawn
{
	GENERATED_BODY()

public:
	ABall();
	void SetBallController(AMyPlayerController* MyPlayerController);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Move(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	
	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
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
	float JumpForce = 65000.f;

	// Velocidad mínima del player para que suene el audio
	float MoveSoundMinSpeed = 250.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "My Data")
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "My Data")
	class UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "My Data")
	int DamageTaken;
	
	bool bIsGrounded = false;
#pragma endregion
	
#pragma region Sounds
	UPROPERTY(EditDefaultsOnly, Category= "Sounds")
	USoundBase* MoveSound;
	
	UPROPERTY(EditDefaultsOnly, Category= "Sounds")
	USoundBase* JumpSound;
	
	UPROPERTY(EditDefaultsOnly, Category= "Sounds")
	USoundBase* PickUpCoinSound;
	
	UPROPERTY(EditDefaultsOnly, Category= "Sounds")
	USoundBase* DeathSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	UAudioComponent* MoveAudioComponent;
#pragma endregion
	
	UPROPERTY()
	AMyPlayerController* BallController;
	
	UFUNCTION()
	void OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
		class AController* InstigatedBy, AActor* DamageCauser);
	
	void CheckGrounded();
};