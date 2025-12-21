#include "Coin.h"

#include "Components/SphereComponent.h"

ACoin::ACoin()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));

	SetRootComponent(SceneComponent);
	MyMesh->SetupAttachment(RootComponent);
	CollisionSphere->SetupAttachment(RootComponent);
	
	CollisionSphere->SetSphereRadius(50.f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
	MyRotator = FRotator(0, 90, 0);
}

// Rotar moneda
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(MyRotator * DeltaTime);
}