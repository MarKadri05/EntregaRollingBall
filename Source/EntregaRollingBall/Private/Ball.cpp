#include "Ball.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Creación de componentes
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MoveAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MoveAudioComponent"));
	
	// Jerarquía de componentes
	SetRootComponent(MyMesh);
	MoveAudioComponent->SetupAttachment(MyMesh);
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	
	MoveAudioComponent->bAutoActivate = false;
	
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

// Tick para comprobar el sonido de moverse
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!MoveSound || !MoveAudioComponent) return;

	const float Speed = MyMesh->GetPhysicsLinearVelocity().Size();
	if (Speed > MoveSoundMinSpeed)
	{
		if (!MoveAudioComponent->IsPlaying())
		{
			MoveAudioComponent->SetSound(MoveSound);
			MoveAudioComponent->Play();
		}
	}
	else
	{
		if (MoveAudioComponent->IsPlaying())
		{
			MoveAudioComponent->Stop();
		}
	}
}

// Comprobar si el player ha interactuado con una moneda o zona de muerte
void ABall::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;
	
	if (OtherActor->ActorHasTag("Coin"))
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), PickUpCoinSound);
		BallController->UpdateScore();
		OtherActor->Destroy();
	} 
	else if (OtherActor->ActorHasTag("DeathZone")){
		BallController->OnLoseLife();
		this->Destroy();
	}
}

// Inputs del player
void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movimiento player
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (Input != nullptr)
	{
		// Cuando MoveAction este en triggered, la función Move se va a ejecutar
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABall::Move);
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ABall::Jump);
	}
}

// Acción de moverse
void ABall::Move(const FInputActionValue& InputActionValue)
{
	FVector2D InputVector = InputActionValue.Get<FVector2D>();
	MyMesh->AddForce(FVector(InputVector.Y, InputVector.X, 0) * Force);
}

// Acción de saltar
void ABall::Jump(const FInputActionValue& InputActionValue)
{
	FHitResult HitResult;
	FVector RayStart = GetActorLocation();
	FVector RayEnd = RayStart + FVector::DownVector * (MyMesh->GetCollisionShape().GetExtent().Z * 1.10f);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	// LineTraceSingleByChannel: Raycast que devuelve el primer objeto que toque 
	// ECC_GameTraceChannel1: Canal de colisión para la traza
	DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Red, false, 1.f, 0, 2.f);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, RayStart, RayEnd, ECC_GameTraceChannel1, Params))
	{
		if (HitResult.GetActor()->ActorHasTag("Jumpable"))
		{
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), JumpSound, GetActorLocation());
			MyMesh->AddImpulse(FVector::UpVector * JumpForce);
		}
	}
}

// Recibir daño
void ABall::OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                          class AController* InstigatedBy, AActor* DamageCauser)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	BallController->OnLoseLife();
	this->Destroy();
}