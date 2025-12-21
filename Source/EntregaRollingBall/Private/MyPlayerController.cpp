#include "MyPlayerController.h"
#include "Ball.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto Input = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Input->AddMappingContext(MyMappingContext, 0);
	
	GetWorldTimerManager().SetTimer(GameTime, this, &AMyPlayerController::UpdateTimer,
		1.0f, true);
}

// Función que actualiza la puntuación
void AMyPlayerController::UpdateScore()
{
	CurrentScore++;
	UpdateUIScore(CurrentScore);
}

// Actualizar contador
void AMyPlayerController::UpdateTimer()
{
	ElapsedTime++;
	UpdateUITimer(ElapsedTime / 60, ElapsedTime % 60);
}

// Función ejecutada cuando el player pierde una vida
void AMyPlayerController::OnLoseLife()
{
	if (CurrentLives > 1)
	{
		// Avisar a BPs que actualize el texto
		CurrentLives--;
		UpdateUILives(CurrentLives);
		
		UnPossess();	// Separar bola del controlador
		
		// Comunicar al GameMode que reinicie el controlador, la bola se pone en el PlayerStart
		GetWorld()->GetAuthGameMode()->RestartPlayer(this);	
	} else
	{
		GameOver();
	}
}

//Pantalla de game over
void AMyPlayerController::GameOver()
{
	GetWorldTimerManager().ClearTimer(GameTime);	// Reiniciar timer al morir
	
	// Mostrar raton
	bShowMouseCursor = true;

	//Mostrar widget de game over
	auto MyWidget = CreateWidget(this, GameOverWidgetClass);
	MyWidget->AddToViewport();
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABall* BallPawn = Cast<ABall>(InPawn);
	
	//El PlayerController le comunica a la bola quien la controla
	BallPawn->SetBallController(this);
}