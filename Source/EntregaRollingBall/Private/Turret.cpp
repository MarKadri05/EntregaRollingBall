#include "Turret.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TurretBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	TurretTower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	
	SetRootComponent(TurretBase);
	TurretTower->SetupAttachment(GetRootComponent());
	RangeSphere->SetupAttachment(GetRootComponent());
	ProjectileSpawnPoint->SetupAttachment(TurretTower);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnOverlap);
	RangeSphere->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnEndOverlap);
}

//Funcion encargada de spawnear los proyectiles
void ATurret::SpawnProjectile() const
{
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
}

// Called every frame
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

void ATurret::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Player"))
	{
		TargetActor = OtherActor;
		
		GetWorldTimerManager().SetTimer(FireRateTimer, this, &ATurret::SpawnProjectile, FireRate, true);
	}
}

void ATurret::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		TargetActor = nullptr;
		GetWorldTimerManager().ClearTimer(FireRateTimer);
	}
}