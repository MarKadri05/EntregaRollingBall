#include "BlinkingPlatform.h"

ABlinkingPlatform::ABlinkingPlatform()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	
	SetRootComponent(Root);
	PlatformMesh->SetupAttachment(Root);
	PlatformMesh->SetMassOverrideInKg(NAME_None, 1000000);
}

// Iniciar temporizador
void ABlinkingPlatform::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(BlinkTimerHandle, this, &ABlinkingPlatform::Blink, 2, true);
}

// Cambiar estado de la plataforma
void ABlinkingPlatform::Blink()
{
	if (bStartsVisible)
	{
		PlatformMesh->SetVisibility(false);
		PlatformMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bStartsVisible = false;
	} else
	{
		PlatformMesh->SetVisibility(true);
		PlatformMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		bStartsVisible = true;
	}
}