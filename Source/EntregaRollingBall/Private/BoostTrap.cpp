#include "BoostTrap.h"
#include "Components/BoxComponent.h"

ABoostTrap::ABoostTrap()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	
	SetRootComponent(Mesh);
	TriggerBox->SetupAttachment(Mesh);
	
	// Configuración del trigger
	TriggerBox->SetBoxExtent(FVector(55.f, 55.f, 150.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABoostTrap::OnOverlapBegin);
}

// Detectar que el player toque la trampa
void ABoostTrap::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	
	if (OtherActor->ActorHasTag("Player")) ApplyForce(OtherComp);
}

// Aplicar boost al player
void ABoostTrap::ApplyForce(UPrimitiveComponent* PlayerComponent)
{
	// Impulsar en dirección de la trampa
	FVector BoostDirection = GetActorForwardVector();
	
	PlayerComponent->AddImpulse(BoostDirection * BoostForce, NAME_None, true);
}