#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
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

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::ProjectileHit);
	//El sistema de particulas acompania al componente
	UGameplayStatics::SpawnEmitterAttached(TrailParticles, ProjectileMesh);
}

void AProjectile::ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, Hit.Location);
	UGameplayStatics::ApplyDamage(OtherActor, 10.f, nullptr, this, UDamageType::StaticClass());
	this->Destroy();
}