#include "Coin.h"

ACoin::ACoin()
{
	PrimaryActorTick.bCanEverTick = true;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	SetRootComponent(SceneComponent);
	MyMesh->SetupAttachment(RootComponent);
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(MyRotator * DeltaTime); //Rotación en el eje z
}