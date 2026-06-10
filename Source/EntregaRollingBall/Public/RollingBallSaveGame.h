#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RollingBallSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FScoreEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 Score = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 TimeSeconds = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 Coins = 0;

	UPROPERTY(BlueprintReadWrite)
	FString Date = "";
};

UCLASS()
class ENTREGAROLLINGBALL_API URollingBallSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	TArray<FScoreEntry> TopScores;

	static const FString SaveSlotName;
	static const int32 MaxScores = 5;
};