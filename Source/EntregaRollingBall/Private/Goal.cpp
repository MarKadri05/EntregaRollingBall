#include "Goal.h"
#include "Ball.h"
#include "MyPlayerController.h"
#include "Components/BoxComponent.h"

AGoal::AGoal()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlapBegin);
}

void AGoal::BeginPlay()
{
	Super::BeginPlay();
}

void AGoal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ABall* Ball = Cast<ABall>(OtherActor);
	if (!Ball) return;

	AMyPlayerController* PC = Cast<AMyPlayerController>(Ball->GetController());
	if (!PC) return;

	PC->EndGame();
}