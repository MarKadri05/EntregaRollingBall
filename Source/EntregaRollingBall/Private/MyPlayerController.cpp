#include "MyPlayerController.h"
#include "Ball.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "RollingBallSaveGame.h"

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

// Terminar el juego al llegar a la puntuación necesaria y volver al menu principal
void AMyPlayerController::EndGame()
{
	GetWorldTimerManager().ClearTimer(GameTime);
	bShowMouseCursor = true;
	
	// Guardar en el ranking y obtenerlo actualizado
	TArray<FScoreEntry> Leaderboard = SaveAndGetLeaderboard(CurrentScore, ElapsedTime, CurrentLives);
	int32 FinalScore = CalculateTotalScore(CurrentScore, ElapsedTime, CurrentLives);
	
	ShowVictoryScreen(FinalScore, ElapsedTime, CurrentScore, Leaderboard);
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABall* BallPawn = Cast<ABall>(InPawn);
	
	//El PlayerController le comunica a la bola quien la controla
	BallPawn->SetBallController(this);
}

// Formula para calcular la puntuación
int32 AMyPlayerController::CalculateTotalScore(int32 Coins, int32 TimeInSeconds, int32 Lives) const
{
	int32 BaseScore = FMath::Max(0, Coins * 1000 - TimeInSeconds * 10);
	int32 LostLives = MaxLives - Lives;
	float Multiplier = FMath::Max(0.0f, 1.0f - (LostLives * 0.05));
	return FMath::RoundToInt(BaseScore * Multiplier);
}

// Actualizar leaderboard
TArray<FScoreEntry> AMyPlayerController::SaveAndGetLeaderboard(int32 Coins, int32 TimeInSeconds, int32 Lives)
{
	// Cargar save existente o crear uno nuevo
	URollingBallSaveGame* SaveGame = Cast<URollingBallSaveGame>(
		UGameplayStatics::LoadGameFromSlot(URollingBallSaveGame::SaveSlotName, 0));

	if (!SaveGame)
	{
		SaveGame = Cast<URollingBallSaveGame>(
			UGameplayStatics::CreateSaveGameObject(URollingBallSaveGame::StaticClass()));
	}

	// Crear la nueva entrada
	FScoreEntry NewEntry;
	NewEntry.Score = CalculateTotalScore(Coins, TimeInSeconds, Lives);
	NewEntry.TimeSeconds = TimeInSeconds;
	NewEntry.Coins = Coins;
	NewEntry.Date = FDateTime::Now().ToString(TEXT("%d/%m/%Y"));

	// Añadir y ordenar de mayor a menor puntuación
	SaveGame->TopScores.Add(NewEntry);
	SaveGame->TopScores.Sort([](const FScoreEntry& A, const FScoreEntry& B)
	{
		return A.Score > B.Score;
	});

	// Mantener solo el top 5
	if (SaveGame->TopScores.Num() > URollingBallSaveGame::MaxScores)
	{
		SaveGame->TopScores.SetNum(URollingBallSaveGame::MaxScores);
	}

	UGameplayStatics::SaveGameToSlot(SaveGame, URollingBallSaveGame::SaveSlotName, 0);

	return SaveGame->TopScores;
}