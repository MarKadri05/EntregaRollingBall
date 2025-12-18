#include "MainMenuController.h"

#include "Blueprint/UserWidget.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	
	// Mostrar ratón para poder hacer click en los botones
	bShowMouseCursor = true;
	
	// Crear y añadir widget a la pantalla
	if (WBP_MainMenu)
	{
		UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(this, WBP_MainMenu);
		MainMenuWidget->AddToViewport();
	}
}