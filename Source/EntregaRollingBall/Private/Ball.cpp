#include "Ball.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "MovingPlatform.h"
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
	
	MyMesh->OnComponentBeginOverlap.AddDynamic(this, &ABall::OnMeshBeginOverlap);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	// Físicas de la bola
	MyMesh->SetSimulatePhysics(true);
	MyMesh->SetMassOverrideInKg(NAME_None, 100);
	MyMesh->SetGenerateOverlapEvents(true);
	MyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MyMesh->BodyInstance.bLockRotation = false;
	MyMesh->SetLinearDamping(0.3f);
	MyMesh->SetAngularDamping(0.8f);
	
	OnTakeAnyDamage.AddDynamic(this, &ABall::OnDamageTaken);
}

// Tick para comprobar el sonido de moverse
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Gravedad
	if (bIsGravityInverted)
	{
		// 1960 = 9.8 * 2 * 100, anula la estandar de unreal y añade la misma hacia el sentido contrario
		MyMesh->AddForce(FVector(0.0f, 0.0f, 1960.0f) * MyMesh->GetMass());
	}
	
	CheckGrounded();
	
	// Calcular velocidad de una plataforma móvil en caso de estar en una y aplicarsela a la bola
	if (CurrentGround)
	{
		UStaticMeshComponent* PlatformMesh = Cast<UStaticMeshComponent>(CurrentGround->
			GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (PlatformMesh)
		{
			FVector PlatformSpeed = PlatformMesh->GetPhysicsLinearVelocity();
			FVector BallSpeed = MyMesh->GetPhysicsLinearVelocity();
			
			BallSpeed.X = PlatformSpeed.X;
			BallSpeed.Y = PlatformSpeed.Y;
			MyMesh->SetPhysicsLinearVelocity(BallSpeed);
		}
	}
	
	if (!MoveSound || !MoveAudioComponent) return;
	if (const float Speed = MyMesh->GetPhysicsLinearVelocity().Size(); Speed > MoveSoundMinSpeed && bIsGrounded)
	{
		if (!MoveAudioComponent->IsPlaying())
		{
			MoveAudioComponent->SetSound(MoveSound);
			MoveAudioComponent->Play();
		}
	}
	else if (MoveAudioComponent->IsPlaying())
	{
		MoveAudioComponent->Stop();
	}
}

void ABall::SetBallController(AMyPlayerController* MyPlayerController)
{
	this->BallController = MyPlayerController;
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
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
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
	if (!bIsGrounded) return;
	
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), JumpSound, GetActorLocation());
	FVector JumpDirection = bIsGravityInverted ? FVector::DownVector : FVector::UpVector;
	MyMesh->AddImpulse(JumpDirection * JumpForce);
}

// Recibir daño
void ABall::OnDamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                          class AController* InstigatedBy, AActor* DamageCauser)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	BallController->OnLoseLife();
	this->Destroy();
}

// Función que comprueba si el player esta en el suelo
void ABall::CheckGrounded()
{
	FHitResult HitResult;
	FVector RayStart = GetActorLocation();
	FVector Direction = bIsGravityInverted ? FVector::UpVector : FVector::DownVector;
	FVector RayEnd = RayStart + Direction * (MyMesh->GetCollisionShape().GetExtent().Z * 1.10f);
	
	// Parametros para el raycast, en este caso, que ignore al player
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bIsGrounded = false;
	CurrentGround = nullptr;
	
	// LineTraceSingleByChannel: Detectar el primer objeto con el que colisione el raycast
	if (GetWorld()->LineTraceSingleByChannel(HitResult, RayStart, RayEnd, ECC_Visibility, Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor->ActorHasTag("Jumpable")) bIsGrounded = true;
		if (HitActor->IsA(AMovingPlatform::StaticClass())) CurrentGround = HitActor;
	}
}

// Gravedad
void ABall::SetGravityInverted(bool bInverted)
{
	bIsGravityInverted = bInverted;
}
	
// Modo '2D'
void ABall::Set2DMode(bool bEnabled)
{
	FVector CurrentVelocity = MyMesh->GetPhysicsLinearVelocity();
	FVector CurrentAngularVelocity = MyMesh->GetPhysicsAngularVelocityInDegrees();
	
	// Cambio
	if (bEnabled)
	{
		SpringArm->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		SpringArm->TargetArmLength = 1500.0f;
		MyMesh->BodyInstance.bLockYTranslation = true;
	}
	// Valores por defecto
	else
	{
		SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
		SpringArm->TargetArmLength = 800.0f;
		MyMesh->BodyInstance.bLockYTranslation = false;
	}
	
	MyMesh->RecreatePhysicsState();
	MyMesh->SetPhysicsLinearVelocity(CurrentVelocity);
	MyMesh->SetPhysicsAngularVelocityInDegrees(CurrentAngularVelocity);
}