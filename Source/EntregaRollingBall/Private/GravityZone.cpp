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

	Ball->SetGravityInverted(true);
	Ball->Set2DMode(true);
}

// Devolver gravedad a la normalidad
void AGravityZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABall* Ball = Cast<ABall>(OtherActor);
	if (!Ball) return;

	Ball->SetGravityInverted(false);
	Ball->Set2DMode(false);
}