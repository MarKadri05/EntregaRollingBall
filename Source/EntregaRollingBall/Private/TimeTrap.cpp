#include "TimeTrap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

ATimeTrap::ATimeTrap()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshOff = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshOff"));
	MeshOn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshOn"));
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	
	SetRootComponent(MeshOff);
	MeshOn->SetupAttachment(RootComponent);
	TriggerBox->SetupAttachment(RootComponent);
	
	// Configuración del trigger
	TriggerBox->SetBoxExtent(FVector(55.f, 55.f, 150.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATimeTrap::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATimeTrap::OnOverlapEnd);
}

void ATimeTrap::BeginPlay()
{
	Super::BeginPlay();
	
	// Empezar con el 1 activado y ocultar el 2
	if(MeshOff) MeshOff->SetVisibility(true);
	if(MeshOn) MeshOn->SetVisibility(false);
}

// El player está en el area de la trampa
void ATimeTrap::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		bPlayerDetected = true;
		OverlappingPlayer = OtherActor;
		GetWorldTimerManager().SetTimer(TrapTimerHandle, this, &ATimeTrap::ToggleTrap, ActivationDelay, false);
	}
}

// El player sale del area de la trampa
void ATimeTrap::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor == OverlappingPlayer)
	{
		bPlayerDetected= false;
		OverlappingPlayer = nullptr;
	}
}

// Función que activa/desactiva la trampa
void ATimeTrap::ToggleTrap()
{
	bTrapActive = !bTrapActive;	// Cambiar estado de la trampa
	
	MeshOff->SetVisibility(!bTrapActive);
	MeshOn->SetVisibility(bTrapActive);
	
	// Capacidad de aplicar daño si la trampa esta activa
	if (bTrapActive){
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ATimeTrap::ApplyDamage, 0.25f, true, 0.f);
	} else
	{
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	}
	
	// Desactivar trampa
	if (bTrapActive)
		GetWorldTimerManager().SetTimer(TrapTimerHandle, this, &ATimeTrap::ToggleTrap, ActivationDelay, false);
}

// Hacer daño al player si pisa la trampa cuando esta activa
void ATimeTrap::ApplyDamage()
{
	if(OverlappingPlayer)
	{
		UGameplayStatics::ApplyDamage(OverlappingPlayer, 10.f, nullptr, this, UDamageType::StaticClass());
	}
}