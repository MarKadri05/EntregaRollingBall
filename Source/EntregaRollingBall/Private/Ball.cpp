#include "Ball.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Creación de componentes
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	// Jerarquía de componentes
	SetRootComponent(MyMesh);
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	
	// Configuración de la cámara
	SpringArm->TargetArmLength = 800;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	
	// Físicas de la bola
	MyMesh->SetSimulatePhysics(true);
	MyMesh->SetMassOverrideInKg(NAME_None, 100);
	MyMesh->SetGenerateOverlapEvents(true);
	MyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	MyMesh->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnMeshBeginOverlap);
}

void ABall::SetBallController(AMyPlayerController* MyPlayerController)
{
	this->BallController = MyPlayerController;
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &ABall::OnDamageTaken);
}

void ABall::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	
	if (OtherActor->ActorHasTag("Coin"))
	{
		BallController->UpdateScore();
		OtherActor->Destroy();
	} 
	else if (OtherActor->ActorHasTag("DeathZone")){
		BallController->OnLoseLife();
		this->Destroy();
	}
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movimiento player
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr)
	{
		// Cuando MoveAction este en triggered, la función Move se va a ejecutar
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABall::Move);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABall::Jump);
	}
}

void ABall::Move(const FInputActionValue& InputActionValue)
{
	FVector2D InputVector = InputActionValue.Get<FVector2D>();

	MyMesh->AddForce(FVector(InputVector.Y, InputVector.X, 0) * Force);
}

void ABall::Jump(const FInputActionValue& InputActionValue)
{
	auto RayEnd = GetActorLocation() + MyMesh->GetCollisionShape().GetExtent().Z * FVector::DownVector;

	if (GetWorld()->LineTraceTestByChannel(GetActorLocation(), RayEnd, ECC_GameTraceChannel1))
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), JumpSound);
		//UGameplayStatics::SpawnEmitterAtLocation()
		MyMesh->AddImpulse(FVector::UpVector * JumpForce);
	}
}

void ABall::OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                          class AController* InstigatedBy, AActor* DamageCauser)
{
	BallController->OnLoseLife();
	this->Destroy();
}