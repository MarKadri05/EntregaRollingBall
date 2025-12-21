#pragma once

#include "CoreMinimal.h"
#include "Ball.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();
	
protected:
	UPROPERTY()
	TSubclassOf<ABall> MyPawnClass;
};