#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedPlayerInput.h"
#include "RollingBallSaveGame.h"
#include "MyPlayerController.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void OnLoseLife();
	void UpdateScore();
	void EndGame();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUILives(int32 NewLives);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUIScore(int32 NewScore);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUITimer(int32 Minutes, int32 Seconds);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowVictoryScreen(int32 FinalScore, int32 FinalTime, int32 FinalCoins, const TArray<FScoreEntry>& Leaderboard);
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY(EditAnywhere, Category= "Inputs")
	class UInputMappingContext* MyMappingContext;
	
#pragma region Data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "Data", meta = (AllowPrivateAccess = "true"))
	FTimerHandle GameTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "Data", meta = (AllowPrivateAccess = "true"))
	int32 CurrentLives = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "Data", meta = (AllowPrivateAccess = "true"))
	int32 MaxLives = CurrentLives;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category= "Data", meta = (AllowPrivateAccess = "true"))
	int32 CurrentScore = 0;
	
	int32 ElapsedTime = 0;
#pragma endregion
	
	UPROPERTY(EditAnywhere, Category= "Widget")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	void UpdateTimer();
	void GameOver();
	
	// SaveGame
	int32 CalculateTotalScore(int32 Coins, int32 TimeInSeconds, int32 Lives) const;
	TArray<FScoreEntry> SaveAndGetLeaderboard(int32 Coins, int32 TimeInSeconds, int32 Lives);
};