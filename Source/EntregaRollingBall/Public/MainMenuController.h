#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

UCLASS()
class ENTREGAROLLINGBALL_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
protected: 
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> Wbp_MainMenu;
};