#include "MovingPlatform.h"
#include "Components/SplineComponent.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	SetRootComponent(Root);
	PlatformMesh->SetupAttachment(Root);
	Spline->SetupAttachment(Root);
	
	// Configuración de la plataforma
	PlatformMesh->SetSimulatePhysics(true);
	PlatformMesh->SetMassOverrideInKg(NAME_None, 100000000);
	PlatformMesh->SetEnableGravity(false);
}

// Calcular puntos del spline y a cual tiene que moverse
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	SplinePointsNum = Spline->GetNumberOfSplinePoints();
	CurrentPointIndex = 0;
	MovePlatform();
}

// Comprobar si la plataforma ha llegado al punto de destino
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsWaiting) return;
	
	if (FVector::Dist(PlatformMesh->GetComponentLocation(), CurrentDestination) < 5.0f)
	{
		bIsWaiting = true;
		PlatformMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GetWorldTimerManager().SetTimer(MovementTimerHandle, this,
		&AMovingPlatform::MovePlatform,3, false);
	}
}

// Mover la plataforma
void AMovingPlatform::MovePlatform()
{
	bIsWaiting = false;
	CalculateNewDestination();
	PlatformMesh->SetPhysicsLinearVelocity(CurrentDirection * Speed);
}

// Calcular siguiente punto del spline al que ir
void AMovingPlatform::CalculateNewDestination()
{
	CurrentDestination = Spline->GetLocationAtSplinePoint(CurrentPointIndex, ESplineCoordinateSpace::World);
	CurrentDirection = (CurrentDestination - PlatformMesh->GetComponentLocation()).GetSafeNormal();
	CurrentPointIndex = (CurrentPointIndex + 1) % SplinePointsNum;
}