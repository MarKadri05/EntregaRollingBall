#include "MainMenuController.h"

#include "Blueprint/UserWidget.h"

void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	
	// Mostrar ratón para poder hacer click en los botones
	bShowMouseCursor = true;
	
	// Crear y añadir widget a la pantalla
	if (Wbp_MainMenu)
	{
		UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(this, Wbp_MainMenu);
		MainMenuWidget->AddToViewport();
	}
}