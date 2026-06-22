#include "GravityZone.h"
#include "Ball.h"
#include "Components/BoxComponent.h"

AGravityZone::AGravityZone()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AGravityZone::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AGravityZone::OnOverlapEnd);
}

void AGravityZone::BeginPlay()
{
	Super::BeginPlay();
}

// Invertir gravedad al estar en la zona
void AGravityZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ABall* Ball = Cast<ABall>(OtherActor);
	if (!Ball) return;

	// Cancelar exit pendiente si vuelve a entrar
	GetWorldTimerManager().ClearTimer(ExitTimerHandle);
	PendingExitBall = nullptr;
	
	Ball->SetGravityInverted(true);
	Ball->Set2DMode(true);
}

// Devolver gravedad a la normalidad tras pasar x tiempo
void AGravityZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABall* Ball = Cast<ABall>(OtherActor);
	if (!Ball) return;

	Ball->SetGravityInverted(false);
	
	PendingExitBall = Ball;
	GetWorldTimerManager().SetTimer(ExitTimerHandle, this, &AGravityZone::ExitGravityZone,
		0.4f, false);
}

void AGravityZone::ExitGravityZone()
{
	if (PendingExitBall)
	{
		PendingExitBall->Set2DMode(false);
		PendingExitBall = nullptr;
	}
}