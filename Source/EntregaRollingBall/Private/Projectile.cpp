#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	ProjectileMesh->SetNotifyRigidBodyCollision(true);

	RootComponent = ProjectileMesh;
	ProjectileMovement->InitialSpeed = 1300.f;
	ProjectileMovement->MaxSpeed = 1300.f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::ProjectileHit);
	UGameplayStatics::SpawnEmitterAttached(TrailParticles, ProjectileMesh);
}

// Función que se activa cuando el proyectil impacta contra algo
void AProjectile::ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, Hit.Location);
	UGameplayStatics::ApplyDamage(OtherActor, 10.f, nullptr, this, UDamageType::StaticClass());
	this->Destroy();
}