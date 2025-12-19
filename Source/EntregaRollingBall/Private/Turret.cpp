#include "Turret.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	TurretTower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	
	RangeSphere->SetSphereRadius(800.f);
	
	SetRootComponent(TurretBase);
	TurretTower->SetupAttachment(GetRootComponent());
	RangeSphere->SetupAttachment(GetRootComponent());
	ProjectileSpawnPoint->SetupAttachment(TurretTower);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnOverlap);
	RangeSphere->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnEndOverlap);
}

// Función encargada de spawnear los proyectiles
void ATurret::SpawnProjectile() const
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());
}

// Rotar torreta cuando ve al player
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActor != nullptr)
	{
		FVector DistanceToTarget = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		FVector InterpVector = FMath::VInterpTo(TurretTower->GetForwardVector(), DistanceToTarget, DeltaTime, RotationSpeed);

		FRotator RotationToTarget = InterpVector.Rotation();
		
		TurretTower->SetWorldRotation(FRotator(0, RotationToTarget.Yaw, 0));
	}
}


// El player entra en el rango de la torreta
void ATurret::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		TargetActor = OtherActor;
		
		GetWorldTimerManager().SetTimer(FireRateTimer, this, &ATurret::SpawnProjectile, FireRate, true);
	}
}

// El player sale del rango de la torreta
void ATurret::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		TargetActor = nullptr;
		GetWorldTimerManager().ClearTimer(FireRateTimer);
	}
}